INCLUDEPATH += $$PWD

include(patient_win/patient_win_src.pri)
include(calculators/calculators_src.pri)
include(visualizators/visualizators_src.pri)

HEADERS += \
    $$PWD/stabtesttemplate.h \
    $$PWD/stabtestexecute.h \
    $$PWD/stabtestvisualize.h \
    $$PWD/stabtestparamsdialog.h \
    $$PWD/stabtestparams.h \
    $$PWD/linesparamsdialog.h \
    $$PWD/stabtestdefines.h

SOURCES += \
    $$PWD/stabtesttemplate.cpp \
    $$PWD/stabtestexecute.cpp \
    $$PWD/stabtestvisualize.cpp \
    $$PWD/stabtestparamsdialog.cpp \
    $$PWD/stabtestparams.cpp \
    $$PWD/linesparamsdialog.cpp

FORMS += \
    $$PWD/stabtestexecute.ui \
    $$PWD/stabtestvisualize.ui \
    $$PWD/stabtestparamsdialog.ui \
    $$PWD/linesparamsdialog.ui

