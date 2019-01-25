#include "helloworldwidget.h"
#include "ui_helloworldwidget.h"

HelloWorldWidget::HelloWorldWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelloWorldWidget)
{
    ui->setupUi(this);
    setWindowTitle("HelloWorld");
}

HelloWorldWidget::~HelloWorldWidget()
{
    delete ui;
}
