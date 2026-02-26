#ifndef IROUTABLE_H
#define IROUTABLE_H

#include <QMap>
#include <QWidget>
#include <QRouteView.h>
#include <Define.h>

class IRoutable {
    friend class QRouter;
    friend class QRouteView;
public:

    // 当前页面被挂载后触发的函数
    virtual void mounted() {};

    // 当前页面解除挂载后触发的函数
    virtual void unmounted() {};

protected:
    // 获取视图（每一个界面中最多允许存在一个视图容器），
    virtual QRouteView* routeViews() = 0;

    // 实例路由对象，仅在类实例化后由路由器赋值，从而将实例和路由定义本身所关联起来
    FRouteObject* m_instRouteObject = nullptr;

    // 路由传参（包括查询参数和路由参数，统一放置在这个变量中）
    QVariantMap m_routeParams = {};
};

Q_DECLARE_INTERFACE(IRoutable, "IRoutable")

#endif // IROUTABLE_H
