
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog() override;

private Q_SLOTS:
    void onCancelClicked();
    void onSaveClicked();
    void onResetClicked();
    void onSelectDownDirClicked();

private:
    void assign();

    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
