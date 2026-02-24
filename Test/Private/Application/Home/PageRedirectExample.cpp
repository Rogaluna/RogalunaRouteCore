#include "Application/Home/PageRedirectExample.h"
#include "ui_PageRedirectExample.h"

PageRedirectExample::PageRedirectExample(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PageRedirectExample)
{
    ui->setupUi(this);
}

PageRedirectExample::~PageRedirectExample()
{
    delete ui;
}
