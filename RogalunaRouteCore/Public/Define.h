#ifndef DEFINE_H
#define DEFINE_H

#include <QPointer>
#include <QWidget>
#include <RogalunaRouteCore_Global.h>

/**
 * @brief The FRouteObject class
 * - 路由对象，使用此类构建应用页面路径
 *
 * @note
 *
 */
class ROGALUNAROUTECORE_EXPORT FRouteObject : public QObject
{
    Q_OBJECT

    friend class QRouter;
public:
    using CreatorFunc = std::function<QWidget*()>;

    explicit FRouteObject(const QString& path,
                          CreatorFunc creator,
                          const QString& viewName = QString(),
                          const QVariantMap& meta = {},
                          const QVector<FRouteObject*>& children = {},
                          QObject* parent = nullptr);

public:
    QWidget* createWidget();

    FRouteObject& withMeta(const QVariantMap& meta);
    FRouteObject& withViewName(const QString& viewName);
    FRouteObject& withChildren(FRouteObject* route);

    QVector<FRouteObject*> getPathNodes() const;
    QString getAbsolutePath() const;

public: // 访问器
    const QString& relativePath() const { return m_path; }
    const QList<QString>& pathParams() const { return m_pathParams; }
    const QList<QString>& queryParams() const { return m_queryParams; }
    const QStringList& segments() const { return m_segments; }
    const QString& fragment() const { return m_fragment; }

    QPointer<QWidget> widgetInstance() const { return m_widgetInstance; }
    const QVariantMap& meta() const { return m_meta; }
    const QString& viewName() const { return m_viewName; }

private:
    void parsePath();

private:
    QString m_path;
    QList<QString> m_pathParams;
    QList<QString> m_queryParams;
    QStringList m_segments;
    QString m_fragment;

    CreatorFunc m_creator;
    QString m_viewName;
    QVariantMap m_meta;
    QPointer<QWidget> m_widgetInstance;
};

#endif // DEFINE_H
