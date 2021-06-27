
#ifndef SENDER_H
#define SENDER_H

#include "transfer.h"
#include "model/device.h"

class Sender : public Transfer
{
public:
    Sender(const Device& receiver, const QString& folderName, const QString& filePath, QObject* parent = nullptr);

    bool start();

    Device getReceiver() const { return mReceiverDev; }

    void resume() override;
    void pause() override;
    void cancel() override;

private Q_SLOTS:
    void onBytesWritten(qint64 bytes);
    void onConnected();
    void onDisconnected();

private:
    void finish();
    void sendData();
    void sendHeader();

    void processCancelPacket(QByteArray& data) override;
    void processPausePacket(QByteArray& data) override;
    void processResumePacket(QByteArray& data) override;

    Device mReceiverDev;
    QString mFilePath;
    QString mFolderName;
    qint64 mFileSize;
    qint64 mBytesRemaining;

    QByteArray mFileBuff;
    qint32 mFileBuffSize;

    bool mCancelled;
    bool mPaused;
    bool mPausedByReceiver;
    bool mIsHeaderSent;
};

#endif // SENDER_H
