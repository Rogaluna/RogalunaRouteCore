#include <QRouteView.h>
#include <QGridLayout>

QRouteView::QRouteView(QWidget* parent) :
    QWidget(parent)
{

}

QRouteView::~QRouteView()
{

}

void QRouteView::setWidget(QWidget *view)
{
    // 删除旧的
    // if (m_widget) m_widget->deleteLater();
    delete m_widget;
    m_widget = nullptr;

    if (!view) {
        return;
    }

    view->setParent(this);
    view->setGeometry(rect());
    view->show();

    m_widget = view;
}

void QRouteView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (m_widget) {
        m_widget->setGeometry(rect()); // 铺满
    }
}
