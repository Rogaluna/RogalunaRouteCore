#ifndef ROUTEGUARD_H
#define ROUTEGUARD_H

#include <functional>
#include <QString>
#include <QVector>

// Base RouteGuard
class RouteGuard
{
public:
    RouteGuard();

    // 路由守卫
    // Guard function implement should be include judge from path logic
    // like :
    // if (from == "home") {
    //      return false; // from home page nav request all block
    // }
    using NavigationGuard = std::function<bool(const QString& from, const QString& to)>;
    virtual void addGlobalGuard(NavigationGuard guard);

    // 调用守卫
    // call guard if exist,
    // if guard is not exist, return true default nav to target
    // else verify guard logic
    virtual bool callGuard(const QString& currentPath, const QString& navigatePath);

    // Trigger

    virtual bool guardBefore(const QString& currentPath, const QString& navigatePath);
    virtual bool guardResolve(const QString& currentPath, const QString& navigatePath);
    virtual bool guardAfter(const QString& currentPath, const QString& navigatePath);

private:
    QVector<NavigationGuard> m_guards;
};

#endif // ROUTEGUARD_H
