INCLUDEPATH += $$PWD

include(cross/cross_src.pri)

HEADERS += \
    $$PWD/stabdynamictestexecute.h \
    $$PWD/stabdynamictestpatientwindow.h

SOURCES += \
    $$PWD/stabdynamictestexecute.cpp \
    $$PWD/stabdynamictestpatientwindow.cpp

FORMS += \
    $$PWD/stabdynamictestexecute.ui \
    $$PWD/stabdynamictestpatientwindow.ui

