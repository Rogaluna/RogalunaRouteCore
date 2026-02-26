#include <QRouteView.h>
#include <QGridLayout>

#include <Interface/IRoutable.h>

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
    unsetWidget();

    view->setParent(this);
    view->setGeometry(rect());
    view->show();

    m_widget = view;

    // 触发挂载函数
    IRoutable* routableView = qobject_cast<IRoutable*>(m_widget);
    if (routableView != nullptr)
    {
        routableView->mounted();
    }
}

QWidget *QRouteView::getWidget()
{
    return m_widget;
}

void QRouteView::unsetWidget()
{
    if (m_widget) {

        // 触发解挂载函数
        IRoutable* routableView = qobject_cast<IRoutable*>(m_widget);
        if (routableView != nullptr)
        {
            // 递归解除其内部视图的挂载
            QRouteView* routeView = routableView->routeViews();
            if (routeView != nullptr)
            {
                routeView->unsetWidget();
            }

            routableView->unmounted();
        }

        m_widget->setParent(nullptr);
        m_widget = nullptr;
    }
}

void QRouteView::clearWidget()
{
    if (m_widget) {

        if (qobject_cast<IRoutable*>(m_widget) != nullptr)
        {
            qobject_cast<IRoutable*>(m_widget)->unmounted();
        }

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
