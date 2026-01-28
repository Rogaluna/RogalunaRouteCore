#include "PageHome.h"
#include "ui_PageHome.h"

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

void PageHome::on_btn_pageOne_clicked()
{
    QGlobalVariables::instance()->getRouter()->push("page1", this);
}

void PageHome::on_btn_pageTwo_clicked()
{
    QGlobalVariables::instance()->getRouter()->push("page2", this);
}

