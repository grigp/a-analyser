INCLUDEPATH += $$PWD

include(dialogs/dialogs_src.pri)
include(visuals/visuals_dopusk_src.pri)

HEADERS += \
    $$PWD/personalprogrammanager.h \
    $$PWD/personalprogramdefines.h \
    $$PWD/personalprogram.h \
    $$PWD/patientprogramwidget.h \
    $$PWD/patientprogramvisualswidget.h \
    $$PWD/personalprogramservice.h

SOURCES += \
    $$PWD/personalprogrammanager.cpp \
    $$PWD/personalprogram.cpp \
    $$PWD/patientprogramwidget.cpp \
    $$PWD/patientprogramvisualswidget.cpp \
    $$PWD/personalprogramservice.cpp

FORMS += \
    $$PWD/patientprogramwidget.ui \
    $$PWD/patientprogramvisualswidget.ui

