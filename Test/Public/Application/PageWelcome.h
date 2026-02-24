#ifndef PAGEWELCOME_H
#define PAGEWELCOME_H

#include <QWidget>

#include <Interface/IRoutable.h>

namespace Ui {
class PageWelcome;
}

class PageWelcome : public QWidget, public IRoutable
{
    Q_OBJECT
    Q_INTERFACES(IRoutable)
public:
    explicit PageWelcome(QWidget *parent = nullptr);
    ~PageWelcome();

protected:
    virtual QRouteView* routeViews() override;

private:
    Ui::PageWelcome *ui;

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
};

#endif // PAGEWELCOME_H
