#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <Interface/IRoutable.h>

namespace Ui {
class HomePage;
}

class HomePage : public QWidget, public IRoutable
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();

public:
    void enterRoute(const QVariantMap& params) override;
    void receiveParams(const QVariantMap& params) override;
    void leaveRoute() override;


private:
    Ui::HomePage *ui;
};

#endif // HOMEPAGE_H
