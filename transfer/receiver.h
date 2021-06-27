
#ifndef RECEIVER_H
#define RECEIVER_H

#include "transfer.h"
#include "model/device.h"

class Receiver : public Transfer
{
public:
    Receiver(const Device& sender, QTcpSocket* socket, QObject* parent = nullptr);

    inline Device getSender() const { return mSenderDev; }
    inline qint64 getReceivedFileSize() const { return mFileSize; }
    inline qint64 getBytesWritten() const { return mBytesRead; }

    void resume() override;
    void pause() override;
    void cancel() override;

private Q_SLOTS:
    void onDisconnected();

private:
    void processHeaderPacket(QByteArray& data) override;
    void processDataPacket(QByteArray& data) override;
    void processFinishPacket(QByteArray& data) override;
    void processCancelPacket(QByteArray& data) override;

    Device mSenderDev;

    qint64 mFileSize;
    qint64 mBytesRead;
};

#endif // RECEIVER_H
