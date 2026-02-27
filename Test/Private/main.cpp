#include "MainWindow.h"
#include "QGlobalVariable.h"

#include <QApplication>

#include <Application/PageHome.h>
#include <Application/PageSetting.h>
#include <Application/PageWelcome.h>

#include <Application/Home/PageManual.h>
#include <Application/Home/PageRedirectExample.h>

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
        []() -> QWidget* { return nullptr; },
        "", {},
        {
            new FRouteObject(
                "home",
                []() -> QWidget* { return new PageHome(); },
                "home", {{"redirect", "manual"}},
                {
                    new FRouteObject(
                        "manual",
                        []() -> QWidget* { return new PageManual(); },
                        "manual"),
                }
                )
            ,
            new FRouteObject(
                "setting",
                []() -> QWidget* { return new PageSetting(); },
                "setting")
            ,
            new FRouteObject(
                "welcome",
                []() -> QWidget* { return new PageWelcome(); },
                "welcome")
        });

    // 将路由安装到根视图中
    router->install(routes, &w);

    router->push("/welcome");

    return a.exec();
}
