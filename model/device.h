
#ifndef DEVICE_H
#define DEVICE_H

#include <QtNetwork/QHostAddress>
#include <QObject>

/*
 * class Device merepresentasikan Node/Computer yang terhubung ke jaringan/LAN
 * yang sama dan bisa bertransfer data
 */
class Device
{
public:
    explicit Device() = default;
    Device(const QString &id, const QString &name, const QString &osName, const QHostAddress &addr);

    inline QString getId() const { return mId; }
    inline QString getName() const { return mName; }
    inline QHostAddress getAddress() const { return mAddress; }
    inline QString getOSName() const { return mOSName; }
    bool isValid() const;

    void setId(const QString& id);
    void setName(const QString& name);
    void setAddress(const QHostAddress& address);
    void setOSName(const QString& osName);

    bool operator==(const Device& other) const;
    bool operator!=(const Device& other) const;

private:
    QString mId{""};
    QString mName{""};
    QString mOSName{""};
    QHostAddress mAddress{QHostAddress::Null};
};

#endif // DEVICE_H
