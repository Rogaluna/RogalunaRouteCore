#ifndef PAGEONE_H
#define PAGEONE_H

#include <QWidget>

#include <Interface/IRoutable.h>

namespace Ui {
class PageOne;
}

class PageOne : public QWidget, public IRoutable
{
    Q_OBJECT
    Q_INTERFACES(IRoutable)
public:
    explicit PageOne(QWidget *parent = nullptr);
    ~PageOne();

protected:
    virtual QRouteView* routeViews() override { return nullptr; };

private:
    Ui::PageOne *ui;
};

#endif // PAGEONE_H
