QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    ndarray.cpp \
    odesolver.cpp \
    predefinedfunc.cpp \
    qcustomplot.cpp \
    runtimedll.cpp

HEADERS += \
    mainwindow.h \
    ndarray.h \
    odesolver.h \
    predefinedfunc.h \
    qcustomplot.h \
    runtimedll.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
