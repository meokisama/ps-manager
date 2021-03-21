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
    database.connectDB();
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::on_pushButton_clicked()
{
    if (Login(ui->username->text(),ui->password->text()))
    {
        this->username = ui->username->text();
        this->password = ui->password->text();

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

    } else {
        ui->state->setText("Invalid credentials!");
    }
}

bool LoginForm::Login(QString u, QString p)
{
    bool exist = false;

    QSqlQuery check(database.getDB());
    check.prepare("SELECT USERNAME FROM USER WHERE USERNAME = (:un) AND PASSWORD = (:pw)");
    check.bindValue(":un", u);
    check.bindValue(":pw", p);

    if (check.exec())
    {
        if (check.next())
        {
            exist = true;
        }
    }

    return exist;
}

void LoginForm::on_closeButton_clicked()
{
    close();
}
