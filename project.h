#ifndef PROJECT_H
#define PROJECT_H

#include <QWidget>
#include "task.h"

namespace Ui {
class project;
}

class project : public QWidget
{
    Q_OBJECT

public:
    explicit project(QWidget *parent = nullptr);
    ~project();
    int getDate(QDate);
    void Style();
    void setValue(QString, QDate);
    void fetchValue(int, QString, int);
    QString id;

private slots:
    void on_pushButton_clicked();

    void on_btnEdit_clicked();

private:
    Ui::project *ui;
    Task *task;
    QStringList color = {"254, 228, 203","186, 243, 210","254, 190, 188",
                         "233, 231, 253","255, 240, 211","255, 211, 226",
                         "186, 243, 210","254, 228, 203","129, 232, 254",
                         "228, 238, 144","255, 155, 115","202, 177, 254"};
};

#endif // PROJECT_H
