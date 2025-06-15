#include "QRouteView.h"
#include "Router.h"

QRouteView::QRouteView(const QString& viewName, QWidget* parent) :
    QStackedWidget(parent),
    m_viewName(viewName)
{
    Router::instance()->registerViewContainer(this, m_viewName);
}

QRouteView::~QRouteView()
{
    Router::instance()->unregisterViewContainer(m_viewName);
}
