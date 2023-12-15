INCLUDEPATH += $$PWD

include(dialogs/dialogs_src.pri)
include(visuals/visuals_src.pri)

HEADERS += \
    $$PWD/personalprogrammanager.h \
    $$PWD/personalprogramdefines.h \
    $$PWD/personalprogram.h \
    $$PWD/patientprogramwidget.h

SOURCES += \
    $$PWD/personalprogrammanager.cpp \
    $$PWD/personalprogram.cpp \
    $$PWD/patientprogramwidget.cpp

FORMS += \
    $$PWD/patientprogramwidget.ui

