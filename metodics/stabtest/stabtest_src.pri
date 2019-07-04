INCLUDEPATH += $$PWD

include(patient_win/patient_win_src.pri)

HEADERS += \
    $$PWD/stabtesttemplate.h \
    $$PWD/stabtestexecute.h \
    $$PWD/stabtestvisualize.h \
    $$PWD/stabtestparamsdialog.h \
    $$PWD/stabtestparams.h

SOURCES += \
    $$PWD/stabtesttemplate.cpp \
    $$PWD/stabtestexecute.cpp \
    $$PWD/stabtestvisualize.cpp \
    $$PWD/stabtestparamsdialog.cpp

FORMS += \
    $$PWD/stabtestexecute.ui \
    $$PWD/stabtestvisualize.ui \
    $$PWD/stabtestparamsdialog.ui

