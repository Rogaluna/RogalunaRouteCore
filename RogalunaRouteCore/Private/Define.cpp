#include <Define.h>

#include <QUrlQuery>

FRouteDesc::FRouteDesc(const QString &_path)
    : m_path(_path)
{
    parsePath();
}

void FRouteDesc::parsePath()
{
    // 1. 分离片段标识符 (#fragment)
    int fragmentPos = m_path.indexOf('#');
    if (fragmentPos != -1) {
        m_fragment = m_path.mid(fragmentPos + 1);
        // 基础路径先去掉片段
        m_basePath = m_path.left(fragmentPos);
    } else {
        m_basePath = m_path;
    }

    // 2. 分离查询参数 (?key=value&...)
    int queryPos = m_basePath.indexOf('?');
    QString pathWithoutQuery = m_basePath;
    if (queryPos != -1) {
        QString queryString = m_basePath.mid(queryPos + 1);
        pathWithoutQuery = m_basePath.left(queryPos);

        // --- 修改点：只存储查询参数的键（QList<QString) ---
        QUrlQuery urlQuery(queryString);
        QList<QPair<QString, QString>> queryItems = urlQuery.queryItems(QUrl::FullyDecoded);
        for (const auto& item : queryItems) {
            // 只添加键（如 key1, key2），忽略值
            m_queryParams.append(item.first);
        }

        // 更新基础路径，移除查询部分
        m_basePath = pathWithoutQuery;
    }

    // 3. 解析路径段 (基于最终的 m_basePath)
    m_segments = m_basePath.split('/', Qt::SkipEmptyParts);
}

FRouteObject::FRouteObject(const QString &path, CreatorFunc creator, const QString &viewName, const QVariantMap &meta, const QVector<FRouteObject*>& children)
    : m_routeDesc(path)
    , m_creator(std::move(creator)) // 移动语义
    , m_viewName(viewName)
    , m_meta(meta)
    , m_widgetInstance(nullptr)
    , m_children(children)
    , m_parent(nullptr)
{

    for (const auto& child : children)
    {
        child->setParent(this);
    }

}

QWidget *FRouteObject::createWidget()
{
    if (m_creator) {
        QWidget* widget = m_creator();
        // 更新内部持有的实例指针（如果需要跟踪最后一次创建的实例）
        // 注意：QPointer 会在指向的对象被 delete 后自动置空
        m_widgetInstance = widget;
        return widget;
    }
    return nullptr; // creator 未设置或无效
}

FRouteObject &FRouteObject::withMeta(const QVariantMap &meta)
{
    m_meta = meta;
    return *this; // 返回自身引用以支持链式调用 (fluent interface)
}

FRouteObject &FRouteObject::withViewName(const QString &viewName)
{
    m_viewName = viewName;
    return *this; // 返回自身引用以支持链式调用
}

FRouteObject &FRouteObject::withChildren(FRouteObject* route)
{
    m_children.append(route);
    return *this;
}

QVector<FRouteObject *> FRouteObject::getPathNodes() const
{
    QVector<FRouteObject*> path;

    // 1. 向上回溯收集所有祖先节点（包括自己）
    FRouteObject* current = const_cast<FRouteObject*>(this);
    while (current) {
        path.prepend(current);
        current = const_cast<FRouteObject*>(current->parent()); // parent() is const, cast for assignment
    }

    return path;
}
