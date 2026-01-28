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
    if (view == nullptr) {
        return;
    }

    // 解除旧 widget 与本容器的父子关系
    if (m_widget) {
        m_widget->setParent(nullptr);
    }

    view->setParent(this);
    view->setGeometry(rect());
    view->show();

    m_widget = view;
}

void QRouteView::unsetWidget()
{
    if (m_widget) {
        m_widget->setParent(nullptr);
        m_widget = nullptr;
    }
}

void QRouteView::clearWidget()
{
    if (m_widget) {
        m_widget->setParent(nullptr);
        delete m_widget;
        m_widget = nullptr;
    }
}

void QRouteView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (m_widget) {
        m_widget->setGeometry(rect()); // 铺满
    }
}
