INCLUDEPATH += $$PWD

include(filtres/filtres_src.pri)

HEADERS += \
    $$PWD/signalexporter.h \
    $$PWD/signalexportdialog.h \
    $$PWD/filenamewidget.h \
    $$PWD/signalexportdefines.h

SOURCES += \
    $$PWD/signalexporter.cpp \
    $$PWD/signalexportdialog.cpp \
    $$PWD/filenamewidget.cpp

FORMS += \
    $$PWD/signalexportdialog.ui \
    $$PWD/filenamewidget.ui
