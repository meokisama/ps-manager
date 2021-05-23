#include "project.h"
#include "ui_project.h"
#include "database.h"

#include <QRandomGenerator>
#include <QDate>
#include <QDateTime>
#include <QGraphicsDropShadowEffect>

Database dbase;

project::project(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::project)
{
    ui->setupUi(this);
    Style();
    QDateTime setID;
    this->id = QString::number(setID.currentSecsSinceEpoch());
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
    dbase.delProject(this->id);
}

void project::setValue(QString a, QDate b)
{
    ui->label->setText(a);
    if(getDate(b) >= 0)
        ui->dayleft->setText(QString::number(getDate(b)) + " days left");
    else ui->dayleft->setText("Overdue");
}

void project::fetchValue(int d, QString a, int b)
{
    ui->label->setText(a);
    if(b >= 0)
        ui->dayleft->setText(QString::number(b) + " days left");
    else ui->dayleft->setText("Overdue");
    QDate date = QDate::fromJulianDay(d);
    ui->date->setText(date.toString(Qt::RFC2822Date));
}

void project::on_btnEdit_clicked()
{
    task = new Task();
    task->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    //Make widget transparent
    task->setAttribute(Qt::WA_TranslucentBackground);

    // Add shadow effect
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setXOffset(2);
    effect->setYOffset(2);
    effect->setBlurRadius(10);
    effect->setColor(QColor(180, 180, 180, 180));
    task->setGraphicsEffect(effect);

    task->show();
}
