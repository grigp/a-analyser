#-------------------------------------------------
#
# Project created by QtCreator 2019-04-16T11:24:01
#
#-------------------------------------------------

QMAKE_EXTRA_TARGETS += before_build makefilehook

QT       += core gui serialport multimedia printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = a-analyzer
CONFIG += c++11
TEMPLATE = app

# каталог, в котором будет располагаться результирующий исполняемый файл
DESTDIR = $$OUT_PWD/bin

MOC_DIR = moc
OBJECTS_DIR = obj
RCC_DIR = rcc
UI_DIR = uic

TRANSLATIONS += a-analyser_en_US.ts #a-analyser_de_DE.ts

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(build/main/main_src.pri)
include(client_widgets/client_widgets_src.pri)
include(kernel/kernel_src.pri)
include(metodics/metodics_src.pri)
include(metodics_common/metodics_common_src.pri)
include(drivers/drivers_src.pri)
include(service/service_src.pri)
include(visual_widgets/visual_widgets_src.pri)
include(factors/factors_src.pri)
include(signal_analysis/signal_analysis_src.pri)
include(summaries/summaries_src.pri)

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    aanalyserapplication.cpp \
    settingsprovider.cpp \
    aanalysersettings.cpp \
    aboutdialog.cpp \
    userinfodialog.cpp

HEADERS += \
        mainwindow.h \
    aanalyserapplication.h \
    settingsprovider.h \
    aanalysersettings.h \
    aboutdialog.h \
    userinfodialog.h

FORMS += \
        mainwindow.ui \
    aanalysersettings.ui \
    aboutdialog.ui \
    userinfodialog.ui

RESOURCES += \
    kernel.qrc

win32:RC_FILE = a-analyser.rc
