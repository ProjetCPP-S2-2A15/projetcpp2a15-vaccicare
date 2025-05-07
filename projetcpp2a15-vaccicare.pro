QT       += core gui printsupport
QT       += sql charts network multimedia multimediawidgets network serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17   #•kenet 11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Design.cpp \
    addressource.cpp \
    arduino.cpp \
    calendrierdialog.cpp \
    chatbot.cpp \
    connection.cpp \
    date.cpp \
    dialogchoixtypeimportpdf.cpp \
    dialogformpatient.cpp \
    dialogchoixstock.cpp \
    dialoglistepatient.cpp \
    dialoglistemedecin.cpp\
    dialoglisteressources.cpp \
    experience.cpp \
    ficheepreience.cpp \
    ficheprojet.cpp \
    fichemedecin.cpp\
    ficheressource.cpp \
    fichevaccin.cpp \
    historiquevaccindialog.cpp \
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
    smtp.cpp \
    vaccin.cpp

HEADERS += \
    Design.h \
    addressource.h \
    arduino.h \
    calendrierdialog.h \
    chatbot.h \
    connection.h \
    date.h \
    dialogchoixtypeimportpdf.h \
    dialogformpatient.h \
    dialogchoixstock.h \
    dialoglistepatient.h \
    dialoglistemedecin.h\
    dialoglisteressources.h \
    experience.h \
    ficheepreience.h \
    ficheprojet.h \
    fichemedecin.h\
    ficheressource.h \
    fichevaccin.h \
    historiquevaccindialog.h \
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
    smtp.h \
    vaccin.h

FORMS += \
    addressource.ui \
    calendrierdialog.ui \
    chatbot.ui \
    dialogchoixtypeimportpdf.ui \
    dialogformpatient.ui \
    dialogchoixstock.ui \
    dialoglistepatient.ui \
    dialoglistemedecin.ui\
    dialoglisteressources.ui \
    ficheepreience.ui \
    ficheprojet.ui \
    fichemedecin.ui\
    ficheressource.ui \
    fichevaccin.ui \
    historiquevaccindialog.ui \
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

RESOURCES += resources.qrc
