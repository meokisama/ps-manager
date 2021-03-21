#include "loginform.h"
#include "ui_loginform.h"
#include "database.h"

#include <QGraphicsDropShadowEffect>

Database database;

LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
    if(!database.connectDB()) ui->state->setText("Database is not opened!");
}

LoginForm::~LoginForm()
{
    delete ui;
}

QString LoginForm::username;
QString LoginForm::password;

void LoginForm::on_pushButton_clicked()
{
    if (database.Login(ui->username->text(),ui->password->text()))
    {
        // Meaningless in this function but needed later
        this->username = ui->username->text();
        this->password = ui->password->text();

        // Close LoginForm and open main window (ProjectManager)
        close();
        main = new Widget();
        //Hide title bar
        main->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        //Make widget transparent
        main->setAttribute(Qt::WA_TranslucentBackground);

        // Add shadow effect
        QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
        effect->setXOffset(2);
        effect->setYOffset(2);
        effect->setBlurRadius(10);
        effect->setColor(QColor(180, 180, 180, 180));
        main->setGraphicsEffect(effect);

        main->show();

    } else ui->state->setText("Invalid credentials!");
}


void LoginForm::on_closeButton_clicked()
{
    close();
}

void LoginForm::on_signUp_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void LoginForm::on_logIn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void LoginForm::on_closeButton_2_clicked()
{
    close();
}

void LoginForm::on_pushButton_2_clicked()
{
    QString fn = ui->fname->text();
    QString ln = ui->lname->text() ;
    QString un = ui->userName->text();
    QString pw = ui->passWord->text();
    QString em = ui->email->text();

    if(fn == "") ui->state_2->setText("Please enter your first name!");

    if(ln == "") ui->state_2->setText("Please enter your last name!");

    if(un == "") ui->state_2->setText("Please enter your username!");

    if(pw == "") ui->state_2->setText("Please enter your password!");

    if(em == "") ui->state_2->setText("Please enter your email!");

    else if(!em.contains("@",Qt::CaseInsensitive))
            ui->state_2->setText("Please enter the right format of email!");


    if(database.checkInfo(un, em) == 1) ui->state_2->setText("This username has been used!");

    else if(database.checkInfo(un, em) == 2) ui->state_2->setText("This email has been used!");

         else if(database.Signup(fn, ln, un, pw, em))
              {
                ui->stackedWidget->setCurrentIndex(0);
                ui->state->setText("Sign up seccessfully! You can now login.");
                ui->fname->setText("");
                ui->lname->setText("");
                ui->passWord->setText("");
                ui->userName->setText("");
                ui->email->setText("");
              } else ui->state_2->setText("Error!");
}

QString LoginForm::getUsername()
{
    return LoginForm::username;
}

QString LoginForm::getPassword()
{
    return LoginForm::password;
}
