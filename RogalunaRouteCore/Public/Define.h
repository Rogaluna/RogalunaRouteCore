#ifndef DEFINE_H
#define DEFINE_H

#include <QPointer>
#include <QWidget>
#include <RogalunaRouteCore_Global.h>

/**
 * @brief The FRouteDesc class
 * 路由描述，将一个字符串形式的路径转为结构
 */
struct ROGALUNAROUTECORE_EXPORT FRouteDesc
{

    QString m_path;                         // 路由路径（原始路径）
    QString m_basePath;                     // 基础路径（不包含参数）
    QList<QString> m_pathParams;            // 路径参数 (例如, 从 /user/:id 中提取到的占位符参数 :id)
    QList<QString> m_queryParams;           // 查询参数 (例如, 从 ?key1=value1&key2=value2 中提取到的查询参数 key1、key2)
    QStringList m_segments;                 // 路径段（存储URL路径被"/"分割后的各个部分）
    QString m_fragment;                     // 片段标识符（存储URL中"#"后面的部分）

    FRouteDesc() = default;
    explicit FRouteDesc(const QString& _path);

private:
    void parsePath();
};

/**
 * @brief The FRouteObject class
 * 路由对象，使用此类构建应用页面路径
 *
 * @note
 *
 */
class ROGALUNAROUTECORE_EXPORT FRouteObject
{
public:
    using CreatorFunc = std::function<QWidget*()>;

    // 主构造函数
    explicit FRouteObject(const QString& path,
                    CreatorFunc creator,
                    const QString& viewName = QString(),
                    const QVariantMap& meta = {},
                    const QVector<FRouteObject*>& children = {});
public: // 方法

    QWidget* createWidget();

    FRouteObject& withMeta(const QVariantMap& meta);
    FRouteObject& withViewName(const QString& viewName);
    FRouteObject& withChildren(FRouteObject* route);

    // 获取当前节点到根的节点列表
    QVector<FRouteObject*> getPathNodes() const;

public: // 访问器

    const FRouteDesc& routeDesc() const { return m_routeDesc; }

    QPointer<QWidget> widgetInstance() const { return m_widgetInstance; }

    // 元数据
    const QVariantMap& meta() const { return m_meta; }

    // 目标视图名称
    const QString& viewName() const { return m_viewName; }

    // 子路由
    const QVector<FRouteObject*>& children() const { return m_children; }

    // 父路由
    const FRouteObject* parent() const { return m_parent; };

private:
    void setParent(FRouteObject* p) { m_parent = p; };

private:
    FRouteDesc m_routeDesc;                 // 路由描述

    CreatorFunc m_creator;                  // 组件创建函数
    QString m_viewName;                     // 目标视图名称
    QVariantMap m_meta;                     // 添加元数据存储

    QPointer<QWidget> m_widgetInstance;     // 创建的控件实例

    QVector<FRouteObject*> m_children;      // 子路由
    FRouteObject* m_parent;                 // 父路由
};

#endif // DEFINE_H
