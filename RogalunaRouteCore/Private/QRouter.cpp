#include "QRouter.h"

#include <QRegularExpression>
#include <QTimer>

#include <View/QDefaultNotFoundView.h>

QRouter::QRouter(QObject *parent)
    : QObject{parent}
    , m_rootView(nullptr)
    , m_notFoundView(nullptr)
{
    m_notFoundView = new QDefaultNotFoundView;
}

void QRouter::install(const QVector<FRouteObject *> &routes, IRoutable* rootView)
{
    m_registeredRoutes = routes; // 保存原始树状结构
    m_flatRouteMap.clear();     // 清空之前的映射

    // 递归注册所有路由，并构建扁平化映射
    registerRoutesRecursively(routes, "");

    m_rootView = rootView;
}

void QRouter::setNotFoundView(QWidget *view)
{
    if (view == nullptr) return;
    m_notFoundView = view;
}

bool QRouter::navTo(const QString &path)
{
    if (path.isEmpty()) {
        qWarning() /* << "QRouter::push: Empty path provided."*/;
        return false;
    }

    // 1. 查找匹配的路由和参数
    auto [matchedRoute, pathParams] = findMatchingRoute(path);

    if (!matchedRoute) {
        qDebug() << "QRouter::push: No matching route found for path:" << path;
        // TODO: 可以在这里处理 404 Not Found 的情况，例如调用一个特殊的 404 路由

        // 当未找到对应路由时，将 NotFound 界面显示到根容器中
        QRouteView* rootContainer = m_rootView->routeViews();
        rootContainer->setWidget(m_notFoundView);

        return false;
    }

    // 2. 创建页面 Widget
    // 注意：createWidget 的具体行为取决于 FRouteObject 的实现。
    // 它可能需要传入 pathParams 等信息。
    // QWidget* newWidget = matchedRoute->createWidget();
    // if (!newWidget) {
    //     qDebug() << "QRouter::push: Failed to create widget for route:" << path;
    //     // TODO: 可以在这里处理创建失败的情况
    //     return false;
    // }

    // 3. 将新页面显示在容器中
    // 此处将 newWidget 添加到你的页面容器中。
    // 根据找到的路由一层一层向上溯源，得到一个页面创建列表
    // 根据这个创建列表，从根上创建页面（如果页面存在则跳过）
    qDebug() << "QRouter::push: Navigating to path:" << path;
    qDebug() << "QRouter::push: Matched route path:" << matchedRoute->routeDesc().m_path;
    // qDebug() << "QRouter::push: Created widget:" << newWidget;
    if (!pathParams.isEmpty()) {
        qDebug() << "QRouter::push: Extracted path parameters:" << pathParams;
    }

    QRouteView* currentContainer = m_rootView->routeViews();
    QVector<FRouteObject*> pathNodes = matchedRoute->getPathNodes();
    for (auto pathNode : pathNodes)
    {
        if (currentContainer == nullptr) break;

        QWidget* nodeWidget = pathNode->widgetInstance();
        // TODO
        // qobject_cast<IRoutable*>(nodeWidget) 判定是否实现了对应接口，如果没有，则应当在编译时报错。
        if (!nodeWidget) // 如果当前视图不存在，生成视图，设置视图容器并跳转下一个
        {
            currentContainer->setWidget(pathNode->createWidget());
        }

        nodeWidget = pathNode->widgetInstance();
        currentContainer = qobject_cast<IRoutable*>(nodeWidget)->routeViews();
    }


    // --- 容器处理逻辑结束 ---

    // 4. 更新路由器内部状态
    m_currentPath = path; // Store the path that was pushed

    // 5. 发射信号通知路由已更改
    // 我们需要一个 FRouteDesc 来传递给信号。
    // 可以创建一个新的，结合匹配的路由信息和当前路径信息。
    // 这里简化处理，直接使用输入 path 创建一个新的 Desc。
    // 更精确的做法可能是将匹配到的 routeDesc 与解析出的 params/query/fragment 合并。
    FRouteDesc currentDesc(path); // 假设 FRouteDesc 构造函数能正确解析 path
    emit routeChanged(currentDesc);

    return true;
}

bool QRouter::push(const QString &path)
{
    // 对当前路径进行分析，去掉查询参数（如果有）

    // 将传入的相对路径添加得到当前路径中
    m_currentPath.append(path);

    // 进行导航
    return navTo(m_currentPath);
}

void QRouter::registerRoutesRecursively(const QVector<FRouteObject *> &routes, const QString &parentPath)
{
    for (FRouteObject * route : routes) {
        const QString& routePath = route->routeDesc().m_path;
        // 计算完整路径
        QString fullPath;
        if (parentPath.isEmpty() || parentPath == "/") {
            if (routePath.startsWith("/")) {
                fullPath = routePath;
            } else {
                fullPath = "/" + routePath;
            }
        } else {
            if (routePath.startsWith("/")) {
                fullPath = parentPath + routePath; // e.g., "/parent" + "/child"
            } else {
                fullPath = parentPath + "/" + routePath; // e.g., "/parent" + "child"
            }
        }
        // 确保路径规范化（处理 // 或结尾的 /）
        // 这里简化处理，实际可能需要更健壮的路径规范化
        if (fullPath.endsWith("/") && fullPath != "/") {
            fullPath.chop(1); // 移除末尾的 '/'
        }

        // 注册到扁平化映射
        m_flatRouteMap[fullPath] = route;
        qDebug() << "Registered route:" << fullPath;

        // 递归注册子路由
        const QVector<FRouteObject*>& children = route->children();
        if (!children.isEmpty()) {
            registerRoutesRecursively(children, fullPath);
        }
    }
}

QPair<FRouteObject *, QMap<QString, QString> > QRouter::findMatchingRoute(const QString &fullPath) const
{
    // 1. 解析输入路径的基本信息 (主要是为了获取 basePath 用于匹配)
    // 假设 FRouteDesc 能正确解析路径
    FRouteDesc inputDesc(fullPath);
    const QString& targetBasePath = inputDesc.m_basePath; // We match against the base path

    // 2. 遍历扁平化映射进行匹配
    // 为了支持动态路由优先于静态路由（例如 /user/new 应该匹配静态路由而不是 /user/:id 的动态路由），
    // 最好按特定顺序检查，或者在注册时就处理好优先级。
    // 简单起见，我们先检查完全匹配，再检查动态匹配。
    // 更健壮的方法是排序路由（静态 > 动态）或构建前缀树。

    // --- 先尝试精确匹配静态路由 ---
    auto exactIt = m_flatRouteMap.constFind(targetBasePath);
    if (exactIt != m_flatRouteMap.constEnd()) {
        FRouteObject* exactRoute = exactIt.value();
        // 检查该路由的路径是否真的是静态的（不含 :）
        const QString& registeredPath = exactRoute->routeDesc().m_path;
        if (!registeredPath.contains(':')) { // Simple check for static route
            return qMakePair(exactRoute, QMap<QString, QString>());
        }
        // If it contains ':', it's actually a dynamic route that happened to match exactly,
        // we should let the dynamic logic handle it to extract params correctly.
        // Or, design registration such that static routes never contain ':'.
    }


    // --- 动态路由匹配 ---
    for (auto it = m_flatRouteMap.constBegin(); it != m_flatRouteMap.constEnd(); ++it) {
        const QString& registeredFullPathKey = it.key(); // Use the flattened key which is the full path
        FRouteObject* registeredRoute = it.value();

        // 获取原始注册路径用于检查是否为动态路由
        const QString& originalRoutePath = registeredRoute->routeDesc().m_path;
        static const QRegularExpression hasParamRegex(":([a-zA-Z_][a-zA-Z0-9_]*)");
        QRegularExpressionMatch isDynamicMatch = hasParamRegex.match(originalRoutePath);

        if (isDynamicMatch.hasMatch()) { // 只处理动态路由
            QString pattern = registeredFullPathKey; // 使用扁平化后的完整路径 key 作为模式基础
            QMap<QString, QString> extractedParams;

            // 提取所有参数名
            QStringList paramNames;
            QRegularExpressionMatchIterator iter = hasParamRegex.globalMatch(originalRoutePath); // 在原始路径上找参数名
            while (iter.hasNext()) {
                QRegularExpressionMatch match = iter.next();
                paramNames << match.captured(1); // 提取参数名 (group 1)
            }

            if (!paramNames.isEmpty()) { // 确保有参数需要匹配
                // 转义路径中的特殊字符，除了我们即将替换的 ':' 占位符部分
                QString escapedPattern = QRegularExpression::escape(pattern);
                // 恢复 ':' 字面量 (如果路径中真的有它且未被转义，这步可能不需要，
                // 但为了安全，先恢复，因为我们的占位符是 :name)
                // 实际上，: 在 escape 后变成 \: ，但我们希望保留 : 作为占位符标记。
                // 更准确的做法是直接替换原始路径中的占位符。
                // 让我们换个思路：直接基于 originalRoutePath 构建正则，然后应用到 registeredFullPathKey 上可能不对。
                // 因为 originalRoutePath 是相对的，registeredFullPathKey 是绝对的。
                // 正确的做法应该是基于 registeredFullPathKey 构建正则，并识别其中的 :param 部分。

                // 重新设计：基于 registeredFullPathKey 构建正则
                QString finalRegexPattern = QRegularExpression::escape(registeredFullPathKey);
                // 现在，我们需要在 escaped 的 registeredFullPathKey 中找到并替换 :param 部分。
                // 但是 escaped 后 : 变成了 \: 。所以我们需要在 escape 之前做替换。
                // 最清晰的方法：直接处理原始的、完整的路径字符串来构建正则。

                // 让我们用一种更清晰的方式来构建正则：
                // 1. 从 registeredFullPathKey 开始 (因为它已经是完整的绝对路径)
                // 2. 找到其中所有类似 :paramName 的部分
                // 3. 将这些部分替换为正则表达式捕获组
                // 4. 对整个字符串进行转义（除了捕获组）

                // 然而，registeredFullPathKey 是已经拼接好的绝对路径，其中的 :param 已经是具体的了。
                // 我们需要回到原始的父子关系来正确构建。

                // 最佳实践：在注册时，就应该为每个 *原始路径* 构建其对应的正则表达式和参数名列表。
                // 为了简化当前修改，我们就在匹配时现场构建。

                // 修正策略：
                // 1. 我们知道 registeredFullPathKey 是 "/parent/:id" 这样的完整路径。
                // 2. 我们需要把它变成正则 "/parent/([^/]+)"
                // 3. 同时记住参数名 ["id"]

                // 提取参数名 (已在上面完成)
                // 构建正则：先转义整个 registeredFullPathKey，然后替换 \:param 为 ([^/]+)
                // 注意：escape 会把 : 变成 \:
                QString tempPatternForRegex = QRegularExpression::escape(registeredFullPathKey);
                // 替换转义后的占位符
                for (const QString& paramName : paramNames) {
                    QString escapedPlaceholder = "\\:" + QRegularExpression::escape(paramName); // \:id
                    QString replacement = "([^/]+)"; // 捕获组
                    tempPatternForRegex.replace(QRegularExpression(escapedPlaceholder), replacement);
                }
                finalRegexPattern = "^" + tempPatternForRegex + "$"; // 锚定开始和结束以确保完全匹配


                QRegularExpression regex(finalRegexPattern);
                QRegularExpressionMatch pathMatch = regex.match(targetBasePath); // 在目标路径上匹配

                if (pathMatch.hasMatch()) {
                    // 提取捕获组到参数 map
                    for (int i = 0; i < paramNames.size(); ++i) {
                        QString value = pathMatch.captured(i + 1); // Group 0 is full match, groups start from 1
                        if (!value.isEmpty()) {
                            // TODO: Consider URL decoding if needed (QUrl::fromPercentEncoding)
                            extractedParams[paramNames[i]] = value;
                        }
                    }
                    // 找到第一个匹配的动态路由即返回
                    // （注意：实际路由库可能会有更复杂的优先级规则）
                    return qMakePair(registeredRoute, extractedParams);
                }
            }
        }
        // 如果 registeredRoute 是静态的，已经在前面的精确匹配阶段处理过了。
    }

    // 3. 未找到匹配
    return qMakePair<FRouteObject*, QMap<QString, QString>>(nullptr, QMap<QString, QString>());
}
