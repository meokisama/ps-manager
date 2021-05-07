#include "widget.h"
#include "ui_widget.h"
#include "loginform.h"
#include "database.h"
#include "project.h"

#include <QSqlRecord>
#include <QMessageBox>

Database dtbase;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QSqlQuery fetcher;
    fetcher.prepare("SELECT * FROM USER WHERE USERNAME = (:un) AND PASSWORD = (:pw)");
    fetcher.bindValue(":un", LoginForm::getUsername());
    fetcher.bindValue(":pw", LoginForm::getPassword());
    fetcher.exec();

    int iLN = fetcher.record().indexOf("TEN");

    while(fetcher.next())
    {
        ui->showName->setText(fetcher.value(iLN).toString());
    }
    ui->addp->setVisible(false);
    ui->dateEdit->setDate(QDate::currentDate());
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
    ui->state->setText("");

    QSqlQuery fetcher;
    fetcher.prepare("SELECT * FROM USER WHERE USERNAME = (:un) AND PASSWORD = (:pw)");
    fetcher.bindValue(":un", LoginForm::getUsername());
    fetcher.bindValue(":pw", LoginForm::getPassword());
    fetcher.exec();

    int iUN = fetcher.record().indexOf("USERNAME");
    int iPW = fetcher.record().indexOf("PASSWORD");
    int iFN = fetcher.record().indexOf("HO");
    int iLN = fetcher.record().indexOf("TEN");
    int iEM = fetcher.record().indexOf("EMAIL");

    while(fetcher.next())
    {
        ui->fname->setText(fetcher.value(iFN).toString());
        ui->lname->setText(fetcher.value(iLN).toString());
        ui->un->setText(fetcher.value(iUN).toString());
        ui->pw->setText(fetcher.value(iPW).toString());
        ui->em->setText(fetcher.value(iEM).toString());
        QString full = fetcher.value(iFN).toString();
        ui->fullname->setText(full.append(" ").append(fetcher.value(iLN).toString()));
    }

}

void Widget::on_btnSave_clicked()
{
    QString fn = ui->fname->text();
    QString ln = ui->lname->text() ;
    QString un = ui->un->text();
    QString pw = ui->pw->text();
    QString em = ui->em->text();

    if(fn == "") ui->state->setText("Please enter your first name!");

    else if(ln == "") ui->state->setText("Please enter your last name!");

    else if(un == "") ui->state->setText("Please enter your username!");

    else if(pw == "") ui->state->setText("Please enter your password!");

    else if(em == "") ui->state->setText("Please enter your email!");

    else if(!em.contains("@",Qt::CaseInsensitive))
            ui->state->setText("Please enter the right format of email!");

    else if(dtbase.Update(fn, ln, un, pw, em))
    {
        ui->state->setText("Your information has updated successfully!");
        ui->fullname->setText(fn.append(" ").append(ln));
        ui->showName->setText(ln);
    } else ui->state->setText("Error!");
}

void Widget::on_addProject_clicked()
{
    ui->addp->setVisible(true);
}

void Widget::on_btnCreate_clicked()
{
    project *newp = new project();
    newp->setValue(ui->pname->text(),ui->dateEdit->date());
    h += 1;
    if(h > 3) {
        h = 1;
        v += 1;
        if(v > 2) {
            v = 1;
            QMessageBox::information(this, tr("  Rất tiếc"), tr("\nHiện tại, ứng dụng chỉ hỗ trợ chừng này projects.\t\n"), QMessageBox::Ok);
            delete newp;
            return;
        }
    }
    if(ui->pname->text() != "") {
        ui->gridLayout->addWidget(newp,v,h,Qt::Alignment());
        ui->addp->setVisible(false);
        ui->pname->clear();
    } else ui->state_2->setText("Enter project's name");
}

void Widget::on_btnCancel_clicked()
{
    ui->addp->setVisible(false);
}
