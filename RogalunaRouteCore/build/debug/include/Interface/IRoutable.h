#ifndef IROUTABLE_H
#define IROUTABLE_H

#include <QMap>
#include <QWidget>
#include <QRouteView.h>

class IRoutable {
    friend class QRouter;
protected:
    // // 进入页面(仅在切入页面的时候触发)
    // virtual void enterRoute(const QVariantMap& params) = 0;

    // // 离开页面
    // virtual void leaveRoute() = 0;

    // // 接收页面参数(可在任何时候触发)
    // virtual void receiveParams(const QVariantMap& params) = 0;

    // 获取视图（每一个界面中最多允许存在一个视图容器），
    virtual QRouteView* routeViews() = 0;

};

Q_DECLARE_INTERFACE(IRoutable, "IRoutable")

#endif // IROUTABLE_H
