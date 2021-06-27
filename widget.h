#ifndef WIDGET_H
#define WIDGET_H

#include "client.h"
#include "model/transfertablemodel.h"
#include "model/devicelistmodel.h"
#include "transfer/devicebroadcaster.h"
#include "transfer/transferserver.h"

#include <QWidget>
#include <QTextTableFormat>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void fetchProject();
    void resetProject();
    void fetchTasks();

public slots:
    void appendMessage(const QString &from, const QString &message);

private slots:
    void on_closeButton_clicked();
    void on_btnHome_clicked();
    void on_btnLiveChat_clicked();
    void on_btnCalendar_clicked();
    void on_btnVideo_clicked();
    void on_btInfo_clicked();
    void on_btnUser_clicked();
    void on_btnSave_clicked();
    void on_addProject_clicked();
    void on_btnCreate_clicked();
    void on_btnCancel_clicked();
    void on_makeCallBtn_clicked();
    void returnPressed();
    void newParticipant(const QString &nick);
    void participantLeft(const QString &nick);
    void showInformation();

    void onSendFilesActionTriggered();
    void onSendFolderActionTriggered();
    void onSettingsActionTriggered();
    void onAboutActionTriggered();

    void onNewReceiverAdded(Receiver* rec);

    void onSenderTableDoubleClicked(const QModelIndex& index);
    void onSenderClearClicked();
    void onSenderCancelClicked();
    void onSenderPauseClicked();
    void onSenderResumeClicked();

    void onReceiverTableDoubleClicked(const QModelIndex& index);
    void onReceiverClearClicked();
    void onReceiverCancelClicked();
    void onReceiverPauseClicked();
    void onReceiverResumeClicked();

    void onSenderTableSelectionChanged(const QItemSelection& selected,
                                       const QItemSelection& deselected);
    void onReceiverTableSelectionChanged(const QItemSelection& selected,
                                         const QItemSelection& deselected);

    void onSenderTableContextMenuRequested(const QPoint& pos);
    void onReceiverTableContextMenuRequested(const QPoint& pos);

    void openSenderFileInCurrentIndex();
    void openSenderFolderInCurrentIndex();
    void removeSenderItemInCurrentIndex();

    void openReceiverFileInCurrentIndex();
    void openReceiverFolderInCurrentIndex();
    void removeReceiverItemInCurrentIndex();
    void deleteReceiverFileInCurrentIndex();

    void onSelectedSenderStateChanged(TransferState state);
    void onSelectedReceiverStateChanged(TransferState state);

private:
    Ui::Widget *ui;
    int h = 0 , v = 1;
    Client client;
    QString myNickName;
    QTextTableFormat tableFormat;
    void setupActions();
    void setupToolbar();
    void connectSignals();
    void sendFile(const QString& folderName, const QString& fileName, const Device& receiver);
    void selectReceiversAndSendTheFiles(QVector<QPair<QString, QString> > dirNameAndFullPath);

    bool anyActiveSender();
    bool anyActiveReceiver();

    TransferTableModel* mSenderModel;
    TransferTableModel* mReceiverModel;
    DeviceListModel* mDeviceModel;

    DeviceBroadcaster* mBroadcaster;
    TransferServer* mTransServer;

    QAction* mShowMainWindowAction;
    QAction* mSendFilesAction;
    QAction* mSendFolderAction;
    QAction* mSettingsAction;
    QAction* mAboutAction;
    QAction* mAboutQtAction;
    QAction* mQuitAction;

    QAction* mSenderOpenAction;
    QAction* mSenderOpenFolderAction;
    QAction* mSenderRemoveAction;
    QAction* mSenderClearAction;
    QAction* mSenderPauseAction;
    QAction* mSenderResumeAction;
    QAction* mSenderCancelAction;

    QAction* mRecOpenAction;
    QAction* mRecOpenFolderAction;
    QAction* mRecRemoveAction;
    QAction* mRecDeleteAction;
    QAction* mRecClearAction;
    QAction* mRecPauseAction;
    QAction* mRecResumeAction;
    QAction* mRecCancelAction;
};
#endif // WIDGET_H
