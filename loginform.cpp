#include "loginform.h"
#include "ui_loginform.h"
#include <QGraphicsDropShadowEffect>

LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::on_pushButton_clicked()
{
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
}

void LoginForm::on_closeButton_clicked()
{
    close();
}
