# RogalunaRouteCore
**RogalunaRouteCore** 是一个为Qt应用程序设计的轻量级路由管理库。它提供了类似Web开发中的路由功能，使得在Qt中管理多页面切换、参数传递和页面生命周期变得更加简单和高效。
## 功能特性
- **声明式路由注册**：通过宏简单注册路由，指定路径、页面组件和视图容器。
- **路由导航**：支持通过路径导航到指定页面，并可传递参数。
- **路由守卫**：在导航前、解析时、导航后三个阶段进行拦截和控制。
- **页面生命周期**：通过`IRoutable`接口实现页面进入、离开和参数接收的生命周期回调。
- **内存管理**：自动管理页面组件的生命周期，支持`keepAlive`选项保持页面状态。
- **嵌套路由**：支持多个路由视图容器，实现复杂布局。
## 核心组件
### QRouter
路由管理器，单例模式，负责路由的注册、导航和参数传递。
主要方法：
- `registerRoute(FRoute*)`: 注册路由。
- `navigateTo(const QString& path, const QVariantMap& params)`: 导航到指定路径。
- `sendParams(const QString& path, const QVariantMap& params)`: 发送参数到指定页面。
### QRouteView
路由视图容器，继承自`QStackedWidget`，用于显示当前路由对应的页面。每个视图容器必须指定一个唯一的名称。
### FRoute
路由项，包含路径、页面创建函数、目标视图名称和元数据（如`keepAlive`）。
### FRouteGuard
路由守卫，用于在导航过程中进行拦截。可以添加多个全局守卫。
### IRoutable
页面生命周期接口，包含：
- `enterRoute(const QVariantMap& params)`: 页面进入时调用。
- `leaveRoute()`: 页面离开时调用。
- `receiveParams(const QVariantMap& params)`: 接收参数时调用。
## 快速开始
### 安装
使用CMake构建并安装库：
```bash
mkdir build
cd build
cmake ..
cmake --build .
cmake --install .
```
### 基本用法
1. **创建路由视图容器**：
   在Qt窗口中放置一个`QRouteView`，并指定视图名称（如`mainView`）。
2. **注册路由**：
   在代码中注册路由，将路径映射到页面组件和视图容器。
   ```cpp
   // 注册一个常规路由
   REGISTER_NORMAL_ROUTE("/home", HomeWidget, "mainView");
   
   // 注册一个持久化路由（keepAlive）
   REGISTER_PERSISTENT_ROUTE("/settings", SettingsWidget, "mainView");
   ```
3. **导航**：
   使用`QRouter::instance()->navigateTo("/home")`进行导航。
4. **实现页面生命周期**：
   让页面组件继承`IRoutable`接口，并实现相关方法。
## 进阶用法
### 路由守卫
可以添加全局路由守卫，在导航过程中进行拦截。
```cpp
FRouteGuard* guard = new FRouteGuard();
guard->addGlobalGuard([](const QString& from, const QString& to) -> bool {
    // 禁止从首页跳转到设置页
    if (from == "/home" && to == "/settings") {
        return false;
    }
    return true;
});
QRouter::instance()->setRouteGuard(guard);
```
### 页面生命周期
在页面组件中实现`IRoutable`接口：
```cpp
class HomeWidget : public QWidget, public IRoutable {
public:
    void enterRoute(const QVariantMap& params) override {
        // 页面进入，初始化数据
    }
    void leaveRoute() override {
        // 页面离开，清理资源
    }
    void receiveParams(const QVariantMap& params) override {
        // 接收参数，更新页面
    }
};
```
### 嵌套路由
通过多个`QRouteView`可以实现嵌套路由。例如，主视图和侧边栏视图可以分别用不同的`QRouteView`管理。
注意：嵌套路由需要将控件设置为`keepAlive`，以避免在切换时被销毁。
## 构建与安装
项目使用CMake构建，依赖Qt5或Qt6的Core和Widgets模块。
CMake配置示例：
```cmake
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
target_link_libraries(your_target PRIVATE RogalunaRouteCore)
```
## 示例
1. 创建路由视图容器
``` cpp
// 路由视图容器，作为页面组件的承载控件，在父视图中创建
QRouteView* mainView = new QRouteView("mainView", centralWidget);
```

2. 注册路由
``` cpp
// 在全局初始化中注册路由
REGISTER_NORMAL_ROUTE("/login", LoginPage, "mainView");
REGISTER_PERSISTENT_ROUTE("/dashboard", DashboardPage, "mainView", {{"keepAlive", true}});
```
3. 实现页面生命周期
``` cpp
// DashboardPage.h
class DashboardPage : public QWidget, public IRoutable {
public:
    void enterRoute(const QVariantMap& params) override {
        // 页面进入时初始化
    }
    
    void receiveParams(const QVariantMap& params) override {
        // 接收参数
    }
    
    void leaveRoute() override {
        // 页面离开时清理资源
    }
};
```
4. 执行导航
``` cpp
// 导航到登录页
QRouter::instance()->navigateTo("/login");

// 带参数导航
QRouter::instance()->navigateTo("/profile", {{"userId", 123}});
```
## 贡献与许可
欢迎贡献代码。
---
