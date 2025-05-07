QT       += core gui printsupport
QT       += sql charts network multimedia multimediawidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17   #•kenet 11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Design.cpp \
    addressource.cpp \
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
    pdfscanner.cpp \
    projectworkflowdialog.cpp \
    projet.cpp \
    resources.cpp \
    selectprojectdialog.cpp \
    smtp.cpp \
    vaccin.cpp

HEADERS += \
    Design.h \
    addressource.h \
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
    pdfscanner.h \
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

DISTFILES += \
    popler/bin/Lerc.dll \
    popler/bin/cairo.dll \
    popler/bin/charset.dll \
    popler/bin/deflate.dll \
    popler/bin/expat.dll \
    popler/bin/fontconfig-1.dll \
    popler/bin/freetype.dll \
    popler/bin/iconv.dll \
    popler/bin/jpeg8.dll \
    popler/bin/lcms2.dll \
    popler/bin/libcrypto-3-x64.dll \
    popler/bin/libcurl.dll \
    popler/bin/libexpat.dll \
    popler/bin/liblzma.dll \
    popler/bin/libpng16.dll \
    popler/bin/libssh2.dll \
    popler/bin/libtiff.dll \
    popler/bin/libzstd.dll \
    popler/bin/openjp2.dll \
    popler/bin/pdfattach.exe \
    popler/bin/pdfdetach.exe \
    popler/bin/pdffonts.exe \
    popler/bin/pdfimages.exe \
    popler/bin/pdfinfo.exe \
    popler/bin/pdfseparate.exe \
    popler/bin/pdftocairo.exe \
    popler/bin/pdftohtml.exe \
    popler/bin/pdftoppm.exe \
    popler/bin/pdftops.exe \
    popler/bin/pdftotext.exe \
    popler/bin/pdfunite.exe \
    popler/bin/pixman-1-0.dll \
    popler/bin/poppler-cpp.dll \
    popler/bin/poppler-glib.dll \
    popler/bin/poppler.dll \
    popler/bin/tiff.dll \
    popler/bin/zlib.dll \
    popler/bin/zstd.dll \
    popler/bin/zstd.exe
