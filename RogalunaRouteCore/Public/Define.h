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

    QPointer<QWidget> widgetInstance() const { return m_widgetInstance; }
    const QVariantMap& meta() const { return m_meta; }
    const QString& viewName() const { return m_viewName; }

private:
    QString m_path;

    CreatorFunc m_creator;
    QString m_viewName;
    QVariantMap m_meta;
    QPointer<QWidget> m_widgetInstance;
};

#endif // DEFINE_H
