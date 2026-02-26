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

void PageManual::mounted()
{
    for (auto it = m_routeParams.begin(); it != m_routeParams.end(); ++it)
    {
        auto key = it.key();
        auto value = it.value();

        qDebug() << "route params: " << key << ", " << value;
    }
}

void PageManual::unmounted()
{

}
