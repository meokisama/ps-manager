#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include "widget.h"

namespace Ui {
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = nullptr);
    ~LoginForm();

private slots:
    void on_pushButton_clicked();

    void on_closeButton_clicked();

private:
    Ui::LoginForm *ui;
    Widget *main;
};

#endif // LOGINFORM_H
