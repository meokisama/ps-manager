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
    static QString getUsername();
    static QString getPassword();

private slots:
    void on_pushButton_clicked();

    void on_closeButton_clicked();

    void on_signUp_clicked();

    void on_logIn_clicked();

    void on_closeButton_2_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::LoginForm *ui;
    Widget *main;
    static QString username;
    static QString password;
};

#endif // LOGINFORM_H
