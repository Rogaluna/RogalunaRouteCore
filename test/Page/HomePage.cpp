#include "HomePage.h"
#include "ui_HomePage.h"

HomePage::HomePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HomePage)
{
    ui->setupUi(this);
}

HomePage::~HomePage()
{
    delete ui;
}

void HomePage::enterRoute(const QVariantMap &params)
{

}

void HomePage::receiveParams(const QVariantMap &params)
{

}

void HomePage::leaveRoute()
{

}
