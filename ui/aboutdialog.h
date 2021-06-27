
#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog() override;

private Q_SLOTS:
    void onCreditsClicked(bool checked);
    void onLicenseClicked(bool checked);

private:

    Ui::AboutDialog *ui;

    QString mCredits;
    QString mLicense;
};

#endif // ABOUTDIALOG_H
