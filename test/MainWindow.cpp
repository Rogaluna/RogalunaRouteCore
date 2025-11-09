#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include <QRouteView.h>
#include <Router.h>

#include <Page/HomePage.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_routeView(new QRouteView("MAIN_VIEW", this))
{
    ui->setupUi(this);

    ui->centralwidget->layout()->addWidget(m_routeView);

    REGISTER_PERSISTENT_ROUTE("/", HomePage, "MAIN_VIEW")

    REDIRECT_ROUTE("/")
}

MainWindow::~MainWindow()
{
    delete ui;
}
