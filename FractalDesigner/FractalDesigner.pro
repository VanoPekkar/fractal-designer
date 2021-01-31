QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Lib/fparser4.5.2/fparser.cc \
    Lib/fparser4.5.2/fpoptimizer.cc \
    funcenter.cpp \
    main.cpp \
    mainscene.cpp \
    mainwindow.cpp \
    savewindow.cpp

HEADERS += \
    Lib/fparser4.5.2/extrasrc/fpaux.hh \
    Lib/fparser4.5.2/extrasrc/fptypes.hh \
    Lib/fparser4.5.2/fparser.hh \
    Lib/fparser4.5.2/fpconfig.hh \
    funcenter.h \
    mainscene.h \
    mainwindow.h \
    savewindow.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Lib/fparser4.5.2/extrasrc/fp_identifier_parser.inc \
    Lib/fparser4.5.2/extrasrc/fp_opcode_add.inc

