#include "project.h"
#include "ui_project.h"

#include <QRandomGenerator>
#include <QDate>

project::project(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::project)
{
    ui->setupUi(this);
    QString bgColor = color[QRandomGenerator::global()->bounded(color.size())];
    setStyleSheet("border-radius:30px;background-color:rgb(" + bgColor + ");");
    getDate();
}

project::~project()
{
    delete ui;
}

void project::getDate()
{
    QDate dat;
    ui->date->setText(dat.currentDate().toString(Qt::RFC2822Date));
}
