#include <RouteGuard.h>
#include <QDebug>

FRouteGuard::FRouteGuard() {}

void FRouteGuard::addGlobalGuard(NavigationGuard guard)
{
    m_guards.append(guard);
}

bool FRouteGuard::callGuard(const QString& currentPath, const QString& navigatePath)
{
    for (auto& guard : m_guards) {
        if (!guard(currentPath, navigatePath)) {
            qWarning() << "Navigation blocked by guard:" << navigatePath;
            return false;
        }
    }

    return true;
}

bool FRouteGuard::guardBefore(const QString &currentPath, const QString &navigatePath)
{
    return callGuard(currentPath, navigatePath);
}

bool FRouteGuard::guardResolve(const QString &currentPath, const QString &navigatePath)
{
    return callGuard(currentPath, navigatePath);
}

bool FRouteGuard::guardAfter(const QString &currentPath, const QString &navigatePath)
{
    return callGuard(currentPath, navigatePath);
}
