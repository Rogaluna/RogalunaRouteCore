#ifndef PAGESETTING_H
#define PAGESETTING_H

#include <QWidget>

#include <Interface/IRoutable.h>

namespace Ui {
class PageSetting;
}

class PageSetting : public QWidget, public IRoutable
{
    Q_OBJECT
    Q_INTERFACES(IRoutable)
public:
    explicit PageSetting(QWidget *parent = nullptr);
    ~PageSetting();

protected:
    virtual QRouteView* routeViews() override { return nullptr; };

private:
    Ui::PageSetting *ui;
};

#endif // PAGESETTING_H
