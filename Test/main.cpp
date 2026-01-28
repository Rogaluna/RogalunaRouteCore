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
    // 根视图
    MainWindow w;
    w.show();

    // QGlobalVariables 是一个全局单例类，将 router 实例化到此处以便全局访问
    QRouter* router = QGlobalVariables::instance()->getRouter();

    // 树形路由对象
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

    // 将路由安装到根视图中
    router->install(routes, &w);

    router->push("/home", nullptr);

    return a.exec();
}
