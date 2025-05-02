QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17   #•kenet 11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Design.cpp \
    connection.cpp \
    date.cpp \
    ficheprojet.cpp \
    listeprojetdialog.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    medecin.cpp \
    patient.cpp \
    projet.cpp \
    resources.cpp \
    vaccin.cpp

HEADERS += \
    Design.h \
    connection.h \
    date.h \
    ficheprojet.h \
    listeprojetdialog.h \
    logindialog.h \
    mainwindow.h \
    medecin.h \
    patient.h \
    projet.h \
    resources.h \
    vaccin.h

FORMS += \
    ficheprojet.ui \
    listeprojetdialog.ui \
    logindialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
