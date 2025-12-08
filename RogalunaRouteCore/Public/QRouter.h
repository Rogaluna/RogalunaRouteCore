#ifndef QROUTER_H
#define QROUTER_H

#include <QObject>
#include <Define.h>

#include <Interface/IRoutable.h>

/**
 * @brief The QRoute class
 * 路由调配类，需要在应用全局作用域内挂载，建议使用单例携带此变量。
 * 用于导航、前进、后退到注册的页面中。
 */
class ROGALUNAROUTECORE_EXPORT QRouter : public QObject
{
    Q_OBJECT
public:
    explicit QRouter(QObject *parent = nullptr);

    /**
     * @brief 核心注册方法：接受 FRouteObject 指针构成的路由树
     * @param routes 路由树的顶层节点列表
     * @param rootView 根视图，必须是一个实现了接口 IRoutable 的界面
     */
    void install(const QVector<FRouteObject*>& routes, IRoutable* rootView);

    /**
     * @brief 设置未定义路由的界面，允许在实际工程内使用自定义的未定义界面
     * @param view 要进行设置的控件指针
     */
    void setNotFoundView(QWidget* view);

    /**
     * @brief 导航到指定路径（绝对路径）
     * 根据路径查找匹配的路由，创建对应的页面 Widget，并将其显示在配置的容器中。
     * @param path 完整目标路径 (e.g., "/user/123", "/settings/profile")
     * @return true 导航成功, false 导航失败 (如未找到匹配路由)
     */
    bool navTo(const QString& fullPath);

    /**
     * @brief 导航到指定路径（相对路径）
     * @param path 相对的目标路径（e.g., "/page")
     * @return true 导航成功, false 导航失败 (如未找到匹配路由)
     */
    bool push(const QString& path);

signals:
    /**
     * @brief 路由改变信号
     * 当 push 成功执行后发射此信号。
     * @param newRouteDesc 新路由的描述信息 (包含了路径、参数等)
     */
    void routeChanged(const FRouteDesc& newRouteDesc);

private:
    /**
     * @brief 递归地注册路由（扁平化路径以便快速查找）
     * @param routes 当前层级的路由列表
     * @param parentPath 父级路径前缀
     */
    void registerRoutesRecursively(const QVector<FRouteObject*>& routes, const QString& parentPath = "");
    /**
     * @brief 根据完整路径查找匹配的路由对象及路径参数
     * 支持静态路径和带 :paramName 的动态路径匹配。
     * @param fullPath 要匹配的完整路径 (e.g., "/user/456")
     * @return QPair<FRouteObject*, QMap<QString, QString>>
     *         first: 匹配的路由对象指针 (若未找到则为 nullptr)
     *         second: 提取的路径参数 (key: param name, value: param value)
     */
    QPair<FRouteObject*, QMap<QString, QString>> findMatchingRoute(const QString& fullPath) const;

private:
    QVector<FRouteObject*> m_registeredRoutes;              // 存储原始注册的路由树（主要用于 install）
    QMap<QString, FRouteObject*> m_flatRouteMap;            // 扁平化的路径到 FRoute 指针的映射，便于快速查找
    QString m_currentPath;                                  // 当前导航到的完整路径
    IRoutable *m_rootView;                                  // 根视图，所有后加载的页面都会在此处生成

    QWidget *m_notFoundView;                                // 默认视图，当找不到路径的时候，会生成该界面
};

#endif // QROUTER_H
