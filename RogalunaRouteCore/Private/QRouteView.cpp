#include <QRouteView.h>
#include <Router.h>

QRouteView::QRouteView(const QString& viewName, QWidget* parent) :
    QStackedWidget(parent),
    m_viewName(viewName)
{
    QRouter::instance()->registerViewContainer(this, m_viewName);
}

QRouteView::~QRouteView()
{
    QRouter::instance()->unregisterViewContainer(m_viewName);
}
