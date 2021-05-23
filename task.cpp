#include "task.h"
#include "ui_task.h"
#include "project.h"


Task::Task(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Task)
{
    ui->setupUi(this);
    ui->title->setText("");
    ui->task1->setVisible(false);
    ui->task2->setVisible(false);
    ui->task3->setVisible(false);
    ui->task4->setVisible(false);
    ui->task5->setVisible(false);
    ui->task6->setVisible(false);
}

Task::~Task()
{
    delete ui;
}

void Task::on_closeButton_clicked()
{
    close();
}

void Task::on_addline_returnPressed()
{
    if (!ui->task1->isVisible())
    {
        ui->task1->setVisible(true);
        ui->checkBox->setText(ui->addline->text());
        ui->addline->clear();
    } else if (!ui->task2->isVisible())
    {
        ui->task2->setVisible(true);
        ui->checkBox_2->setText(ui->addline->text());
        ui->addline->clear();
    } else if (!ui->task3->isVisible())
    {
        ui->task3->setVisible(true);
        ui->checkBox_3->setText(ui->addline->text());
        ui->addline->clear();
    } else if (!ui->task4->isVisible())
    {
        ui->task4->setVisible(true);
        ui->checkBox_4->setText(ui->addline->text());
        ui->addline->clear();
    } else if (!ui->task5->isVisible())
    {
        ui->task5->setVisible(true);
        ui->checkBox_5->setText(ui->addline->text());
        ui->addline->clear();
    } else if (!ui->task6->isVisible())
    {
        ui->task6->setVisible(true);
        ui->checkBox_6->setText(ui->addline->text());
        ui->addline->clear();
    }
}

void Task::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == 2) ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
    else if (arg1 == 0) ui->checkBox->setStyleSheet("");
}

void Task::on_checkBox_2_stateChanged(int arg1)
{
    if (arg1 == 2) ui->checkBox_2->setStyleSheet("font-style: italic; text-decoration: line-through;");
    else if (arg1 == 0) ui->checkBox_2->setStyleSheet("");
}

void Task::on_checkBox_3_stateChanged(int arg1)
{
    if (arg1 == 2) ui->checkBox_3->setStyleSheet("font-style: italic; text-decoration: line-through;");
    else if (arg1 == 0) ui->checkBox_3->setStyleSheet("");
}

void Task::on_checkBox_4_stateChanged(int arg1)
{
    if (arg1 == 2) ui->checkBox_4->setStyleSheet("font-style: italic; text-decoration: line-through;");
    else if (arg1 == 0) ui->checkBox_4->setStyleSheet("");
}

void Task::on_checkBox_5_stateChanged(int arg1)
{
    if (arg1 == 2) ui->checkBox_5->setStyleSheet("font-style: italic; text-decoration: line-through;");
    else if (arg1 == 0) ui->checkBox_5->setStyleSheet("");
}

void Task::on_checkBox_6_stateChanged(int arg1)
{
    if (arg1 == 2) ui->checkBox_6->setStyleSheet("font-style: italic; text-decoration: line-through;");
    else if (arg1 == 0) ui->checkBox_6->setStyleSheet("");
}
