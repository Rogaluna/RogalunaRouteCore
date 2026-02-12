# RogalunaRouteCore

**RogalunaRouteCore** 是一个为Qt应用程序设计的轻量级路由管理库。它提供了类似Web开发中的路由功能，使得在Qt中管理多页面切换、参数传递和页面生命周期变得更加简单和高效。

## 功能特性

- **声明式路由注册**：通过宏简单注册路由，指定路径、页面组件和视图容器。
- **路由导航**：支持通过路径导航到指定页面，并可传递参数。
- ~~**路由守卫**：在导航前、解析时、导航后三个阶段进行拦截和控制。~~
- ~~**页面生命周期**：通过 `IRoutable`接口实现页面进入、离开和参数接收的生命周期回调~~。
- **内存管理**：自动管理页面组件的生命周期。
- **嵌套路由**：支持多个路由视图容器，实现复杂布局。

## 核心组件

### QRouter

路由管理器，单例模式，负责路由的注册、导航和参数传递。
主要方法：

- `install(FRouteObject* routes, IRoutable* rootView)`: 注册树型路由对象到指定根界面内。

```cpp
{
    ...
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
    ...
}
```

- `push(const QString &path, QWidget* parent)`: 导航到指定路径。

```cpp
{
    QRouter* router = QGlobalVariables::instance()->getRouter()

    // 用法一: 绝对导航到 "/home"
    // 比如当前路径是 "/" ，导航到 "/home"
    // 当前路径是 "/setting" ，同样导航到 "/home"
    router->push("/home", nullptr);

    ...

    // 用法二: 相对导航到 "./setting"
    // 比如当前路径是 "/" ，导航到 "/setting"
    // 当前路径是 "/home" ，则会导航到 "/home/setting"
    // 当前路径是 "/setting" ，则会导航到 "/setting/setting"
    router->push("./setting", nullptr);

    ...

    // 用法三: 自动导航到 "page1"
    // 比如当前路径是 "/home" ，而 this 指向 "/home" ，导航到 "/home/page1"
    // 当前路径是 "/home/page2" ，而 this 指向 "/home" ，同样导航到 "/home/page1"
    // 如果第二个参数传 nullptr，则默认使用相对导航
    router->push("page1", this);
}
```

- `prePage()`: 跳转到上一页（如果可以跳转的话）
- `nextPage()`: 跳转到下一页（如果可以跳转的话）
- `refresh()`: 刷新当前页面

### IRoutable

标识视图页面接口，只有实现此接口的类才可以被 router 识别

```cpp
#include <QWidget>

#include <Interface/IRoutable.h>

namespace Ui {
class PageOne;
}

class PageOne : public QWidget, public IRoutable
{
    Q_OBJECT
    Q_INTERFACES(IRoutable)
public:
    explicit PageOne(QWidget *parent = nullptr);
    ~PageOne();

protected:
    virtual QRouteView* routeViews() override { return nullptr; };

private:
    Ui::PageOne *ui;
};
```

### QRouteView

路由视图容器，继承自 `QWidget`，这是一个容器类，用于承载路由视图指向的界面。如果需要让当前的页面拥有子页面，则需要视图容器放置到 ui 中：
![1769614406506](images/README/1769614406506.png)
示例图中，名为 container 是一个 QRouteView，它在 ui 界面后退、前进、刷新按钮的下方空白处。
在 ui 中设置了容器后，还需要修改类，实现以下接口函数：

```cpp
QRouteView *MainWindow::routeViews()
{
    // 返回在 ui 中设置的容器，使得 router 可以寻找到要添加视图的目标
    return ui->container;
}
```

### ~~FRouteGuard~~

~~路由守卫，用于在导航过程中进行拦截。可以添加多个全局守卫。~~


## 示例

见附带的 Test 项目

