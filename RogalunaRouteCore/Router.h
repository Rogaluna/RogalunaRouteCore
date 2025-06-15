#ifndef ROUTER_H
#define ROUTER_H

#include <RogalunaRouteCore_Global.h>
#include <QObject>
#include <QMap>
#include <QRegularExpression>
#include <QPointer>
#include <QTimer>

class QRouteView;
class RouteGuard;

// 常规路由，不携带任何元数据
#define REGISTER_NORMAL_ROUTE(path, Class, viewName)        \
Router::instance()->registerRoute(                          \
    new Route(path, []() -> QWidget* {                      \
        QWidget* page = new Class;                          \
        return page;                                        \
    }, viewName)                                            \
    );                                                      \

// 持久路由，携带 keepAlive 元数据
#define REGISTER_PERSISTENT_ROUTE(path, Class, viewName)    \
Router::instance()->registerRoute(                          \
    new Route(path, []() -> QWidget* {                      \
            QWidget* page = new Class;                      \
            return page;                                    \
    }, viewName, {{"keepAlive", true}})                     \
    );                                                      \

// 重定向
#define REDIRECT_ROUTE(navPath)                             \
QTimer::singleShot(0, this, [](){                           \
    Router::instance()->navigateTo(navPath);                \
});                                                         \

/**
 * @brief The Route class
 * 路由类
 * 结构中包含：
 * - 路由路径
 * - 控件创建函数（创建路由到的控件）
 * - 绑定的视图名称（创建的控件绑定的 QRouteView 名称）
 * - 元数据
 */
class ROGALUNAROUTECORE_EXPORT Route {
public:
    using Creator = std::function<QWidget*()>;

    // 主构造函数
    explicit Route(const QString& path,
                   Creator creator,
                   const QString& viewName,
                   const QVariantMap& meta = {});
public: // 方法

    QWidget* createWidget();

    Route& withMeta(const QVariantMap& meta);
    Route& withViewName(const QString& viewName);


public: // 访问器

    const QString& path() const { return m_path; }

    QPointer<QWidget> widgetInstance() const { return m_widgetInstance; }

    // 元数据
    QVariantMap meta() const { return m_meta; }

    // 目标视图名称
    const QString& viewName() const { return m_viewName; }

private:
    QString m_path;                         // 路由路径
    Creator m_creator;                      // 组件创建函数
    QString m_viewName;                     // 目标视图名称
    QVariantMap m_meta;                     // 添加元数据存储

    QPointer<QWidget> m_widgetInstance;              // 创建的控件实例
};

/******************************************************************************************************
 *
 ******************************************************************************************************/

/******************************************************************************************************
 *
 ******************************************************************************************************/

/**
 * @brief The Router class
 * 路由器类
 * 用于管理、调用路由的工具
 * 主要方法有：
 * - registerViewContainer      注册视图容器
 * - unregisterViewContainer    注销视图容器
 * - registerRoute              注册路由
 * - navigateTo                 导航路由
 * - sendParams                 发送参数到目标页面
 * 提供的信号：
 * - routeChanged               在路由变更时触发
 * 槽函数：
 * -
 *
 * 内置结构：
 * - NavigationResult           查询到的导航结果
 */
class ROGALUNAROUTECORE_EXPORT Router : public QObject
{
    Q_OBJECT
public:
    static Router* instance();

    // 注册视图容器
    void registerViewContainer(QRouteView* container, const QString& viewName);

    // 解除注册视图容器
    void unregisterViewContainer(const QString& viewName);

    // 注册路由
    void registerRoute(Route* route);

    // 导航函数
    bool navigateTo(const QString& path);

    // 发送参数到目标路由
    bool sendParams(const QString& path, const QVariantMap& params);

public:
    // 访问器 get

    // 获取当前显示路由
    Route* currentRoute() const { return m_currentRoute; };
    // 获取名称对应的视图容器
    QRouteView* viewContainer(const QString& name) const { return m_viewContainers[name]; };

public:
    // 访问器 set

    // 设置路由守卫
    void setRouteGuard(RouteGuard* guard);

signals:
    void routeChanged(const QString& newPath);

private:

    explicit Router(QObject* parent = nullptr);
    ~Router() = default;

    /**
     * @brief The NavigationResult struct
     * 导航结果结构体
     * - targetPath                 目标路径，终点位置的字符串
     * - targetRoute                目标路由，如果为空，则说明没有匹配到
     * -
     */
    struct NavigationResult
    {
        Route* sourceRoute;
        Route* targetRoute;
    };

    // 匹配路径，获取导航结果
    NavigationResult resolvePath(const QString& path) const;
    // 应用导航结果，如果可行，将会跳转到对应的路由
    void applyRouteChanges(const NavigationResult& result);
    // 清理孤立页面
    void clearOrphanedWidget();

private:
    static Router* m_instance;

    RouteGuard* m_routeGuard;                                           // 路由守卫
    QMap<QString, Route*> m_routes;                                     // 已注册的路由
    QMap<QString, QRouteView*> m_viewContainers;                        // 已注册的视图容器，唯一对应容器名

    Route* m_currentRoute;                                              // 当前显示的路由
};

#endif // ROUTER_H
