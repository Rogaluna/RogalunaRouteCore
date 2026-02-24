#include "Application/PageHome.h"
#include "./ui_PageHome.h"

#include <QGlobalVariable.h>

#include <QDebug>

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

void PageHome::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    qDebug() << "current Item: " << current->data(0, Qt::DisplayRole);
}

