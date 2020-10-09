QT += core gui
QT += websockets
QT += printsupport


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TEMPLATE        = app
TARGET          = textedit
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    accountinterface.cpp \
    changepwd.cpp \
    client.cpp \
    crdt.cpp \
    inserturi.cpp \
    inserttitle.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    message.cpp \
    showuridialog.cpp \
    symbol.cpp \
    textedit.cpp \
    socketmanager.cpp

HEADERS += \
    accountinterface.h \
    changepwd.h \
    client.h \
    crdt.h \
    inserturi.h \
    inserttitle.h \
    loginwindow.h \
    mainwindow.h \
    message.h \
    showuridialog.h \
    symbol.h \
    textedit.h \
    socketmanager.h

FORMS += \
    accountinterface.ui \
    changepwd.ui \
    inserturi.ui \
    inserttitle.ui \
    loginwindow.ui \
    mainwindow.ui \
    showuridialog.ui

RESOURCES += textedit.qrc \
    images/Icons/Icons.qrc
build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

EXAMPLE_FILES = textedit.qdoc
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#unix|win32: LIBS += -lmongoc-1.0

DISTFILES += \
    images/Icons/icons8-cancel-24.png \
    images/Icons/icons8-insert-page-24.png \
    images/Icons/icons8-male-user-24.png \
    images/Icons/icons8-open-document-24.png \
    images/Icons/logo-icon.png \
    images/logo32.png \
    images/mac/editcopy.png \
    images/mac/editcut.png \
    images/mac/editpaste.png \
    images/mac/editredo.png \
    images/mac/editundo.png \
    images/mac/exportpdf.png \
    images/mac/filenew.png \
    images/mac/fileopen.png \
    images/mac/fileprint.png \
    images/mac/filesave.png \
    images/mac/textbold.png \
    images/mac/textcenter.png \
    images/mac/textitalic.png \
    images/mac/textjustify.png \
    images/mac/textleft.png \
    images/mac/textright.png \
    images/mac/textunder.png \
    images/mac/zoomin.png \
    images/mac/zoomout.png \
    images/win/editcopy.png \
    images/win/editcut.png \
    images/win/editpaste.png \
    images/win/editredo.png \
    images/win/editundo.png \
    images/win/exportpdf.png \
    images/win/filenew.png \
    images/win/fileopen.png \
    images/win/fileprint.png \
    images/win/filesave.png \
    images/win/textbold.png \
    images/win/textcenter.png \
    images/win/textitalic.png \
    images/win/textjustify.png \
    images/win/textleft.png \
    images/win/textright.png \
    images/win/textunder.png \
    images/win/zoomin.png \
    images/win/zoomout.png
