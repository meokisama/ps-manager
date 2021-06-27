#include "widget.h"
#include "ui_widget.h"
#include "loginform.h"
#include "database.h"
#include "project.h"

#include <QtWidgets>
#include <QSqlRecord>
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>

Database dtbase;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    fetchProject();
    v = ui->gridLayout->rowCount();
    h = ui->gridLayout->columnCount();
    QSqlQuery fetcher;
    fetcher.prepare("SELECT * FROM USER WHERE USERNAME = (:un)");
    fetcher.bindValue(":un", LoginForm::getUsername());
    fetcher.exec();

    int iLN = fetcher.record().indexOf("TEN");

    while(fetcher.next())
    {
        ui->showName->setText(fetcher.value(iLN).toString());
    }
    ui->addp->setVisible(false);
    ui->dateEdit->setDate(QDate::currentDate());

    ui->inputMes->setFocusPolicy(Qt::StrongFocus);
    ui->textEdit->setFocusPolicy(Qt::NoFocus);
    ui->textEdit->setReadOnly(true);
    ui->listWidget->setFocusPolicy(Qt::NoFocus);

    connect(ui->inputMes, &QLineEdit::returnPressed,
            this, &Widget::returnPressed);
    connect(&client, &Client::newMessage,
            this, &Widget::appendMessage);
    connect(&client, &Client::newParticipant,
            this, &Widget::newParticipant);
    connect(&client, &Client::participantLeft,
            this, &Widget::participantLeft);

    myNickName = client.nickName();
    newParticipant(myNickName);
    tableFormat.setBorder(0);
    QTimer::singleShot(10 * 1000, this, SLOT(showInformation()));

    ui->your1_0->setVisible(false);
    ui->your1_1->setVisible(false);
    ui->your1_2->setVisible(false);
    ui->your1_3->setVisible(false);
    ui->your1_4->setVisible(false);
    ui->your1_5->setVisible(false);
    ui->your1_6->setVisible(false);
    ui->your1_7->setVisible(false);
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
    resetProject();
    fetchProject();
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
    fetcher.prepare("SELECT * FROM USER WHERE USERNAME = (:un)");
    fetcher.bindValue(":un", LoginForm::getUsername());
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

    fetchTasks();
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
    resetProject();
    fetchProject();
    if (ui->gridLayout->count() == 0) {v=1; h=0;}
    else if (ui->gridLayout->count() == 1){v=1; h=1;}
    else if (ui->gridLayout->count() == 2){v=1; h=2;}
    else if (ui->gridLayout->count() == 3){v=1; h=3;}
    else if (ui->gridLayout->count() == 4){v=2; h=1;}
    else if (ui->gridLayout->count() == 5){v=2; h=2;}
    else if (ui->gridLayout->count() == 6){v=2; h=3;}

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
        QDate a;
        dtbase.addProject(newp->id, ui->pname->text(), a.currentDate().toJulianDay(), ui->dateEdit->date().toJulianDay());
        ui->gridLayout->addWidget(newp,v,h,Qt::Alignment());
        ui->addp->setVisible(false);
        ui->pname->clear();
    } else ui->state_2->setText("Enter project's name");

}

void Widget::on_btnCancel_clicked()
{
    ui->addp->setVisible(false);
}

void Widget::fetchProject()
{
    QSqlQuery fetcher;
    fetcher.prepare("SELECT * FROM projects");

    fetcher.exec();

    int iD = fetcher.record().indexOf("id");
    int iN = fetcher.record().indexOf("name");
    int iS = fetcher.record().indexOf("startdate");
    int iF = fetcher.record().indexOf("findate");

    int v1 = 1, h1 = 0;

    while(fetcher.next())
    {
        project *newp = new project();
        newp->id = fetcher.value(iD).toString();
        newp->fetchValue(fetcher.value(iS).toInt(), fetcher.value(iN).toString(), fetcher.value(iF).toInt() - fetcher.value(iS).toInt());
        h1 += 1;
        if(h1 > 3) {
            h1 = 1;
            v1 += 1;
            if(v1 > 2) {
                v1 = 1;
            }
        }
        ui->gridLayout->addWidget(newp,v1, h1,Qt::Alignment());
    }
}

void Widget::resetProject()
{
    while (ui->gridLayout->count())
    {
        QLayoutItem* item = ui->gridLayout->itemAt(0);
        ui->gridLayout->removeItem(item);

        QWidget* widget = item->widget();
        if(widget) delete widget;
    }
}

void Widget::on_pushButton_2_clicked()
{
    QFileDialog::getOpenFileName(this, "Choose file", QDir::currentPath(), "All files (*.*) ;; Document (*.doc *.rtf);; Image (*.png *.jpg)");
}

void Widget::on_pushButton_3_clicked()
{
    //QDesktopServices::openUrl(QUrl("https://meoki.herokuapp.com/join/Meoki"));
    QDesktopServices::openUrl(QUrl("http://localhost:3000"));
}

void Widget::appendMessage(const QString &from, const QString &message)
{
    if (from.isEmpty() || message.isEmpty())
        return;

    QTextCursor cursor(ui->textEdit->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable *table = cursor.insertTable(1, 2, tableFormat);
    table->cellAt(0, 0).firstCursorPosition().insertText('<' + from + "> ");
    table->cellAt(0, 1).firstCursorPosition().insertText(message);
    QScrollBar *bar = ui->textEdit->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void Widget::returnPressed()
{
    QString text = ui->inputMes->text();
    if (text.isEmpty())
        return;

    if (text.startsWith(QChar('/'))) {
        QColor color = ui->textEdit->textColor();
        ui->textEdit->setTextColor(Qt::red);
        ui->textEdit->append(tr("! Unknown command: %1")
                         .arg(text.left(text.indexOf(' '))));
        ui->textEdit->setTextColor(color);
    } else {
        client.sendMessage(text);
        appendMessage(myNickName, text);
    }

    ui->inputMes->clear();
}

void Widget::newParticipant(const QString &nick)
{
    if (nick.isEmpty())
        return;

    QColor color = ui->textEdit->textColor();
    ui->textEdit->setTextColor(Qt::gray);
    ui->textEdit->append(tr("* %1 has joined").arg(nick));
    ui->textEdit->setTextColor(color);
    ui->listWidget->addItem(nick);
}

void Widget::participantLeft(const QString &nick)
{
    if (nick.isEmpty())
        return;

    QList<QListWidgetItem *> items = ui->listWidget->findItems(nick,
                                                           Qt::MatchExactly);
    if (items.isEmpty())
        return;

    delete items.at(0);
    QColor color = ui->textEdit->textColor();
    ui->textEdit->setTextColor(Qt::gray);
    ui->textEdit->append(tr("* %1 has left").arg(nick));
    ui->textEdit->setTextColor(color);
}

void Widget::showInformation()
{
    if (ui->listWidget->count() == 1) {
        //QMessageBox::information(this, tr("Chat"), tr("Launch several instances of this program on your local network and start chatting!"));
    }
}

void Widget::fetchTasks()
{
    QSqlQuery fetcher;
    fetcher.prepare("SELECT * FROM tasks WHERE recip = (:rc)");
    fetcher.bindValue(":rc", ui->lname->text());
    fetcher.exec();

    int iN = fetcher.record().indexOf("name");
    int iS = fetcher.record().indexOf("state");
    int iR = fetcher.record().indexOf("recip");

    while(fetcher.next())
    {
        if (!ui->your1_0->isVisible())
        {
            ui->your1_0->setVisible(true);
            ui->checkBox->setText("[" + fetcher.value(iR).toString() + "]  " + fetcher.value(iN).toString());
            if (fetcher.value(iS).toString() == "D") {ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
            ui->checkBox->setCheckState(Qt::Checked);}
        } else if (!ui->your1_1->isVisible())
        {
            ui->your1_1->setVisible(true);
            ui->checkBox_2->setText("[" + fetcher.value(iR).toString() + "]  " +fetcher.value(iN).toString());
            if (fetcher.value(iS).toString() == "D") {ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
            ui->checkBox_2->setCheckState(Qt::Checked);}
        } else if (!ui->your1_2->isVisible())
        {
            ui->your1_2->setVisible(true);
            ui->checkBox_3->setText("[" + fetcher.value(iR).toString() + "]  " +fetcher.value(iN).toString());
            if (fetcher.value(iS).toString() == "D") {ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
            ui->checkBox_3->setCheckState(Qt::Checked);}
        } else if (!ui->your1_3->isVisible())
        {
            ui->your1_3->setVisible(true);
            ui->checkBox_4->setText("[" + fetcher.value(iR).toString() + "]  " +fetcher.value(iN).toString());
            if (fetcher.value(iS).toString() == "D") {ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
            ui->checkBox_4->setCheckState(Qt::Checked);}
        } else if (!ui->your1_4->isVisible())
        {
            ui->your1_4->setVisible(true);
            ui->checkBox_5->setText("[" + fetcher.value(iR).toString() + "]  " +fetcher.value(iN).toString());
            if (fetcher.value(iS).toString() == "D") {ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
            ui->checkBox_5->setCheckState(Qt::Checked);}
        } else if (!ui->your1_5->isVisible())
        {
            ui->your1_5->setVisible(true);
            ui->checkBox_6->setText("[" + fetcher.value(iR).toString() + "]  " +fetcher.value(iN).toString());
            if (fetcher.value(iS).toString() == "D") {ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
            ui->checkBox_6->setCheckState(Qt::Checked);}
        } else if (!ui->your1_6->isVisible())
        {
            ui->your1_6->setVisible(true);
            ui->checkBox_7->setText("[" + fetcher.value(iR).toString() + "]  " +fetcher.value(iN).toString());
            if (fetcher.value(iS).toString() == "D") {ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
            ui->checkBox_7->setCheckState(Qt::Checked);}
        } else if (!ui->your1_7->isVisible())
        {
            ui->your1_7->setVisible(true);
            ui->checkBox_8->setText("[" + fetcher.value(iR).toString() + "]  " +fetcher.value(iN).toString());
            if (fetcher.value(iS).toString() == "D") {ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
            ui->checkBox_8->setCheckState(Qt::Checked);}
        }
    }
}
