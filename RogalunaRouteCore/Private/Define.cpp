#include "Define.h"
#include <QUrlQuery>

FRouteObject::FRouteObject(const QString& path,
                           CreatorFunc creator,
                           const QString& viewName,
                           const QVariantMap& meta,
                           const QVector<FRouteObject*>& children,
                           QObject* parent)
    : QObject(parent)
    , m_path(path)
    , m_creator(std::move(creator))
    , m_viewName(viewName)
    , m_meta(meta)
    , m_widgetInstance(nullptr)
{
    parsePath();

    // 将传入的子路由设置为当前对象的子对象（自动加入 QObject 树）
    for (FRouteObject* child : children) {
        if (child) {
            child->setParent(this);
        }
    }
}

void FRouteObject::parsePath()
{
    // 1. 分离片段标识符 (#fragment)
    int fragPos = m_path.indexOf('#');
    QString basePathWithQuery;
    if (fragPos != -1) {
        m_fragment = m_path.mid(fragPos + 1);
        basePathWithQuery = m_path.left(fragPos);
    } else {
        m_fragment.clear();
        basePathWithQuery = m_path;
    }

    // 2. 分离查询参数 (?key=value...)
    int queryPos = basePathWithQuery.indexOf('?');
    QString cleanPath;
    if (queryPos != -1) {
        QString queryString = basePathWithQuery.mid(queryPos + 1);
        cleanPath = basePathWithQuery.left(queryPos);

        QUrlQuery urlQuery(queryString);
        for (const auto& item : urlQuery.queryItems(QUrl::FullyDecoded)) {
            m_queryParams.append(item.first); // 只存 key
        }
    } else {
        cleanPath = basePathWithQuery;
    }

    // 3. 提取路径段
    m_segments = cleanPath.split('/', Qt::SkipEmptyParts);

    // 4. 提取路径参数占位符（如 ":id"）
    m_pathParams.clear();
    for (const QString& seg : m_segments) {
        if (seg.startsWith(':')) {
            m_pathParams.append(seg);
        }
    }
}

QWidget* FRouteObject::createWidget()
{
    if (m_creator) {
        QWidget* w = m_creator();
        m_widgetInstance = w;
        return w;
    }
    return nullptr;
}

FRouteObject& FRouteObject::withMeta(const QVariantMap& meta)
{
    m_meta = meta;
    return *this;
}

FRouteObject& FRouteObject::withViewName(const QString& viewName)
{
    m_viewName = viewName;
    return *this;
}

FRouteObject& FRouteObject::withChildren(FRouteObject* route)
{
    if (route) {
        route->setParent(this); // 利用 QObject 的父子机制
    }
    return *this;
}

QVector<FRouteObject*> FRouteObject::getPathNodes() const
{
    QVector<FRouteObject*> path;
    const FRouteObject* current = this;
    while (current) {
        path.prepend(const_cast<FRouteObject*>(current));
        current = qobject_cast<const FRouteObject*>(current->parent());
    }
    return path;
}

QString FRouteObject::getAbsolutePath() const
{
    if (m_path == "/") {
        return "/";
    }

    QStringList parts;
    const FRouteObject* node = this;
    while (node) {
        if (node->m_path != "/") {
            parts.prepend(node->m_path);
        }
        node = qobject_cast<const FRouteObject*>(node->parent());
    }

    return "/" + parts.join("/");
}
