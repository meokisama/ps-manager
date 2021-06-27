#include "widget.h"
#include "ui_widget.h"
#include "loginform.h"
#include "database.h"
#include "project.h"
#include <QDesktopServices>
#include <QProgressBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>
#include <QToolButton>
#include <QCloseEvent>
#include <QtDebug>
#include <QListView>
#include <QTreeView>

#include "ui/receiverselectordialog.h"
#include "ui/settingsdialog.h"
#include "settings.h"
#include "ui/aboutdialog.h"
#include "util.h"
#include "transfer/sender.h"
#include "transfer/receiver.h"
#include <QtWidgets>
#include <QSqlRecord>
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>

Database dtbase;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    fetchProject();
    v = ui->gridLayout->rowCount();
    h = ui->gridLayout->columnCount();
    QSqlQuery fetcher;
    fetcher.prepare("SELECT * FROM USER WHERE USERNAME = (:un)");
    fetcher.bindValue(":un", LoginForm::getUsername());
    fetcher.exec();

    int iLN = fetcher.record().indexOf("TEN");

    while(fetcher.next())
    {
        ui->showName->setText(fetcher.value(iLN).toString());
    }
    ui->addp->setVisible(false);
    ui->dateEdit->setDate(QDate::currentDate());

    ui->inputMes->setFocusPolicy(Qt::StrongFocus);
    ui->textEdit->setFocusPolicy(Qt::NoFocus);
    ui->textEdit->setReadOnly(true);
    ui->listWidget->setFocusPolicy(Qt::NoFocus);

    connect(ui->inputMes, &QLineEdit::returnPressed,
            this, &Widget::returnPressed);
    connect(&client, &Client::newMessage,
            this, &Widget::appendMessage);
    connect(&client, &Client::newParticipant,
            this, &Widget::newParticipant);
    connect(&client, &Client::participantLeft,
            this, &Widget::participantLeft);

    myNickName = client.nickName();
    newParticipant(myNickName);
    tableFormat.setBorder(0);
    QTimer::singleShot(10 * 1000, this, SLOT(showInformation()));

    ui->your1_0->setVisible(false);
    ui->your1_1->setVisible(false);
    ui->your1_2->setVisible(false);
    ui->your1_3->setVisible(false);
    ui->your1_4->setVisible(false);
    ui->your1_5->setVisible(false);
    ui->your1_6->setVisible(false);
    ui->your1_7->setVisible(false);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_closeButton_clicked()
{
    close();
}

void Widget::on_btnHome_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    resetProject();
    fetchProject();
}

void Widget::on_btnLiveChat_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::on_btnCalendar_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    setupActions();
    setupToolbar();
    mBroadcaster = new DeviceBroadcaster(this);
    mBroadcaster->start();
    mSenderModel = new TransferTableModel(this);
    mReceiverModel = new TransferTableModel(this);
    mDeviceModel = new DeviceListModel(mBroadcaster, this);
    mTransServer = new TransferServer(mDeviceModel, this);
    mTransServer->listen();

//    mSenderModel->setHeaderData((int) TransferTableModel::Column::Peer, Qt::Horizontal, tr("Receiver"));
//    mReceiverModel->setHeaderData((int) TransferTableModel::Column::Peer, Qt::Horizontal, tr("Sender"));

    ui->senderTableView->setModel(mSenderModel);
    ui->receiverTableView->setModel(mReceiverModel);

    ui->senderTableView->setColumnWidth((int)TransferTableModel::Column::FileName, 340);
    ui->senderTableView->setColumnWidth((int)TransferTableModel::Column::Progress, 160);

    ui->receiverTableView->setColumnWidth((int)TransferTableModel::Column::FileName, 340);
    ui->receiverTableView->setColumnWidth((int)TransferTableModel::Column::Progress, 160);

    connectSignals();
}

void Widget::on_btnVideo_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void Widget::on_btInfo_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void Widget::on_btnUser_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
    ui->state->setText("");

    QSqlQuery fetcher;
    fetcher.prepare("SELECT * FROM USER WHERE USERNAME = (:un)");
    fetcher.bindValue(":un", LoginForm::getUsername());
    fetcher.exec();

    int iUN = fetcher.record().indexOf("USERNAME");
    int iPW = fetcher.record().indexOf("PASSWORD");
    int iFN = fetcher.record().indexOf("HO");
    int iLN = fetcher.record().indexOf("TEN");
    int iEM = fetcher.record().indexOf("EMAIL");

    while(fetcher.next())
    {
        ui->fname->setText(fetcher.value(iFN).toString());
        ui->lname->setText(fetcher.value(iLN).toString());
        ui->un->setText(fetcher.value(iUN).toString());
        ui->pw->setText(fetcher.value(iPW).toString());
        ui->em->setText(fetcher.value(iEM).toString());
        QString full = fetcher.value(iFN).toString();
        ui->fullname->setText(full.append(" ").append(fetcher.value(iLN).toString()));
    }

    fetchTasks();
}

void Widget::on_btnSave_clicked()
{
    QString fn = ui->fname->text();
    QString ln = ui->lname->text() ;
    QString un = ui->un->text();
    QString pw = ui->pw->text();
    QString em = ui->em->text();

    if(fn == "") ui->state->setText("Please enter your first name!");

    else if(ln == "") ui->state->setText("Please enter your last name!");

    else if(un == "") ui->state->setText("Please enter your username!");

    else if(pw == "") ui->state->setText("Please enter your password!");

    else if(em == "") ui->state->setText("Please enter your email!");

    else if(!em.contains("@",Qt::CaseInsensitive))
            ui->state->setText("Please enter the right format of email!");

    else if(dtbase.Update(fn, ln, un, pw, em))
    {
        ui->state->setText("Your information has updated successfully!");
        ui->fullname->setText(fn.append(" ").append(ln));
        ui->showName->setText(ln);
    } else ui->state->setText("Error!");
}

void Widget::on_addProject_clicked()
{
    ui->addp->setVisible(true);
}

void Widget::on_btnCreate_clicked()
{
    resetProject();
    fetchProject();
    if (ui->gridLayout->count() == 0) {v=1; h=0;}
    else if (ui->gridLayout->count() == 1){v=1; h=1;}
    else if (ui->gridLayout->count() == 2){v=1; h=2;}
    else if (ui->gridLayout->count() == 3){v=1; h=3;}
    else if (ui->gridLayout->count() == 4){v=2; h=1;}
    else if (ui->gridLayout->count() == 5){v=2; h=2;}
    else if (ui->gridLayout->count() == 6){v=2; h=3;}

    project *newp = new project();
    newp->setValue(ui->pname->text(),ui->dateEdit->date());
    h += 1;
    if(h > 3) {
        h = 1;
        v += 1;
        if(v > 2) {
            v = 1;
            QMessageBox::information(this, tr("  Rất tiếc"), tr("\nHiện tại, ứng dụng chỉ hỗ trợ chừng này projects.\t\n"), QMessageBox::Ok);
            delete newp;
            return;
        }
    }
    if(ui->pname->text() != "") {
        QDate a;
        dtbase.addProject(newp->id, ui->pname->text(), a.currentDate().toJulianDay(), ui->dateEdit->date().toJulianDay());
        ui->gridLayout->addWidget(newp,v,h,Qt::Alignment());
        ui->addp->setVisible(false);
        ui->pname->clear();
    } else ui->state_2->setText("Enter project's name");

}

void Widget::on_btnCancel_clicked()
{
    ui->addp->setVisible(false);
}

void Widget::fetchProject()
{
    QSqlQuery fetcher;
    fetcher.prepare("SELECT * FROM projects");

    fetcher.exec();

    int iD = fetcher.record().indexOf("id");
    int iN = fetcher.record().indexOf("name");
    int iS = fetcher.record().indexOf("startdate");
    int iF = fetcher.record().indexOf("findate");

    int v1 = 1, h1 = 0;

    while(fetcher.next())
    {
        project *newp = new project();
        newp->id = fetcher.value(iD).toString();
        newp->fetchValue(fetcher.value(iS).toInt(), fetcher.value(iN).toString(), fetcher.value(iF).toInt() - fetcher.value(iS).toInt());
        h1 += 1;
        if(h1 > 3) {
            h1 = 1;
            v1 += 1;
            if(v1 > 2) {
                v1 = 1;
            }
        }
        ui->gridLayout->addWidget(newp,v1, h1,Qt::Alignment());
    }
}

void Widget::resetProject()
{
    while (ui->gridLayout->count())
    {
        QLayoutItem* item = ui->gridLayout->itemAt(0);
        ui->gridLayout->removeItem(item);

        QWidget* widget = item->widget();
        if(widget) delete widget;
    }
}

void Widget::on_makeCallBtn_clicked()
{
    //QDesktopServices::openUrl(QUrl("https://meoki.herokuapp.com/join/Meoki"));
    QDesktopServices::openUrl(QUrl("http://localhost:3000"));
}

void Widget::appendMessage(const QString &from, const QString &message)
{
    if (from.isEmpty() || message.isEmpty())
        return;

    QTextCursor cursor(ui->textEdit->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable *table = cursor.insertTable(1, 2, tableFormat);
    table->cellAt(0, 0).firstCursorPosition().insertText('<' + from + "> ");
    table->cellAt(0, 1).firstCursorPosition().insertText(message);
    QScrollBar *bar = ui->textEdit->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void Widget::returnPressed()
{
    QString text = ui->inputMes->text();
    if (text.isEmpty())
        return;

    if (text.startsWith(QChar('/'))) {
        QColor color = ui->textEdit->textColor();
        ui->textEdit->setTextColor(Qt::red);
        ui->textEdit->append(tr("! Unknown command: %1")
                         .arg(text.left(text.indexOf(' '))));
        ui->textEdit->setTextColor(color);
    } else {
        client.sendMessage(text);
        appendMessage(myNickName, text);
    }

    ui->inputMes->clear();
}

void Widget::newParticipant(const QString &nick)
{
    if (nick.isEmpty())
        return;

    QColor color = ui->textEdit->textColor();
    ui->textEdit->setTextColor(Qt::gray);
    ui->textEdit->append(tr("* %1 has joined").arg(nick));
    ui->textEdit->setTextColor(color);
    ui->listWidget->addItem(nick);
}

void Widget::participantLeft(const QString &nick)
{
    if (nick.isEmpty())
        return;

    QList<QListWidgetItem *> items = ui->listWidget->findItems(nick,
                                                           Qt::MatchExactly);
    if (items.isEmpty())
        return;

    delete items.at(0);
    QColor color = ui->textEdit->textColor();
    ui->textEdit->setTextColor(Qt::gray);
    ui->textEdit->append(tr("* %1 has left").arg(nick));
    ui->textEdit->setTextColor(color);
}

void Widget::showInformation()
{
    if (ui->listWidget->count() == 1) {
        //QMessageBox::information(this, tr("Chat"), tr("Launch several instances of this program on your local network and start chatting!"));
    }
}

void Widget::fetchTasks()
{
    QSqlQuery fetcher;
    fetcher.prepare("SELECT * FROM tasks WHERE recip = (:rc)");
    fetcher.bindValue(":rc", ui->lname->text());
    fetcher.exec();

    int iN = fetcher.record().indexOf("name");
    int iS = fetcher.record().indexOf("state");
    int iR = fetcher.record().indexOf("recip");

    while(fetcher.next())
    {
        if (!ui->your1_0->isVisible())
        {
            ui->your1_0->setVisible(true);
            ui->checkBox->setText("[" + fetcher.value(iR).toString() + "]  " + fetcher.value(iN).toString());
            if (fetcher.value(iS).toString() == "D") {ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
            ui->checkBox->setCheckState(Qt::Checked);}
        } else if (!ui->your1_1->isVisible())
        {
            ui->your1_1->setVisible(true);
            ui->checkBox_2->setText("[" + fetcher.value(iR).toString() + "]  " +fetcher.value(iN).toString());
            if (fetcher.value(iS).toString() == "D") {ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
            ui->checkBox_2->setCheckState(Qt::Checked);}
        } else if (!ui->your1_2->isVisible())
        {
            ui->your1_2->setVisible(true);
            ui->checkBox_3->setText("[" + fetcher.value(iR).toString() + "]  " +fetcher.value(iN).toString());
            if (fetcher.value(iS).toString() == "D") {ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
            ui->checkBox_3->setCheckState(Qt::Checked);}
        } else if (!ui->your1_3->isVisible())
        {
            ui->your1_3->setVisible(true);
            ui->checkBox_4->setText("[" + fetcher.value(iR).toString() + "]  " +fetcher.value(iN).toString());
            if (fetcher.value(iS).toString() == "D") {ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
            ui->checkBox_4->setCheckState(Qt::Checked);}
        } else if (!ui->your1_4->isVisible())
        {
            ui->your1_4->setVisible(true);
            ui->checkBox_5->setText("[" + fetcher.value(iR).toString() + "]  " +fetcher.value(iN).toString());
            if (fetcher.value(iS).toString() == "D") {ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
            ui->checkBox_5->setCheckState(Qt::Checked);}
        } else if (!ui->your1_5->isVisible())
        {
            ui->your1_5->setVisible(true);
            ui->checkBox_6->setText("[" + fetcher.value(iR).toString() + "]  " +fetcher.value(iN).toString());
            if (fetcher.value(iS).toString() == "D") {ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
            ui->checkBox_6->setCheckState(Qt::Checked);}
        } else if (!ui->your1_6->isVisible())
        {
            ui->your1_6->setVisible(true);
            ui->checkBox_7->setText("[" + fetcher.value(iR).toString() + "]  " +fetcher.value(iN).toString());
            if (fetcher.value(iS).toString() == "D") {ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
            ui->checkBox_7->setCheckState(Qt::Checked);}
        } else if (!ui->your1_7->isVisible())
        {
            ui->your1_7->setVisible(true);
            ui->checkBox_8->setText("[" + fetcher.value(iR).toString() + "]  " +fetcher.value(iN).toString());
            if (fetcher.value(iS).toString() == "D") {ui->checkBox->setStyleSheet("font-style: italic; text-decoration: line-through;");
            ui->checkBox_8->setCheckState(Qt::Checked);}
        }
    }
}



void Widget::connectSignals()
{
    connect(mTransServer, &TransferServer::newReceiverAdded, this, &Widget::onNewReceiverAdded);

    QItemSelectionModel* senderSel = ui->senderTableView->selectionModel();
    connect(senderSel, &QItemSelectionModel::selectionChanged,
            this, &Widget::onSenderTableSelectionChanged);

    QItemSelectionModel* receiverSel = ui->receiverTableView->selectionModel();
    connect(receiverSel, &QItemSelectionModel::selectionChanged,
            this, &Widget::onReceiverTableSelectionChanged);
}

void Widget::sendFile(const QString& folderName, const QString &filePath, const Device &receiver)
{
    Sender* sender = new Sender(receiver, folderName, filePath, this);
    sender->start();
    mSenderModel->insertTransfer(sender);
    QModelIndex progressIdx = mSenderModel->index(0, (int)TransferTableModel::Column::Progress);

    QProgressBar* progress = new QProgressBar();
    connect(sender->getTransferInfo(), &TransferInfo::progressChanged, progress, &QProgressBar::setValue);

    ui->senderTableView->setIndexWidget(progressIdx, progress);
    ui->senderTableView->scrollToTop();
}

void Widget::selectReceiversAndSendTheFiles(QVector<QPair<QString, QString> > dirNameAndFullPath)
{
    ReceiverSelectorDialog dialog(mDeviceModel);
    if (dialog.exec() == QDialog::Accepted) {
        QVector<Device> receivers = dialog.getSelectedDevices();
        for (const Device& receiver : receivers) {
            if (receiver.isValid()) {

                mBroadcaster->sendBroadcast();
                for (const auto& p : dirNameAndFullPath) {
                    sendFile(p.first, p.second, receiver);
                }

            }
        }
    }
}

void Widget::onSendFilesActionTriggered()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select files"));
    if (fileNames.size() <= 0)
        return;

    QVector<QPair<QString, QString> > pairs;
    for (const auto& fName : fileNames)
        pairs.push_back( QPair<QString, QString>("", fName) );

    selectReceiversAndSendTheFiles(pairs);
}

void Widget::onSendFolderActionTriggered()
{
    QStringList dirs;
    QFileDialog fDialog(this);
    fDialog.setOption(QFileDialog::DontUseNativeDialog, true);
    fDialog.setFileMode(QFileDialog::Directory);
    fDialog.setOption(QFileDialog::ShowDirsOnly);

    /*
     * Enable multiple foder selection
     */
    QListView* lView = fDialog.findChild<QListView*>("listView");
    if (lView)
        lView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QTreeView* tView = fDialog.findChild<QTreeView*>("treeView");
    if (tView)
        tView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    if (!fDialog.exec()) {
        return;
    }

    /*
     * Iterate through all selected folders
     */
    QVector< QPair<QString, QString> > pairs;
    dirs = fDialog.selectedFiles();
    for (const auto& dirName : dirs) {

        QDir dir(dirName);
        QVector< QPair<QString, QString> > ps =
                Util::getInnerDirNameAndFullFilePath(dir, dir.dirName());
        pairs.append(ps);
    }

    selectReceiversAndSendTheFiles(pairs);
}

void Widget::onSettingsActionTriggered()
{
    SettingsDialog dialog;
    dialog.exec();
}

void Widget::onAboutActionTriggered()
{
    AboutDialog dialog;
    dialog.exec();
}

void Widget::onNewReceiverAdded(Receiver *rec)
{
    QProgressBar* progress = new QProgressBar();
    connect(rec->getTransferInfo(), &TransferInfo::progressChanged, progress, &QProgressBar::setValue);
    mReceiverModel->insertTransfer(rec);
    QModelIndex progressIdx = mReceiverModel->index(0, (int)TransferTableModel::Column::Progress);

    ui->receiverTableView->setIndexWidget(progressIdx, progress);
    ui->receiverTableView->scrollToTop();
}

void Widget::onSenderTableSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    if (selected.size() > 0) {

        QModelIndex first = selected.indexes().first();
        if (first.isValid()) {
            TransferInfo* ti = mSenderModel->getTransferInfo(first.row());
            ui->resumeSenderBtn->setEnabled(ti->canResume());
            ui->pauseSenderBtn->setEnabled(ti->canPause());
            ui->cancelSenderBtn->setEnabled(ti->canCancel());

            connect(ti, &TransferInfo::stateChanged, this, &Widget::onSelectedSenderStateChanged);
        }

    }

    if (deselected.size() > 0) {

        QModelIndex first = deselected.indexes().first();
        if (first.isValid()) {
            TransferInfo* ti = mSenderModel->getTransferInfo(first.row());
            disconnect(ti, &TransferInfo::stateChanged, this, &Widget::onSelectedSenderStateChanged);
        }

    }
}

void Widget::onSenderTableDoubleClicked(const QModelIndex& index)
{
    Q_UNUSED(index);
    openSenderFileInCurrentIndex();
}

void Widget::onSenderClearClicked()
{
    mSenderModel->clearCompleted();
}

void Widget::onSenderCancelClicked()
{
    QModelIndex currIndex = ui->senderTableView->currentIndex();
    if (currIndex.isValid()) {
        Transfer* sender = mSenderModel->getTransfer(currIndex.row());
        sender->cancel();
    }
}

void Widget::onSenderPauseClicked()
{
    QModelIndex currIndex = ui->senderTableView->currentIndex();
    if (currIndex.isValid()) {
        Transfer* sender = mSenderModel->getTransfer(currIndex.row());
        sender->pause();
    }
}

void Widget::onSenderResumeClicked()
{
    QModelIndex currIndex = ui->senderTableView->currentIndex();
    if (currIndex.isValid()) {
        Transfer* sender = mSenderModel->getTransfer(currIndex.row());
        sender->resume();
    }
}


void Widget::onReceiverTableDoubleClicked(const QModelIndex& index)
{
    if (index.isValid()) {
        TransferInfo* ti = mReceiverModel->getTransferInfo(index.row());
        if (ti && ti->getState() == TransferState::Finish)
            openReceiverFileInCurrentIndex();
    }
}

void Widget::onReceiverClearClicked()
{
    mReceiverModel->clearCompleted();
}

void Widget::onReceiverTableSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    if (selected.size() > 0) {

        QModelIndex first = selected.indexes().first();
        if (first.isValid()) {
            TransferInfo* ti = mReceiverModel->getTransferInfo(first.row());
            ui->resumeReceiverBtn->setEnabled(ti->canResume());
            ui->pauseReceiverBtn->setEnabled(ti->canPause());
            ui->cancelReceiverBtn->setEnabled(ti->canCancel());

            connect(ti, &TransferInfo::stateChanged, this, &Widget::onSelectedReceiverStateChanged);
        }

    }

    if (deselected.size() > 0) {

        QModelIndex first = deselected.indexes().first();
        if (first.isValid()) {
            TransferInfo* ti = mReceiverModel->getTransferInfo(first.row());
            disconnect(ti, &TransferInfo::stateChanged, this, &Widget::onSelectedReceiverStateChanged);
        }

    }
}

void Widget::onReceiverCancelClicked()
{
    QModelIndex currIndex = ui->receiverTableView->currentIndex();
    if (currIndex.isValid()) {
        Transfer* rec = mReceiverModel->getTransfer(currIndex.row());
        rec->cancel();
    }
}

void Widget::onReceiverPauseClicked()
{
    QModelIndex currIndex = ui->receiverTableView->currentIndex();
    if (currIndex.isValid()) {
        Transfer* rec = mReceiverModel->getTransfer(currIndex.row());
        rec->pause();
    }
}

void Widget::onReceiverResumeClicked()
{
    QModelIndex currIndex = ui->receiverTableView->currentIndex();
    if (currIndex.isValid()) {
        Transfer* rec = mReceiverModel->getTransfer(currIndex.row());
        rec->resume();
    }
}

void Widget::onSenderTableContextMenuRequested(const QPoint& pos)
{
    QModelIndex currIndex = ui->senderTableView->indexAt(pos);
    QMenu contextMenu;

    if (currIndex.isValid()) {
        TransferInfo* ti = mSenderModel->getTransferInfo(currIndex.row());
        TransferState state = ti->getState();
        bool enableRemove = state == TransferState::Finish ||
                            state == TransferState::Cancelled ||
                            state == TransferState::Disconnected ||
                            state == TransferState::Idle;

        mSenderRemoveAction->setEnabled(enableRemove);
        mSenderPauseAction->setEnabled(ti->canPause());
        mSenderResumeAction->setEnabled(ti->canResume());
        mSenderCancelAction->setEnabled(ti->canCancel());

        contextMenu.addAction(mSenderOpenAction);
        contextMenu.addAction(mSenderOpenFolderAction);
        contextMenu.addSeparator();
        contextMenu.addAction(mSendFilesAction);
        contextMenu.addAction(mSendFolderAction);
        contextMenu.addSeparator();
        contextMenu.addAction(mSenderRemoveAction);
        contextMenu.addAction(mSenderClearAction);
        contextMenu.addSeparator();
        contextMenu.addAction(mSenderPauseAction);
        contextMenu.addAction(mSenderResumeAction);
        contextMenu.addAction(mSenderCancelAction);
    }
    else {
        contextMenu.addAction(mSendFilesAction);
        contextMenu.addAction(mSendFolderAction);
        contextMenu.addSeparator();
        contextMenu.addAction(mSenderClearAction);
    }

    QPoint globPos = ui->senderTableView->mapToGlobal(pos);
    contextMenu.exec(globPos);
}

void Widget::onReceiverTableContextMenuRequested(const QPoint& pos)
{
    QModelIndex currIndex = ui->receiverTableView->indexAt(pos);
    QMenu contextMenu;

    if (currIndex.isValid()) {
        TransferInfo* ti = mReceiverModel->getTransferInfo(currIndex.row());
        TransferState state = ti->getState();
        bool enableFileMenu = state == TransferState::Finish;
        bool enableRemove = state == TransferState::Finish ||
                            state == TransferState::Cancelled ||
                            state == TransferState::Disconnected ||
                            state == TransferState::Idle;

        mRecOpenAction->setEnabled(enableFileMenu);
        mRecOpenFolderAction->setEnabled(enableFileMenu);
        mRecRemoveAction->setEnabled(enableFileMenu | enableRemove);
        mRecDeleteAction->setEnabled(enableFileMenu);
        mRecPauseAction->setEnabled(ti->canPause());
        mRecResumeAction->setEnabled(ti->canResume());
        mRecCancelAction->setEnabled(ti->canCancel());

        contextMenu.addAction(mRecOpenAction);
        contextMenu.addAction(mRecOpenFolderAction);
        contextMenu.addAction(mRecRemoveAction);
        contextMenu.addAction(mRecDeleteAction);
        contextMenu.addAction(mRecClearAction);
        contextMenu.addSeparator();
        contextMenu.addAction(mRecPauseAction);
        contextMenu.addAction(mRecResumeAction);
        contextMenu.addAction(mRecCancelAction);
    }
    else {
        contextMenu.addAction(mRecClearAction);
    }

    QPoint globPos = ui->receiverTableView->mapToGlobal(pos);
    contextMenu.exec(globPos);
}

void Widget::openSenderFileInCurrentIndex()
{
    QModelIndex currIndex = ui->senderTableView->currentIndex();
    QModelIndex fileNameIndex = mSenderModel->index(currIndex.row(), (int)TransferTableModel::Column::FileName);
    QString fileName = mSenderModel->data(fileNameIndex).toString();

    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}

void Widget::openSenderFolderInCurrentIndex()
{
    QModelIndex currIndex = ui->senderTableView->currentIndex();
    QModelIndex fileNameIndex = mSenderModel->index(currIndex.row(), (int)TransferTableModel::Column::FileName);
    QString dir = QFileInfo(mSenderModel->data(fileNameIndex).toString()).absoluteDir().absolutePath();

    QDesktopServices::openUrl(QUrl::fromLocalFile(dir));
}

void Widget::removeSenderItemInCurrentIndex()
{
    QModelIndex currIndex = ui->senderTableView->currentIndex();
    mSenderModel->removeTransfer(currIndex.row());
}

void Widget::openReceiverFileInCurrentIndex()
{
    QModelIndex currIndex = ui->receiverTableView->currentIndex();
    QModelIndex fileNameIndex = mReceiverModel->index(currIndex.row(), (int)TransferTableModel::Column::FileName);
    QString fileName = mReceiverModel->data(fileNameIndex).toString();

    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}

void Widget::openReceiverFolderInCurrentIndex()
{
    QModelIndex currIndex = ui->receiverTableView->currentIndex();
    QModelIndex fileNameIndex = mReceiverModel->index(currIndex.row(), (int)TransferTableModel::Column::FileName);
    QString dir = QFileInfo(mReceiverModel->data(fileNameIndex).toString()).absoluteDir().absolutePath();

    QDesktopServices::openUrl(QUrl::fromLocalFile(dir));
}

void Widget::removeReceiverItemInCurrentIndex()
{
    QModelIndex currIndex = ui->receiverTableView->currentIndex();
    mReceiverModel->removeTransfer(currIndex.row());
}

void Widget::deleteReceiverFileInCurrentIndex()
{
    QModelIndex currIndex = ui->receiverTableView->currentIndex();
    QModelIndex fileNameIndex = mReceiverModel->index(currIndex.row(), (int)TransferTableModel::Column::FileName);
    QString fileName = mReceiverModel->data(fileNameIndex).toString();

    QString str = "Are you sure wants to delete<p>" + fileName + "?";
    QMessageBox::StandardButton ret = QMessageBox::question(this, tr("Delete"), str);
    if (ret == QMessageBox::Yes) {
        QFile::remove(fileName);
        mReceiverModel->removeTransfer(currIndex.row());
    }
}

void Widget::onSelectedSenderStateChanged(TransferState state)
{
    ui->resumeSenderBtn->setEnabled(state == TransferState::Paused);
    ui->pauseSenderBtn->setEnabled(state == TransferState::Transfering || state == TransferState::Waiting);
    ui->cancelSenderBtn->setEnabled(state == TransferState::Transfering || state == TransferState::Waiting ||
                                    state == TransferState::Paused);
}

void Widget::onSelectedReceiverStateChanged(TransferState state)
{
    ui->resumeReceiverBtn->setEnabled(state == TransferState::Paused);
    ui->pauseReceiverBtn->setEnabled(state == TransferState::Transfering || state == TransferState::Waiting);
    ui->cancelReceiverBtn->setEnabled(state == TransferState::Transfering || state == TransferState::Waiting ||
                                    state == TransferState::Paused);
}

void Widget::setupToolbar()
{
    QMenu* sendMenu = new QMenu();
    sendMenu->addAction(mSendFilesAction);
    sendMenu->addAction(mSendFolderAction);

    ui->sendBtn->setPopupMode(QToolButton::InstantPopup);
    ui->sendBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->sendBtn->setMenu(sendMenu);

    ui->settingBtn->addAction(mSettingsAction);
    ui->settingBtn->setDefaultAction(mSettingsAction);
}

void Widget::setupActions()
{
    mSendFilesAction = new QAction(QIcon(":/img/img/file.png"), tr("Send files..."), this);
    connect(mSendFilesAction, &QAction::triggered, this, &Widget::onSendFilesActionTriggered);
    mSendFolderAction = new QAction(QIcon(":/img/img/folder.png"), tr("Send folders..."), this);
    connect(mSendFolderAction, &QAction::triggered, this, &Widget::onSendFolderActionTriggered);
    mSettingsAction = new QAction(QIcon(":/img/img/settings.png"), tr("Settings"), this);
    connect(mSettingsAction, &QAction::triggered, this, &Widget::onSettingsActionTriggered);
    mAboutAction = new QAction(QIcon(":/img/img/about.png"), tr("About"), this);
    mAboutAction->setMenuRole(QAction::AboutRole);
    connect(mAboutAction, &QAction::triggered, this, &Widget::onAboutActionTriggered);
    mAboutQtAction = new QAction(tr("About Qt"), this);
    mAboutQtAction->setMenuRole(QAction::AboutQtRole);
    connect(mAboutQtAction, &QAction::triggered, QApplication::instance(), &QApplication::aboutQt);

    mSenderOpenAction = new QAction(tr("Open"), this);
    connect(mSenderOpenAction, &QAction::triggered, this, &Widget::openSenderFileInCurrentIndex);
    mSenderOpenFolderAction = new QAction(tr("Open folder"), this);
    connect(mSenderOpenFolderAction, &QAction::triggered, this, &Widget::openSenderFolderInCurrentIndex);
    mSenderRemoveAction = new QAction(QIcon(":/img/img/remove.png"), tr("Remove"), this);
    connect(mSenderRemoveAction, &QAction::triggered, this, &Widget::removeSenderItemInCurrentIndex);
    mSenderClearAction = new QAction(QIcon(":/img/img/clear.png"), tr("Clear"), this);
    connect(mSenderClearAction, &QAction::triggered, this, &Widget::onSenderClearClicked);
    mSenderPauseAction = new QAction(QIcon(":/img/img/pause.png"), tr("Pause"), this);
    connect(mSenderPauseAction, &QAction::triggered, this, &Widget::onSenderPauseClicked);
    mSenderResumeAction = new QAction(QIcon(":/img/img/resume.png"), tr("Resume"), this);
    connect(mSenderResumeAction, &QAction::triggered, this, &Widget::onSenderResumeClicked);
    mSenderCancelAction = new QAction(QIcon(":/img/img/cancel.png"), tr("Cancel"), this);
    connect(mSenderCancelAction, &QAction::triggered, this, &Widget::onSenderCancelClicked);

    mRecOpenAction = new QAction(tr("Open"), this);
    connect(mRecOpenAction, &QAction::triggered, this, &Widget::openReceiverFileInCurrentIndex);
    mRecOpenFolderAction = new QAction(tr("Open folder"), this);
    connect(mRecOpenFolderAction, &QAction::triggered, this, &Widget::openReceiverFolderInCurrentIndex);
    mRecRemoveAction = new QAction(QIcon(":/img/img/remove.png"), tr("Remove"), this);
    connect(mRecRemoveAction, &QAction::triggered, this, &Widget::removeReceiverItemInCurrentIndex);
    mRecDeleteAction = new QAction(tr("Delete from disk"), this);
    connect(mRecDeleteAction, &QAction::triggered, this, &Widget::deleteReceiverFileInCurrentIndex);
    mRecClearAction = new QAction(QIcon(":/img/img/clear.png"), tr("Clear"), this);
    connect(mRecClearAction, &QAction::triggered, this, &Widget::onReceiverClearClicked);
    mRecPauseAction = new QAction(QIcon(":/img/img/pause.png"), tr("Pause"), this);
    connect(mRecPauseAction, &QAction::triggered, this, &Widget::onReceiverPauseClicked);
    mRecResumeAction = new QAction(QIcon(":/img/img/resume.png"), tr("Resume"), this);
    connect(mRecResumeAction, &QAction::triggered, this, &Widget::onReceiverResumeClicked);
    mRecCancelAction = new QAction(QIcon(":/img/img/cancel.png"), tr("Cancel"), this);
    connect(mRecCancelAction, &QAction::triggered, this, &Widget::onReceiverCancelClicked);
}
