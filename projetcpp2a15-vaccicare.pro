QT       += core gui printsupport
QT       += sql charts network multimedia multimediawidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17   #•kenet 11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Design.cpp \
    calendrierdialog.cpp \
    connection.cpp \
    date.cpp \
    dialogchoixstock.cpp \
    dialoglistepatient.cpp \
    dialogpatientin.cpp \
    experience.cpp \
    ficheepreience.cpp \
    ficheprojet.cpp \
    listeprojetdialog.cpp \
    listevaccindialog.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    medecin.cpp \
    patient.cpp \
    projectworkflowdialog.cpp \
    projet.cpp \
    resources.cpp \
    selectprojectdialog.cpp \
    vaccin.cpp

HEADERS += \
    Design.h \
    calendrierdialog.h \
    connection.h \
    date.h \
    dialogchoixstock.h \
    dialoglistepatient.h \
    dialogpatientin.h \
    experience.h \
    ficheepreience.h \
    ficheprojet.h \
    listeprojetdialog.h \
    listevaccindialog.h \
    logindialog.h \
    mainwindow.h \
    medecin.h \
    patient.h \
    projectworkflowdialog.h \
    projet.h \
    resources.h \
    selectprojectdialog.h \
    vaccin.h

FORMS += \
    calendrierdialog.ui \
    dialogchoixstock.ui \
    dialoglistepatient.ui \
    dialogpatientin.ui \
    ficheepreience.ui \
    ficheprojet.ui \
    listeprojetdialog.ui \
    listevaccindialog.ui \
    logindialog.ui \
    mainwindow.ui \
    projectworkflowdialog.ui \
    selectprojectdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
