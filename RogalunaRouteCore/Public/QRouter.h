#ifndef QROUTER_H
#define QROUTER_H

#include <QObject>
#include <Define.h>

#include <Interface/IRoutable.h>

#include <QRouteHistory.h>

/**
 * @brief The QRoute class
 * 路由调配类，需要在应用全局作用域内挂载，建议使用单例携带此变量。
 * 用于导航、前进、后退到注册的页面中。
 */
class ROGALUNAROUTECORE_EXPORT QRouter : public QObject
{
    Q_OBJECT
public:
    using CreatorFunc = std::function<QWidget*()>;
public:
    explicit QRouter(QObject *parent = nullptr);

    /**
     * @brief 核心注册方法：接受 FRouteObject 指针构成的路由树
     * @param routes 路由树的顶层节点列表
     * @param rootView 根视图，必须是一个实现了接口 IRoutable 的界面
     */
    void install(FRouteObject* routes, IRoutable* rootView);

    /**
     * @brief 设置未定义路由的界面，允许在实际工程内使用自定义的未定义界面
     * @param view 要进行设置的控件指针
     */
    void setNotFoundView(CreatorFunc view);

    /**
     * @brief 导航到指定路径（相对路径）
     * @param path 相对的目标路径（e.g., "/page")
     * @param parent 指定 push 的控件，当路径使用默认写法时，这个参数必须非空，并且需要使用 IRoutable 的子类，一般使用 this 即可
     * @return true 导航成功, false 导航失败 (如未找到匹配路由)
     */
    bool push(const QString &path, QWidget* parent);

    /**
     * @brief 下一页
     */
    void nextPage();

    /**
     * @brief 上一页
     */
    void prePage();

    /**
     * @brief 刷新（销毁根容器内的页面并重新生成）
     */
    void refresh();

signals:
    /**
     * @brief 当前路径发生变更，在进入 push 时触发，变更即将进入的路由路径
     * @param path
     */
    void currentRoutePathChange(const QString& path);

    /**
     * @brief 当前路由对象变更信号，只有在当前路由对象变更时才触发
     */
    void currentRouteObjectChange(FRouteObject *currentRouteObject);


private:
    void buildFlatRouteMap(const QVector<FRouteObject*>& roots);

private:
    FRouteObject* m_registeredRoutes;                       // 存储原始注册的路由树（主要用于 install）
    QMap<QString, FRouteObject*> m_flatRouteMap;            // 扁平化的路径到 FRoute 指针的映射，便于快速查找
    FRouteObject *m_currentRouteObject;                     // 当前视图路由对象

    CreatorFunc m_notFoundViewCreator;                      // 默认视图创建器，当找不到路径的时候，会生成该界面

    QRouteHistory* m_history;                               // 历史记录对象
};

#endif // QROUTER_H
