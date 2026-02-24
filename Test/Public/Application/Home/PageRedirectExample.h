#ifndef PAGEREDIRECTEXAMPLE_H
#define PAGEREDIRECTEXAMPLE_H

#include <QWidget>

#include <Interface/IRoutable.h>

namespace Ui {
class PageRedirectExample;
}

class PageRedirectExample : public QWidget, public IRoutable
{
    Q_OBJECT
    Q_INTERFACES(IRoutable)

public:
    explicit PageRedirectExample(QWidget *parent = nullptr);
    ~PageRedirectExample();

protected:
    virtual QRouteView* routeViews() override { return nullptr; };

private:
    Ui::PageRedirectExample *ui;
};

#endif // PAGEREDIRECTEXAMPLE_H
