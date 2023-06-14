INCLUDEPATH += $$PWD

include(visuals/visuals_dopusk_src.pri)
include(transformers/transformers_src.pri)
include(misc/misc_src.pri)

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
