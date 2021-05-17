#include "project.h"
#include "ui_project.h"

#include <QRandomGenerator>
#include <QDate>

project::project(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::project)
{
    ui->setupUi(this);
    Style();
}

project::~project()
{
    delete ui;
}

int project::getDate(QDate a)
{
    QDate dat;
    ui->date->setText(dat.currentDate().toString(Qt::RFC2822Date));
    return a.toJulianDay() - dat.currentDate().toJulianDay();
}

void project::Style()
{
    QString bgColor = color[QRandomGenerator::global()->bounded(color.size())];
    this->setStyleSheet("border-radius:30px;background-color:rgb(" + bgColor + ");");
    ui->dayleft->setStyleSheet("background: rgb(255, 255, 255);border-radius: 13px;color: rgb(252, 95, 89);");
}

void project::on_pushButton_clicked()
{
    close();
}

void project::setValue(QString a, QDate b)
{
    ui->label->setText(a);
    if(getDate(b) >= 0)
        ui->dayleft->setText(QString::number(getDate(b)) + " days left");
    else ui->dayleft->setText("Overdue");
}

void project::fetchValue(QString a, int b)
{
    ui->label->setText(a);
    if(b >= 0)
        ui->dayleft->setText(QString::number(b) + " days left");
    else ui->dayleft->setText("Overdue");
}
