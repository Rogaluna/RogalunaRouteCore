#ifndef QROUTEVIEW_H
#define QROUTEVIEW_H

#include <RogalunaRouteCore_Global.h>
#include <QStackedWidget>

/**
 * @brief The QRouteView class
 * 路由视图控件
 * 必须指定视图的名称，否则无法进行导航
 * 视图名称在应用内是唯一的，当视图被注册的时候，名称将作为索引指引页面切换
 *
 * 注意，在使用嵌套路由视图的时候，一定要将控件设置为 keepAlive
 */
class ROGALUNAROUTECORE_EXPORT QRouteView : public QStackedWidget
{
    Q_OBJECT
public:
    explicit QRouteView(const QString& viewName, QWidget* parent = nullptr);
    ~QRouteView();

    const QString& viewName() const { return m_viewName; }

private:
    QString m_viewName;
};

#endif // QROUTEVIEW_H
