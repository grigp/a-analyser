INCLUDEPATH += $$PWD

include(take_put/take_put_src.pri)
include(tetris/tetris_src.pri)

HEADERS += \
    $$PWD/trenexecute.h \
    $$PWD/trenagerpatientwindow.h \
    $$PWD/trenstabexecute.h \
    $$PWD/trentestcalculator.h \
    $$PWD/trenvisualize.h

SOURCES += \
    $$PWD/trenexecute.cpp \
    $$PWD/trenagerpatientwindow.cpp \
    $$PWD/trenstabexecute.cpp \
    $$PWD/trentestcalculator.cpp \
    $$PWD/trenvisualize.cpp

FORMS += \
    $$PWD/trenexecute.ui \
    $$PWD/trenagerpatientwindow.ui \
    $$PWD/trenstabexecute.ui \
    $$PWD/trenvisualize.ui

