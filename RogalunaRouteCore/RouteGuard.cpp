#include "RouteGuard.h"
#include <QDebug>

RouteGuard::RouteGuard() {}

void RouteGuard::addGlobalGuard(NavigationGuard guard)
{
    m_guards.append(guard);
}

bool RouteGuard::callGuard(const QString& currentPath, const QString& navigatePath)
{
    for (auto& guard : m_guards) {
        if (!guard(currentPath, navigatePath)) {
            qWarning() << "Navigation blocked by guard:" << navigatePath;
            return false;
        }
    }

    return true;
}

bool RouteGuard::guardBefore(const QString &currentPath, const QString &navigatePath)
{
    return callGuard(currentPath, navigatePath);
}

bool RouteGuard::guardResolve(const QString &currentPath, const QString &navigatePath)
{
    return callGuard(currentPath, navigatePath);
}

bool RouteGuard::guardAfter(const QString &currentPath, const QString &navigatePath)
{
    return callGuard(currentPath, navigatePath);
}
