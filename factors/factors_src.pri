INCLUDEPATH += $$PWD

include(classic/classic_src.pri)
include(vector/vector_src.pri)

HEADERS += \
    $$PWD/factorsdefines.h \
    $$PWD/factors.h \
    $$PWD/factorsfactory.h

SOURCES += \
    $$PWD/factors.cpp \
    $$PWD/factorsfactory.cpp

FORMS += \

