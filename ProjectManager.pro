QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    client.cpp \
    connection.cpp \
    database.cpp \
    loginform.cpp \
    main.cpp \
    model/device.cpp \
    model/devicelistmodel.cpp \
    model/transferinfo.cpp \
    model/transfertablemodel.cpp \
    peermanager.cpp \
    project.cpp \
    server.cpp \
    settings.cpp \
    singleinstance.cpp \
    task.cpp \
    transfer/devicebroadcaster.cpp \
    transfer/receiver.cpp \
    transfer/sender.cpp \
    transfer/transfer.cpp \
    transfer/transferserver.cpp \
    ui/aboutdialog.cpp \
    ui/mainwindow.cpp \
    ui/receiverselectordialog.cpp \
    ui/settingsdialog.cpp \
    util.cpp \
    widget.cpp

HEADERS += \
    client.h \
    connection.h \
    database.h \
    loginform.h \
    model/device.h \
    model/devicelistmodel.h \
    model/transferinfo.h \
    model/transfertablemodel.h \
    peermanager.h \
    project.h \
    server.h \
    settings.h \
    singleinstance.h \
    task.h \
    transfer/devicebroadcaster.h \
    transfer/receiver.h \
    transfer/sender.h \
    transfer/transfer.h \
    transfer/transferserver.h \
    ui/aboutdialog.h \
    ui/mainwindow.h \
    ui/receiverselectordialog.h \
    ui/settingsdialog.h \
    util.h \
    widget.h

FORMS += \
    loginform.ui \
    project.ui \
    task.ui \
    ui/aboutdialog.ui \
    ui/mainwindow.ui \
    ui/receiverselectordialog.ui \
    ui/settingsdialog.ui \
    widget.ui

TRANSLATIONS += \
    ProjectManager_vi_VN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
requires(qtConfig(udpsocket))
requires(qtConfig(listwidget))

RESOURCES += \
    img.qrc \
    img.qrc \
    res.qrc \
    res.qrc

DISTFILES += \
    ProjectManager.pro.user \
    README.md \
    database.db \
    text/credits.html \
    text/gpl-3.0.txt
