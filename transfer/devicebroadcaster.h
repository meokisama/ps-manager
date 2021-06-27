
#ifndef DEVICEBROADCASTER_H
#define DEVICEBROADCASTER_H

#include <QObject>
#include <QTimer>
#include <QtNetwork>

#include "model/device.h"

/*
 * DeviceBroadcaster digunakan untuk membroadcast Device,
 * dengan mengirim packet data berisi informasi Device menggunakan
 * protokol UDP
 */
class DeviceBroadcaster : public QObject
{
    Q_OBJECT

public:
    explicit DeviceBroadcaster(QObject *parent = nullptr);

Q_SIGNALS:
    void broadcastReceived(const Device& fromDevice);

public Q_SLOTS:
    void start();
    void sendBroadcast();

private Q_SLOTS:
    void processBroadcast();

private:
    QVector<QHostAddress> getBroadcastAddressFromInterfaces();

    QTimer mTimer;
    QUdpSocket mUdpSock;
};

#endif // DEVICEBROADCASTER_H
