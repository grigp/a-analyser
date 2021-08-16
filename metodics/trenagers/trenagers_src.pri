INCLUDEPATH += $$PWD

include(take_put/take_put_src.pri)
include(tetris/tetris_src.pri)

HEADERS += \
    $$PWD/trenexecute.h \
    $$PWD/trenagerpatientwindow.h \
    $$PWD/trenstabexecute.h

SOURCES += \
    $$PWD/trenexecute.cpp \
    $$PWD/trenagerpatientwindow.cpp \
    $$PWD/trenstabexecute.cpp

FORMS += \
    $$PWD/trenexecute.ui \
    $$PWD/trenagerpatientwindow.ui \
    $$PWD/trenstabexecute.ui

