#ifndef PROJECT_H
#define PROJECT_H

#include <QWidget>

namespace Ui {
class project;
}

class project : public QWidget
{
    Q_OBJECT

public:
    explicit project(QWidget *parent = nullptr);
    ~project();
    void getDate();

private:
    Ui::project *ui;
    QStringList color = {"254, 228, 203","186, 243, 210","252, 95, 89",
                         "233, 231, 253","255, 240, 211","255, 211, 226",
                         "186, 243, 210","254, 228, 203","1, 212, 255",
                         "228, 238, 144","255, 155, 115","182, 146, 254"};
};

#endif // PROJECT_H
