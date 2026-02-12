#include "Application/PageHome.h"
#include "./ui_PageHome.h"

#include <QGlobalVariable.h>

PageHome::PageHome(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PageHome)
{
    ui->setupUi(this);
}

PageHome::~PageHome()
{
    delete ui;
}

QRouteView *PageHome::routeViews()
{
    return ui->container;
}
