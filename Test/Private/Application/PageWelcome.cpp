#include "Application/PageWelcome.h"
#include "./ui_PageWelcome.h"

#include <QGlobalVariable.h>
#include <QRouter.h>

PageWelcome::PageWelcome(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PageWelcome)
{
    ui->setupUi(this);
}

PageWelcome::~PageWelcome()
{
    delete ui;
}

QRouteView *PageWelcome::routeViews()
{
    return nullptr;
}

void PageWelcome::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    QGlobalVariables::instance()->getRouter()->push("/home", nullptr);
}
