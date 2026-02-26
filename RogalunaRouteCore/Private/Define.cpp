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
    // 将传入的子路由设置为当前对象的子对象（自动加入 QObject 树）
    for (FRouteObject* child : children) {
        if (child) {
            child->setParent(this);
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
