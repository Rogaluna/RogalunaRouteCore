#include "View/QDefaultNotFoundView.h"

#include <QHBoxLayout>
#include <QLabel>

QDefaultNotFoundView::QDefaultNotFoundView(QWidget *parent)
    : QWidget{parent}
{
    QLabel* titleLabel = new QLabel("NOT FOUND");
    QHBoxLayout* layout = new QHBoxLayout;

    titleLabel->setStyleSheet("color: red; font-size: 48px;");

    layout->addWidget(titleLabel);
    setLayout(layout);
}
