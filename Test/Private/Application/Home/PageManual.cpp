#include "Application/Home/PageManual.h"
#include "ui_PageManual.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

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

        QString resourcePath = QString(":/Resources/%1.html").arg(value.toString());
        QFile file(resourcePath);

        if (file.exists()) {
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                QString htmlContent = in.readAll();
                file.close();

                ui->textEdit->setHtml(htmlContent);
                break;
            }
            else {
                qWarning() << "Failed to open resource file:" << resourcePath;
            }
        }
        else {
            qWarning() << "Resource file not found:" << resourcePath;
        }
    }
}

void PageManual::unmounted()
{

}
