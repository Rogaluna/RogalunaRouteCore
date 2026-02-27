#include "QRouter.h"
#include <QStack>
#include <QDebug>
#include <QWidget>
#include <QRegularExpression>

#include <View/QDefaultNotFoundView.h>

#include <QUrl>
#include <QUrlQuery>

QRouter::QRouter(QObject *parent)
    : QObject(parent)
    , m_currentRouteObject(nullptr)
    , m_notFoundViewCreator(nullptr)
    , m_history(nullptr)
{
    m_history = new QRouteHistory(this);

    // 设置默认 notFound 页面
    setNotFoundView([]() -> QWidget* { return new QDefaultNotFoundView(); });
}

void QRouter::install(FRouteObject* routes, IRoutable* rootView)
{
    // 断言传入的根视图必定是派生自 QWidget 的实体控件
    Q_ASSERT(dynamic_cast<QWidget*>(rootView) != nullptr);

    if (!rootView) {
        qWarning() << "QRouter::install: rootView is null!";
        return;
    }

    m_registeredRoutes = routes;
    m_registeredRoutes->m_widgetInstance = dynamic_cast<QWidget*>(rootView);
    m_flatRouteMap.clear();

    // 构建路由映射
    buildFlatRouteMap({routes});
}

void QRouter::setNotFoundView(CreatorFunc viewCreator)
{
    m_notFoundViewCreator = viewCreator;
}

bool QRouter::push(const QString &path, QWidget* parent, const QVariantMap& params)
{
    // 解析路径到映射的路由表中：

    // 先解析相对路径，用当前路径和传入的 path 进行拼接，如果能够找到映射的路径，直接使用并使得函数返回 true

    // 当 path 以 "/" 开头时，表示寻找绝对路径，直接解析传入的 path 路径，如果找到了，使用并使得函数返回 true

    // 如果都没有找到，则显示 notFound 页面，并返回 false

    IRoutable* rootView = dynamic_cast<IRoutable*>(m_registeredRoutes->widgetInstance().data());

    if (!rootView) {
        qWarning() << "QRouter::push: rootView not installed.";
        return false;
    }

    // === 路径标准化 ===
    QString targetPath = formatUrl(path, parent);

    // 目标页面路径变更
    emit currentRoutePathChange(targetPath);

    QVariantMap routeParams;
    QString rediectPath;
    FRouteObject* targetRoute = resolveRoute(rootView, targetPath, params, routeParams, rediectPath);
    if (targetRoute == nullptr) {
        // 如果未找到目标路由
        if (m_notFoundViewCreator != nullptr) {
            // 将 notFoundView 挂到 rootView 的第一个 routeView
            QRouteView* views = rootView->routeViews();
            if (views != nullptr) {
                QWidget* m_notFoundView = m_notFoundViewCreator();
                views->setWidget(m_notFoundView);
            } else {
                qWarning() << "Root view has no routeViews to display notFound page.";
            }
            m_currentRouteObject = nullptr;
            return true;
            // 视为“成功显示”错误页
        } else {
            qWarning() << "Route not found and no notFoundView set:" << path;
            return false;
        }
    }
    if (targetPath != rediectPath) {
        // 如果发生了重定向路径变更，更新当前页面路径
        emit currentRoutePathChange(rediectPath);
    }

    // === 获取从根到目标的路由链（顺序：[root, ..., target]）===
    QVector<FRouteObject*> routeChain = targetRoute->getPathNodes();

    // === 逐级创建并挂载 widget ===
    IRoutable* currentParentRoutable = rootView; // 初始父容器是 rootView

    // 跳过根路由
    for (auto it = routeChain.begin() + 1; it != routeChain.end(); ++it)
    {
        FRouteObject* routeNode = *it;

        // 创建或复用 widget
        QWidget* widget = routeNode->widgetInstance();
        if (!widget) {
            widget = routeNode->createWidget();

            if (!widget) {
                // 创建失败
                qWarning() << "Failed to create widget for route:" << routeNode->getAbsolutePath();
                return false;
            }
            else
            {
                // 创建成功，对界面进行初始化设置
                IRoutable* routableView = qobject_cast<IRoutable*>(widget);
                routableView->m_instRouteObject = routeNode;
            }
        }

        // 如果当前迭代的视图是末尾视图
        if (it == routeChain.end() - 1) {
            // 将解析的查询参数通知给视图实例
            IRoutable* routableView = qobject_cast<IRoutable*>(widget);
            routableView->m_routeParams = routeParams;
        }

        // 如果是第一个节点（根路由），它应被挂到 rootView 的 routeView 中
        // 否则，currentParentRoutable 是上一级 widget（已实现 IRoutable）

        QRouteView* parentViews = currentParentRoutable->routeViews();
        if (parentViews == nullptr) {
            // 没有视图容器，无法挂载
            qWarning() << "Parent routable has no routeViews for child route:"
                       << routeNode->getAbsolutePath();
            return false;
        }

        QRouteView* targetView = parentViews; // 使用第一个占位视图
        // 将视图挂载到目标视图上，在执行挂载前，会进行一个比较：
        // 如果当前挂载的视图与创建的视图指针地址一致，则表明无需覆盖挂载，否则会先解除当前挂载的视图然后将新的界面挂载到其中
        // 尾部视图例外，它永远会重新挂载
        if (it == routeChain.end() - 1 ) {
            // 如果是末尾视图，无论如何它都会重新挂载
            targetView->setWidget(widget);
        } else {
            // 非末尾视图则判定是否重用
            if (targetView->getWidget() != widget) {
                // 解除当前挂载的视图
                IRoutable* routableView = qobject_cast<IRoutable*>(targetView->getWidget());
                if (routableView != nullptr && routableView->routeViews() != nullptr) {
                    routableView->routeViews()->unsetWidget();
                }

                targetView->setWidget(widget); // 将创建的视图挂载到目标视图上
            }
        }

        // 更新 currentParentRoutable：如果当前 widget 支持 IRoutable，则作为下一级父容器
        currentParentRoutable = qobject_cast<IRoutable*>(widget);
        if (!currentParentRoutable) {
            // 还有子路由要挂，但当前 widget 不支持 IRoutable → 无法嵌套
            qWarning() << "Widget for route does not implement IRoutable, cannot nest deeper:"
                       << routeNode->getAbsolutePath();
            return false;
        }
    }

    m_currentRouteObject = targetRoute;
    emit currentRouteObjectChange(m_currentRouteObject);
    // 变更页面成功，存入历史记录
    m_history->push(targetPath);

    return true;
}

void QRouter::nextPage()
{
    QString entry;
    m_history->next(entry);
    m_history->setCanOperaState(false);

    push(entry, nullptr);
}

void QRouter::prePage()
{
    QString entry;
    if (m_currentRouteObject == nullptr)
    {
        // 如果当前页面是 notFound, 回退到可用界面

        m_history->current(entry);
    }
    else
    {
        // 否则从历史记录中取出路径

        m_history->pre(entry);
    }

    m_history->setCanOperaState(false);
    push(entry, nullptr);
}

void QRouter::refresh()
{
    m_history->setCanOperaState(false);
    IRoutable* rootView = dynamic_cast<IRoutable*>(m_registeredRoutes->widgetInstance().data());
    rootView->routeViews()->clearWidget();
    push(m_currentRouteObject->getAbsolutePath(), nullptr);
}

void QRouter::buildFlatRouteMap(const QVector<FRouteObject *> &roots)
{
    QStack<FRouteObject*> stack;

    // 将所有根节点压入栈
    for (FRouteObject* root : roots) {
        if (root) {
            stack.push(root);
        }
    }

    while (!stack.isEmpty()) {
        FRouteObject* current = stack.pop();
        QString absPath = current->getAbsolutePath();

        // 注册到扁平映射（后注册的会覆盖先注册的同路径路由）
        m_flatRouteMap[absPath] = current;

        // 遍历子节点（通过 QObject 的 children()）
        const QObjectList& children = current->children();
        for (QObject* childObj : children) {
            if (FRouteObject* childRoute = qobject_cast<FRouteObject*>(childObj)) {
                stack.push(childRoute);
            }
        }
    }
}

QString QRouter::formatUrl(const QString path, QWidget* parent)
{
    // === 路径标准化 ===
    QString targetPath;
    if (path.startsWith("./")) {
        // 相对当前路径
        if (m_currentRouteObject) {
            targetPath = m_currentRouteObject->getAbsolutePath() + "/" + path;
        } else {
            targetPath = "/" + path;
        }
    } else if (path.startsWith('/')) {
        // 使用绝对路径
        targetPath = path;
    } else {
        // 相对指定的父级
        // Q_ASSERT(parent != nullptr && qobject_cast<IRoutable*>(parent));

        if (parent != nullptr && qobject_cast<IRoutable*>(parent)) {
            m_currentRouteObject = qobject_cast<IRoutable*>(parent)->m_instRouteObject;
        }

        if (m_currentRouteObject) {
            targetPath = m_currentRouteObject->getAbsolutePath() + "/" + path;
        } else {
            targetPath = "/" + path;
        }
    }

    // 标准化：合并斜杠、去尾斜杠
    targetPath = targetPath.trimmed();
    if (targetPath.isEmpty()) targetPath = "/";
    static const QRegularExpression multiSlashRegex("/+");
    targetPath.replace(multiSlashRegex, "/");
    if (targetPath.length() > 1 && targetPath.endsWith('/'))
        targetPath.chop(1);
    if (!targetPath.startsWith('/'))
        targetPath.prepend('/');

    return targetPath;
}

FRouteObject *QRouter::resolveRoute(IRoutable* rootView, const QString &targetPath, const QVariantMap &params, QVariantMap &outRouteParams, QString& outRediectPath)
{
    outRediectPath = targetPath;
    // === 提取查询参数 ===
    QString queryPart;
    QString purePath = outRediectPath;
    int queryPos = purePath.indexOf('?');
    if (queryPos != -1) {
        queryPart = purePath.mid(queryPos + 1);
        purePath = purePath.left(queryPos); // 去掉查询部分，保留纯路径
    }
    QUrlQuery urlQuery(queryPart);
    for (const auto &key : urlQuery.queryItems(QUrl::FullyDecoded)) {
        // 注意：如果同一个 key 出现多次，后面的会覆盖前面的
        outRouteParams.insert(key.first, key.second);
    }

    // === 查找路由 ===
    FRouteObject* targetRoute = m_flatRouteMap.value(purePath, nullptr);

    // === 处理未找到情况 ===
    if (!targetRoute) {
        return targetRoute;
    }

    // === 处理重定向 ===
    // 如果在携带的元数据中存在名为 [ redirect ] 的键，将其值作为增补路径添加到 purePath 中去
    if (targetRoute->meta().contains("redirect")) {
        QString redirectSeg = targetRoute->meta().value("redirect").toString();
        purePath = purePath + "/" + redirectSeg;
        outRediectPath = purePath;

        if (!queryPart.isEmpty()) {
            // 如果原先路径中存在查询参数，将查询参数加到末尾
            queryPart = purePath.mid(queryPos + 1);
            outRediectPath = purePath + "/" + queryPart;
        }

        return resolveRoute(rootView, outRediectPath, params, outRouteParams, outRediectPath);
    }

    // 从传入参数中提取路由参数并覆写到输出参数
    outRouteParams.insert(params);
    return targetRoute;
}
