
#ifndef DEVICELISTMODEL_H
#define DEVICELISTMODEL_H

#include <QAbstractListModel>

#include "transfer/devicebroadcaster.h"
#include "device.h"

class DeviceListModel : public QAbstractListModel
{
public:
    DeviceListModel(DeviceBroadcaster* deviceBC, QObject* parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    void refresh();

    Device device(int index) const;
    Device device(const QString& id) const;
    Device device(const QHostAddress& address) const;

    QVector<Device> getDevices() const;
    void setDevices(const QVector<Device> &getDevices);

private Q_SLOTS:
    void onBCReceived(const Device &fromDevice);

private:
    DeviceBroadcaster* mDBC;
    QVector<Device> mDevices;
};

#endif // DEVICELISTMODEL_H
