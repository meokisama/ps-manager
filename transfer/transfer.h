
#ifndef TRANSFER_H
#define TRANSFER_H

#include <QFile>
#include <QTcpSocket>
#include <QObject>

#include "model/device.h"
#include "model/transferinfo.h"


enum class PacketType : char
{
    Header = 0x01,
    Data,
    Finish,
    Cancel,
    Pause,
    Resume
};

class Transfer : public QObject
{
    Q_OBJECT

public:
    Transfer(QTcpSocket* socket, QObject* parent = nullptr);

    inline QFile* getFile() const { return mFile; }
    inline QTcpSocket* getSocket() const { return mSocket; }
    inline TransferInfo* getTransferInfo() const { return mInfo; }

    virtual void resume();
    virtual void pause();
    virtual void cancel();

protected:
    void clearReadBuffer();
    void setSocket(QTcpSocket* socket);

    virtual void processPacket(QByteArray& data, PacketType type);
    virtual void processHeaderPacket(QByteArray& data);
    virtual void processDataPacket(QByteArray& data);
    virtual void processFinishPacket(QByteArray& data);
    virtual void processCancelPacket(QByteArray& data);
    virtual void processPausePacket(QByteArray& data);
    virtual void processResumePacket(QByteArray& data);

    virtual void writePacket(qint32 packetDataSize, PacketType type, const QByteArray& data);

    QFile* mFile;
    QTcpSocket* mSocket;
    TransferInfo* mInfo;

private Q_SLOTS:
    void onReadyRead();

private:

    //
    QByteArray mBuff;
    qint32 mPacketSize;

    int mHeaderSize;
};

#endif // TRANSFER_H
