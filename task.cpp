#include "task.h"
#include "ui_task.h"
#include "project.h"
#include "database.h"

#include <QSqlRecord>

Database fetchValue;

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

Task::Task(QString id): ui(new Ui::Task)
{
    this->id = id;
    ui->setupUi(this);
    ui->title->setText(fetchValue.fetchName(id));
    ui->task1->setVisible(false);
    ui->task2->setVisible(false);
    ui->task3->setVisible(false);
    ui->task4->setVisible(false);
    ui->task5->setVisible(false);
    ui->task6->setVisible(false);

    QSqlQuery fetcher;
    fetcher.prepare("SELECT * FROM tasks WHERE id = (:id)");
    fetcher.bindValue(":id", id);
    fetcher.exec();

    int iN = fetcher.record().indexOf("name");
    int iS = fetcher.record().indexOf("state");

    while(fetcher.next())
    {
        if (!ui->task1->isVisible())
        {
            ui->task1->setVisible(true);
            ui->checkBox->setText(fetcher.value(iN).toString());
            if (fetcher.value(iS).toString() == "D") ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
            ui->checkBox->setCheckState(Qt::Checked);
        } else if (!ui->task2->isVisible())
        {
            ui->task2->setVisible(true);
            ui->checkBox_2->setText(fetcher.value(iN).toString());
            if (fetcher.value(iS).toString() == "D") ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
            ui->checkBox_2->setCheckState(Qt::Checked);
        } else if (!ui->task3->isVisible())
        {
            ui->task3->setVisible(true);
            ui->checkBox_3->setText(fetcher.value(iN).toString());
            if (fetcher.value(iS).toString() == "D") ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
            ui->checkBox_3->setCheckState(Qt::Checked);
        } else if (!ui->task4->isVisible())
        {
            ui->task4->setVisible(true);
            ui->checkBox_4->setText(fetcher.value(iN).toString());
            if (fetcher.value(iS).toString() == "D") ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
            ui->checkBox_4->setCheckState(Qt::Checked);
        } else if (!ui->task5->isVisible())
        {
            ui->task5->setVisible(true);
            ui->checkBox_5->setText(fetcher.value(iN).toString());
            if (fetcher.value(iS).toString() == "D") ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
            ui->checkBox_5->setCheckState(Qt::Checked);
        } else if (!ui->task6->isVisible())
        {
            ui->task6->setVisible(true);
            ui->checkBox_6->setText(fetcher.value(iN).toString());
            if (fetcher.value(iS).toString() == "D") ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
            ui->checkBox_6->setCheckState(Qt::Checked);
        }
    }
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
        QString state = "N";
        ui->task1->setVisible(true);
        ui->checkBox->setText(ui->addline->text());
        fetchValue.addTasks(this->id, ui->addline->text(),state);
        ui->addline->clear();
    } else if (!ui->task2->isVisible())
    {
        QString state = "N";
        ui->task2->setVisible(true);
        ui->checkBox_2->setText(ui->addline->text());
        fetchValue.addTasks(this->id, ui->addline->text(),state);
        ui->addline->clear();
    } else if (!ui->task3->isVisible())
    {
        QString state = "N";
        ui->task3->setVisible(true);
        ui->checkBox_3->setText(ui->addline->text());
        fetchValue.addTasks(this->id, ui->addline->text(),state);
        ui->addline->clear();
    } else if (!ui->task4->isVisible())
    {
        QString state = "N";
        ui->task4->setVisible(true);
        ui->checkBox_4->setText(ui->addline->text());
        fetchValue.addTasks(this->id, ui->addline->text(),state);
        ui->addline->clear();
    } else if (!ui->task5->isVisible())
    {
        QString state = "N";
        ui->task5->setVisible(true);
        ui->checkBox_5->setText(ui->addline->text());
        fetchValue.addTasks(this->id, ui->addline->text(),state);
        ui->addline->clear();
    } else if (!ui->task6->isVisible())
    {
        QString state = "N";
        ui->task6->setVisible(true);
        ui->checkBox_6->setText(ui->addline->text());
        fetchValue.addTasks(this->id, ui->addline->text(),state);
        ui->addline->clear();
    }
}

void Task::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == 2) {ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
        QString state = "D";
        fetchValue.updateTasks(this->id,ui->checkBox->text(),state);
    }
    else if (arg1 == 0) {ui->checkBox->setStyleSheet("");
        QString state = "N";
        fetchValue.updateTasks(this->id,ui->checkBox->text(),state);
    }
}

void Task::on_checkBox_2_stateChanged(int arg1)
{
    if (arg1 == 2) {ui->checkBox_2->setStyleSheet("font-style: italic; text-decoration: line-through;");
        QString state = "D";
        fetchValue.updateTasks(this->id,ui->checkBox_2->text(),state);
    }
    else if (arg1 == 0) {ui->checkBox_2->setStyleSheet("");
        QString state = "N";
        fetchValue.updateTasks(this->id,ui->checkBox->text(),state);
    }
}

void Task::on_checkBox_3_stateChanged(int arg1)
{
    if (arg1 == 2) {ui->checkBox_3->setStyleSheet("font-style: italic; text-decoration: line-through;");
        QString state = "D";
        fetchValue.updateTasks(this->id,ui->checkBox_3->text(),state);
    }
    else {if (arg1 == 0) ui->checkBox_3->setStyleSheet("");
        QString state = "N";
        fetchValue.updateTasks(this->id,ui->checkBox->text(),state);
    }
}

void Task::on_checkBox_4_stateChanged(int arg1)
{
    if (arg1 == 2) {ui->checkBox_4->setStyleSheet("font-style: italic; text-decoration: line-through;");
        QString state = "D";
        fetchValue.updateTasks(this->id,ui->checkBox_4->text(),state);
    }
    else if (arg1 == 0) {ui->checkBox_4->setStyleSheet("");
        QString state = "N";
        fetchValue.updateTasks(this->id,ui->checkBox->text(),state);
    }
}

void Task::on_checkBox_5_stateChanged(int arg1)
{
    if (arg1 == 2) {ui->checkBox_5->setStyleSheet("font-style: italic; text-decoration: line-through;");
        QString state = "D";
        fetchValue.updateTasks(this->id,ui->checkBox_5->text(),state);
    }
    else if (arg1 == 0) {ui->checkBox_5->setStyleSheet("");
        QString state = "N";
        fetchValue.updateTasks(this->id,ui->checkBox->text(),state);
    }
}

void Task::on_checkBox_6_stateChanged(int arg1)
{
    if (arg1 == 2) {ui->checkBox_6->setStyleSheet("font-style: italic; text-decoration: line-through;");
        QString state = "D";
        fetchValue.updateTasks(this->id,ui->checkBox_6->text(),state);
    }
    else if (arg1 == 0) {ui->checkBox_6->setStyleSheet("");
        QString state = "N";
        fetchValue.updateTasks(this->id,ui->checkBox->text(),state);
    }
}

void Task::on_pushButton_clicked()
{
    ui->task1->setVisible(false);
    fetchValue.deleteTasks(this->id,ui->checkBox->text());
    ui->checkBox->setStyleSheet("");
    ui->checkBox->setCheckState(Qt::Unchecked);
}

void Task::on_pushButton_2_clicked()
{
    ui->task2->setVisible(false);
    fetchValue.deleteTasks(this->id,ui->checkBox_2->text());
    ui->checkBox_2->setStyleSheet("");
    ui->checkBox_2->setCheckState(Qt::Unchecked);
}

void Task::on_pushButton_3_clicked()
{
    ui->task3->setVisible(false);
    fetchValue.deleteTasks(this->id,ui->checkBox_3->text());
    ui->checkBox_3->setStyleSheet("");
    ui->checkBox_3->setCheckState(Qt::Unchecked);
}

void Task::on_pushButton_4_clicked()
{
    ui->task4->setVisible(false);
    fetchValue.deleteTasks(this->id,ui->checkBox_4->text());
    ui->checkBox_4->setStyleSheet("");
    ui->checkBox_4->setCheckState(Qt::Unchecked);
}

void Task::on_pushButton_5_clicked()
{
    ui->task5->setVisible(false);
    fetchValue.deleteTasks(this->id,ui->checkBox_5->text());
    ui->checkBox_5->setStyleSheet("");
    ui->checkBox_5->setCheckState(Qt::Unchecked);
}

void Task::on_pushButton_6_clicked()
{
    ui->task6->setVisible(false);
    fetchValue.deleteTasks(this->id,ui->checkBox_6->text());
    ui->checkBox_6->setStyleSheet("");
    ui->checkBox_6->setCheckState(Qt::Unchecked);
}
