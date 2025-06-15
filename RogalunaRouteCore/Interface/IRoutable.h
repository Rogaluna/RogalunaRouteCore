#ifndef IROUTABLE_H
#define IROUTABLE_H

#include <QMap>
#include <QWidget>

class IRoutable {
public:
    // 进入页面(仅在切入页面的时候触发)
    virtual void enterRoute() = 0;

    // 离开页面
    virtual void leaveRoute() = 0;

    // 接收页面参数(可在任何时候触发)
    virtual void receiveParams(const QVariantMap& params) = 0;
};
Q_DECLARE_INTERFACE(IRoutable, "IRoutable")

#endif // IROUTABLE_H
