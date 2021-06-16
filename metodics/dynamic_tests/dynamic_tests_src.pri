INCLUDEPATH += $$PWD

include(cross/cross_src.pri)
include(step_offset/step_offset_src.pri)

HEADERS += \
    $$PWD/stabdynamictestexecute.h \
    $$PWD/stabdynamictestpatientwindow.h

SOURCES += \
    $$PWD/stabdynamictestexecute.cpp \
    $$PWD/stabdynamictestpatientwindow.cpp

FORMS += \
    $$PWD/stabdynamictestexecute.ui \
    $$PWD/stabdynamictestpatientwindow.ui

