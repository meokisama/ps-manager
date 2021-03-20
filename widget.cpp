#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_closeButton_clicked()
{
    close();
}

void Widget::on_btnHome_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Widget::on_btnLiveChat_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::on_btnCalendar_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void Widget::on_btnVideo_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void Widget::on_btInfo_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void Widget::on_btnUser_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}
