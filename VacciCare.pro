QT       += core gui sql charts multimedia texttospeech

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Design.cpp \
    connection.cpp \
    date.cpp \
    exportdialog.cpp \
    ficheprojet.cpp \
    main.cpp \
    mainwindow.cpp \
    projet.cpp \
    #speechrecognizer.cpp \
    statsscreen.cpp

HEADERS += \
    Design.h \
    connection.h \
    date.h \
    exportdialog.h \
    ficheprojet.h \
    mainwindow.h \
    minimal_sapi.h \
    projet.h \
    qt_win32_compat.h \
    sapi_guids.h \
    #speechrecognizer.h \
    statsscreen.h

FORMS += \
    exportdialog.ui \
    ficheprojet.ui \
    mainwindow.ui \
    statsscreen.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
