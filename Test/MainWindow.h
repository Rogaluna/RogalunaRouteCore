#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRouter.h>
#include <Interface/IRoutable.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public IRoutable
{
    Q_OBJECT
    Q_INTERFACES(IRoutable)

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_refresh_clicked();

    void on_btn_back_clicked();

    void on_btn_forward_clicked();

    void on_btn_setting_clicked();

    void on_btn_notFound_clicked();

protected:
    virtual QRouteView* routeViews() override;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
