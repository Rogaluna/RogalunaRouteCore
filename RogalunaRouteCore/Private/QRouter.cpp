#include "QRouter.h"
#include <QStack>
#include <QDebug>
#include <QWidget>
#include <QRegularExpression>

#include <View/QDefaultNotFoundView.h>

QRouter::QRouter(QObject *parent)
    : QObject(parent)
    , m_currentRouteObject(nullptr)
    , m_notFoundViewCreator(nullptr)
    , m_history(nullptr)
{
    m_history = new QRouteHistory(this);
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

    // 设置默认 notFound 页面
    setNotFoundView([]() -> QWidget* { return new QDefaultNotFoundView(); });
}

void QRouter::setNotFoundView(CreatorFunc viewCreator)
{
    m_notFoundViewCreator = viewCreator;
}

bool QRouter::push(const QString &path, QWidget* parent)
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

    // 目标页面路径变更
    emit currentRoutePathChange(targetPath);

    // === 查找路由 ===
    FRouteObject* targetRoute = m_flatRouteMap.value(targetPath, nullptr);

    // === 处理未找到情况 ===
    if (!targetRoute) {
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
            return true; // 视为“成功显示”错误页
        } else {
            qWarning() << "Route not found and no notFoundView set:" << path;
            return false;
        }
    }

    // === 获取从根到目标的路由链（顺序：[root, ..., target]）===
    QVector<FRouteObject*> routeChain = targetRoute->getPathNodes();

    // === 逐级创建并挂载 widget ===
    IRoutable* currentParentRoutable = rootView; // 初始父容器是 rootView

    // 跳过根路由
    for (int i = 1; i < routeChain.size(); ++i) {
        FRouteObject* routeNode = routeChain[i];

        // 创建或复用 widget
        QWidget* widget = routeNode->widgetInstance();
        if (!widget) {
            widget = routeNode->createWidget();
            qobject_cast<IRoutable*>(widget)->m_instRouteObject = routeNode;

            if (!widget) {
                // 创建失败
                qWarning() << "Failed to create widget for route:" << routeNode->getAbsolutePath();
                return false;
            }
        } else {
            // 即便 widget 还存在在内存中，也需要将其中解除容器内控件与其容器的关联
            // TODO: 可能有问题...
            if (qobject_cast<IRoutable*>(widget) && qobject_cast<IRoutable*>(widget)->routeViews() != nullptr) {
                qobject_cast<IRoutable*>(widget)->routeViews()->unsetWidget();
            }
        }

        // 如果是第一个节点（根路由），它应被挂到 rootView 的 routeView 中
        // 否则，currentParentRoutable 是上一级 widget（已实现 IRoutable）

        auto parentViews = currentParentRoutable->routeViews();
        if (parentViews == nullptr) {
            qWarning() << "Parent routable has no routeViews for child route:"
                       << routeNode->getAbsolutePath();
            return false;
        }

        QRouteView* targetView = parentViews; // 使用第一个占位视图
        targetView->setWidget(widget);

        // 更新 currentParentRoutable：如果当前 widget 支持 IRoutable，则作为下一级父容器
        currentParentRoutable = qobject_cast<IRoutable*>(widget);
        if (!currentParentRoutable && i < routeChain.size() - 1) {
            // 还有子路由要挂，但当前 widget 不支持 IRoutable → 无法嵌套
            qWarning() << "Widget for route does not implement IRoutable, cannot nest deeper:"
                       << routeNode->getAbsolutePath();
            return false;
        }
    }

    m_currentRouteObject = targetRoute;
    emit currentRouteObjectChange(m_currentRouteObject);
    // 变更页面成功，存入历史记录
    m_history->push(m_currentRouteObject->getAbsolutePath());

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
    m_history->pre(entry);
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
