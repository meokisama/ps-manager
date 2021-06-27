#ifndef WIDGET_H
#define WIDGET_H

#include "client.h"

#include <QWidget>
#include <QTextTableFormat>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void fetchProject();
    void resetProject();
    void fetchTasks();

public slots:
    void appendMessage(const QString &from, const QString &message);

private slots:
    void on_closeButton_clicked();
    void on_btnHome_clicked();
    void on_btnLiveChat_clicked();
    void on_btnCalendar_clicked();
    void on_btnVideo_clicked();
    void on_btInfo_clicked();
    void on_btnUser_clicked();
    void on_btnSave_clicked();
    void on_addProject_clicked();
    void on_btnCreate_clicked();
    void on_btnCancel_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void returnPressed();
    void newParticipant(const QString &nick);
    void participantLeft(const QString &nick);
    void showInformation();

private:
    Ui::Widget *ui;
    int h = 0 , v = 1;
    Client client;
    QString myNickName;
    QTextTableFormat tableFormat;
};
#endif // WIDGET_H
