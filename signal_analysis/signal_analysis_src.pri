INCLUDEPATH += $$PWD

include(visuals/visuals_src.pri)

HEADERS += \
    $$PWD/visualsfactory.h \
    $$PWD/visuals.h \
    $$PWD/visual.h

SOURCES += \
    $$PWD/visualsfactory.cpp \
    $$PWD/visuals.cpp \
    $$PWD/visual.cpp

FORMS +=
