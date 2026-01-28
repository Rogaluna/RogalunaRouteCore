#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "QGlobalVariable.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(QGlobalVariables::instance()->getRouter(), &QRouter::currentRoutePathChange, this, &MainWindow::on_route_path_changed);
    connect(QGlobalVariables::instance()->getRouter(), &QRouter::currentRouteObjectChange, this, &MainWindow::on_route_changed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_refresh_clicked()
{
    QGlobalVariables::instance()->getRouter()->refresh();
}

void MainWindow::on_btn_back_clicked()
{
    QGlobalVariables::instance()->getRouter()->prePage();
}


void MainWindow::on_btn_forward_clicked()
{
    QGlobalVariables::instance()->getRouter()->nextPage();
}

void MainWindow::on_btn_setting_clicked()
{
    QGlobalVariables::instance()->getRouter()->push("/setting", this);
}

void MainWindow::on_btn_notFound_clicked()
{
    QGlobalVariables::instance()->getRouter()->push("/notFound", this);
}

void MainWindow::on_route_path_changed(const QString &path)
{
    ui->le_routePath->setText(path);
}

void MainWindow::on_route_changed(FRouteObject *m_currentRouteObject)
{
    ui->le_routePath->setText(m_currentRouteObject->getAbsolutePath());
}

QRouteView *MainWindow::routeViews()
{
    return ui->container;
}

void MainWindow::on_le_routePath_returnPressed()
{
    QString targetPath = ui->le_routePath->text();

    QGlobalVariables::instance()->getRouter()->push(targetPath, this);
}

