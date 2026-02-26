#ifndef PAGEMANUAL_H
#define PAGEMANUAL_H

#include <QWidget>

#include <Interface/IRoutable.h>

namespace Ui {
class PageManual;
}

class PageManual : public QWidget, public IRoutable
{
    Q_OBJECT

public:
    explicit PageManual(QWidget *parent = nullptr);
    ~PageManual();

public:
    virtual void mounted() override;
    virtual void unmounted() override;

protected:
    virtual QRouteView* routeViews() override { return nullptr; };

private:
    Ui::PageManual *ui;
};

#endif // PAGEMANUAL_H
