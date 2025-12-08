#ifndef PAGEHOME_H
#define PAGEHOME_H

#include <QWidget>

#include <Interface/IRoutable.h>

namespace Ui {
class PageHome;
}

class PageHome : public QWidget, public IRoutable
{
    Q_OBJECT
    Q_INTERFACES(IRoutable)
public:
    explicit PageHome(QWidget *parent = nullptr);
    ~PageHome();

protected:
    virtual QRouteView* routeViews() override;

private slots:
    void on_btn_pageOne_clicked();

private:
    Ui::PageHome *ui;
};

#endif // PAGEHOME_H
