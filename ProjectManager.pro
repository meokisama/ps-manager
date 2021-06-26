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
    peermanager.cpp \
    project.cpp \
    server.cpp \
    task.cpp \
    widget.cpp

HEADERS += \
    client.h \
    connection.h \
    database.h \
    loginform.h \
    peermanager.h \
    project.h \
    server.h \
    task.h \
    widget.h

FORMS += \
    loginform.ui \
    project.ui \
    task.ui \
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
    img.qrc

DISTFILES += \
    database.db
