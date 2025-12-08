#include "MainWindow.h"
#include "QGlobalVariable.h"

#include <QApplication>

#include <Page/PageHome.h>
#include <Page/PageSetting.h>

#include <Page/Home/PageOne.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QRouter* router = QGlobalVariables::instance()->getRouter();

    QVector<FRouteObject*> routes =  {

    new FRouteObject(
        "/home",
        []() -> QWidget* { return new PageHome(); },
        "home", {},
            {
                new FRouteObject(
                    "/page1",
                    []() -> QWidget* { return new PageOne(); },
                    "page1"),
            }
        ),
    new FRouteObject(
        "/setting",
        []() -> QWidget* { return new PageSetting(); },
        "setting"),
    };

    router->install(routes, &w);

    QObject::connect(router, &QRouter::routeChanged,
                     [](const FRouteDesc& desc) {
                         qDebug() << "Route changed to:" << desc.m_path;
                     });

    router->push("/home");

    return a.exec();
}
