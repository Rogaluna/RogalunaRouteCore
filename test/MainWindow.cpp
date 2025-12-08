#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "QGlobalVariable.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_refresh_clicked()
{

}


void MainWindow::on_btn_back_clicked()
{

}


void MainWindow::on_btn_forward_clicked()
{

}

void MainWindow::on_btn_setting_clicked()
{
    QGlobalVariables::instance()->getRouter()->push("/setting");
}

void MainWindow::on_btn_notFound_clicked()
{
    QGlobalVariables::instance()->getRouter()->push("/notFound");
}

QRouteView *MainWindow::routeViews()
{
    return ui->container;
}




