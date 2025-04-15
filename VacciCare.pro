QT       += core gui sql charts multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32 {
WINDOWS_SDK_VERSION = 10.0.19041.0
INCLUDEPATH += "C:/Program Files (x86)/Windows Kits/10/Include/$$WINDOWS_SDK_VERSION/um"
 INCLUDEPATH += "C:/Program Files (x86)/Windows Kits/10/Include/$$(WindowsSDKVersion)/shared"

# Exclude ATL-dependent headers
DEFINES += _ATL_DISABLE_NO_WARNINGS
DEFINES += _SAPI_BUILD_VER=0x053

# Required libraries
    LIBS += -lsapi -lole32 -loleaut32 -luuid -lrpcrt4

    # MinGW compatibility
    DEFINES += WIN32_LEAN_AND_MEAN
    DEFINES += NOMINMAX
    DEFINES += INITGUID  # This ensures GUIDs are defined properly
    QMAKE_LFLAGS += -Wl,--enable-stdcall-fixup
}

SOURCES += \
    connection.cpp \
    date.cpp \
    exportdialog.cpp \
    ficheprojet.cpp \
    main.cpp \
    mainwindow.cpp \
    projet.cpp \
    speechrecognizer.cpp \
    statsscreen.cpp

HEADERS += \
    connection.h \
    date.h \
    exportdialog.h \
    ficheprojet.h \
    mainwindow.h \
    minimal_sapi.h \
    projet.h \
    sapi_guids.h \
    speechrecognizer.h \
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
