#include "PageSetting.h"
#include "ui_PageSetting.h"

PageSetting::PageSetting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PageSetting)
{
    ui->setupUi(this);
}

PageSetting::~PageSetting()
{
    delete ui;
}
