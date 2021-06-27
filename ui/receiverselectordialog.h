
#ifndef RECEIVERSELECTORDIALOG_H
#define RECEIVERSELECTORDIALOG_H

#include <QDialog>

class DeviceListModel;
class Device;

namespace Ui {
class ReceiverSelectorDialog;
}

class ReceiverSelectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReceiverSelectorDialog(DeviceListModel* model, QWidget *parent = nullptr);
    ~ReceiverSelectorDialog() override;

    Device getSelectedDevice() const;
    QVector<Device> getSelectedDevices() const;

private Q_SLOTS:
    void onSendClicked();
    void onRefreshClicked();

private:
    Ui::ReceiverSelectorDialog *ui;

    DeviceListModel* mModel;
};

#endif // RECEIVERSELECTORDIALOG_H
