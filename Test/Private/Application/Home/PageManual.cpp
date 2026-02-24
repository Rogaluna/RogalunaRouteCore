#include "Application/Home/PageManual.h"
#include "ui_PageManual.h"

PageManual::PageManual(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PageManual)
{
    ui->setupUi(this);
}

PageManual::~PageManual()
{
    delete ui;
}
