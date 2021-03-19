#include "loginform.h"

#include <QApplication>
#include <QGraphicsDropShadowEffect>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginForm w;

    //Hide title bar
    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    //Make widget transparent
    w.setAttribute(Qt::WA_TranslucentBackground);

    // Add shadow effect
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setXOffset(2);
    effect->setYOffset(2);
    effect->setBlurRadius(10);
    effect->setColor(QColor(180, 180, 180, 180));
    w.setGraphicsEffect(effect);

    w.show();

    return a.exec();
}
