#ifndef QDEFAULTNOTFOUNDVIEW_H
#define QDEFAULTNOTFOUNDVIEW_H

#include <QWidget>
#include <Interface/IRoutable.h>

class QDefaultNotFoundView : public QWidget, public IRoutable
{
    Q_OBJECT
public:
    explicit QDefaultNotFoundView(QWidget *parent = nullptr);
    ~QDefaultNotFoundView() = default;

protected:
    virtual QRouteView* routeViews() { return nullptr; };

signals:
};

#endif // QDEFAULTNOTFOUNDVIEW_H
