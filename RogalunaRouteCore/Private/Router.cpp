#include "Router.h"

#include <QRegularExpressionMatch>
#include <QDebug>
#include <RouteGuard.h>
#include <QRouteView.h>

#include <Interface/IRoutable.h>

FRoute::FRoute(const QString& path,
               CreatorFunc creator,
               const QString& viewName,
               const QVariantMap& meta)
    : m_path(path),
    m_creator(creator),
    m_viewName(viewName),
    m_meta(meta),
    m_widgetInstance(nullptr)
{}

QWidget *FRoute::createWidget()
{
    if (m_widgetInstance.isNull()) {
        m_widgetInstance = m_creator();
    }
    return m_widgetInstance;
}

FRoute &FRoute::withMeta(const QVariantMap &meta)
{
    m_meta = meta;
    return *this;
}

FRoute &FRoute::withViewName(const QString &viewName)
{
    m_viewName = viewName;
    return *this;
}

/******************************************************************************************************
 *
 ******************************************************************************************************/

/******************************************************************************************************
 *
 ******************************************************************************************************/

QRouter* QRouter::m_instance = nullptr;

QRouter *QRouter::instance()
{
    if (m_instance == nullptr)
    {
        m_instance = new QRouter;
    }

    return m_instance;
}

QRouter::QRouter(QObject* parent) :
    QObject(parent),
    m_routeGuard(new FRouteGuard),
    m_currentRoute(nullptr)
{}

void QRouter::registerViewContainer(QRouteView *container, const QString &viewName)
{
    m_viewContainers[viewName] = container;
}

void QRouter::unregisterViewContainer(const QString &viewName)
{
    m_viewContainers.remove(viewName);
}

void QRouter::registerRoute(FRoute* route)
{
    const QString& path = route->path();
    // 写入路由映射表
    m_routes[path] = route;
}

bool QRouter::navigateTo(const QString &path, const QVariantMap& params)
{
    // 拷贝当前路径名称，如果在初始化阶段，则设置为空字符串
    QString currentPath = (m_currentRoute != nullptr) ?
                              m_currentRoute->path() :
                              QString();

    // 前置守卫
    if (!m_routeGuard->guardBefore(currentPath, path))
    {
        return false;
    }

    // 解析路径到路由导航结果
    NavigationResult result = resolvePath(path);

    // 解析守卫
    if (!m_routeGuard->guardResolve(currentPath, path))
    {
        return false;
    }

    // 应用路由变更
    applyRouteChanges(result, params);

    // 后置守卫
    if (!m_routeGuard->guardAfter(currentPath, path))
    {
        return false;
    }

    return true;
}

bool QRouter::sendParams(const QString &path, const QVariantMap &params)
{
    // 获取目标页面
    QWidget* widget = nullptr;
    FRoute* route = m_routes[path];
    if (route != nullptr)
    {
        widget = route->widgetInstance();
    }

    if (widget != nullptr)
    {
        if (IRoutable* routable = qobject_cast<IRoutable*>(widget))
        {
            routable->receiveParams(params);

            return true;
        }
    }

    return false;
}

void QRouter::setRouteGuard(FRouteGuard *guard)
{
    m_routeGuard = guard;
}

QRouter::NavigationResult QRouter::resolvePath(const QString &path) const
{
    NavigationResult result;

    result.sourceRoute = m_currentRoute;
    result.targetRoute = m_routes[path];

    return result;
}

void QRouter::applyRouteChanges(const NavigationResult &result, const QVariantMap& params)
{
    const QString& viewName = result.targetRoute->viewName();
    QRouteView* targetContainer = viewContainer(viewName);

    // QWidget* sourceWidget = result.sourceRoute->widgetInstance();
    // QWidget* targetWidget = result.targetRoute->widgetInstance();

    // 如果存在控件，使用它；不存在，创建它
    QWidget* targetWidget = result.targetRoute->widgetInstance();
    if (targetWidget == nullptr) {
        targetWidget = result.targetRoute->createWidget();
    }

    // 添加到视图容器
    targetContainer->addWidget(targetWidget);

    targetContainer->setCurrentWidget(targetWidget);

    // 调用上页面的离开函数
    if (m_currentRoute && m_currentRoute->widgetInstance()) {
        if (IRoutable* routable = qobject_cast<IRoutable*>(m_currentRoute->widgetInstance())) {
            routable->leaveRoute();
        }
    }

    // 更新当前路由
    m_currentRoute = result.targetRoute;

    // 调用此页面的进入函数
    if (result.targetRoute->widgetInstance()) {
        if (IRoutable* routable = qobject_cast<IRoutable*>(result.targetRoute->widgetInstance())) {
            routable->enterRoute(params);
        }
    }

    // 清理孤立控件
    clearOrphanedWidget();
}

void QRouter::clearOrphanedWidget()
{
    // 清理策略：
    // 1. 当前显示的页面保留
    // 2. 设置了 keepAlive 元数据的页面保留
    // 3. 清理孤立控件

    // 获取当前活动且不为 KeepAlive 的路由
    QList<FRoute*> activeRoutes;
    for (auto& r : m_routes)
    {
        if (!r->widgetInstance().isNull() && !r->meta().value("keepAlive", false).toBool())
        {
            QWidget* activeWidget = r->widgetInstance();

            // 排除掉当前页面
            if (activeWidget != m_currentRoute->widgetInstance())
            {
                activeRoutes.append(r);
            }
        }
    }

    // 清理孤立控件
    for (auto& activeRoute : activeRoutes)
    {
        // 获取控件所在的容器
        const QString& viewName = activeRoute->viewName();
        QRouteView* routeView = viewContainer(viewName);

        // 移除并清理控件
        routeView->removeWidget(activeRoute->widgetInstance());
        activeRoute->widgetInstance()->deleteLater();
    }
}

