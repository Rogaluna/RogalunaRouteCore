#include "PageOne.h"
#include "ui_PageOne.h"

PageOne::PageOne(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PageOne)
{
    ui->setupUi(this);
}

PageOne::~PageOne()
{
    delete ui;
}
