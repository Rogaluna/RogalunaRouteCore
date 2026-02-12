#ifndef IROUTABLE_H
#define IROUTABLE_H

#include <QMap>
#include <QWidget>
#include <QRouteView.h>
#include <Define.h>

class IRoutable {
    friend class QRouter;
protected:
    // 获取视图（每一个界面中最多允许存在一个视图容器），
    virtual QRouteView* routeViews() = 0;

    // 自定义状态，在离开界面时获取返回的页面缓存值，以待还原时使用
    virtual QVariantMap cacheData() { return {}; };

    // 实例路由对象，仅在类实例化后由路由器赋值，从而将实例和路由定义本身所关联起来
    FRouteObject* m_instRouteObject = nullptr;
};

Q_DECLARE_INTERFACE(IRoutable, "IRoutable")

#endif // IROUTABLE_H
