
QT       += core gui sql
QT += charts sql
QT += printsupport



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    StatTvaccin.cpp \
    connection.cpp \
    date.cpp \
    design.cpp \
    main.cpp \
    mainwindow.cpp \
    pdf_scanner.cpp \
    vaccin.cpp

HEADERS += \
    StatTvaccin.h \
    connection.h \
    date.h \
    design.h \
    mainwindow.h \
    pdf_scanner.h \
    vaccin.h

FORMS += \
   mainwindow.ui



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
