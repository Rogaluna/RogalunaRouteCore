#include "MainWindow.h"
#include "QGlobalVariable.h"

#include <QApplication>

#include <Page/PageHome.h>
#include <Page/PageSetting.h>

#include <Page/Home/PageOne.h>
#include <Page/Home/PageTwo.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QRouter* router = QGlobalVariables::instance()->getRouter();

    FRouteObject* routes = new FRouteObject(
        "/",
        []() -> QWidget* { return new PageSetting(); },
        "", {}, {
            new FRouteObject(
                "home",
                []() -> QWidget* { return new PageHome(); },
                "home", {},
                {
                    new FRouteObject(
                        "page1",
                        []() -> QWidget* { return new PageOne(); },
                        "page1"),
                    new FRouteObject(
                        "page2",
                        []() -> QWidget* { return new PageTwo(); },
                        "page2"),
                }
                ),
            new FRouteObject(
                "setting",
                []() -> QWidget* { return new PageSetting(); },
                "setting"),
         });

    router->install(routes, &w);

    router->push("/home", nullptr);

    return a.exec();
}
