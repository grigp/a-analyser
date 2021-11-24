INCLUDEPATH += $$PWD

include(cross/cross_src.pri)
include(step_offset/step_offset_src.pri)
include(step_deviation/step_deviation_src.pri)
include(evolventa/evolventa_src.pri)
include(octaedron/octaedron_src.pri)
include(boxer_dodging/boxer_dodging_src.pri)

HEADERS += \
    $$PWD/stabdynamictestexecute.h \
    $$PWD/stabdynamictestpatientwindow.h

SOURCES += \
    $$PWD/stabdynamictestexecute.cpp \
    $$PWD/stabdynamictestpatientwindow.cpp

FORMS += \
    $$PWD/stabdynamictestexecute.ui \
    $$PWD/stabdynamictestpatientwindow.ui

