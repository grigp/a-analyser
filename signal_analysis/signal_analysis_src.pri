INCLUDEPATH += $$PWD

include(visuals/visuals_src.pri)

HEADERS += \
    $$PWD/visualsfactory.h \
    $$PWD/visualdescriptor.h \
    $$PWD/visuals.h \
    $$PWD/teststreemodel.h \
    $$PWD/visualdefines.h

SOURCES += \
    $$PWD/visualsfactory.cpp \
    $$PWD/visualdescriptor.cpp \
    $$PWD/visuals.cpp \
    $$PWD/teststreemodel.cpp

FORMS +=
