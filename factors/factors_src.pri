INCLUDEPATH += $$PWD

include(classic/classic_src.pri)
include(vector/vector_src.pri)

HEADERS += \
    $$PWD/factorsdefines.h \
    $$PWD/factorsfactory.h \
    $$PWD/multifactor.h

SOURCES += \
    $$PWD/factorsfactory.cpp \
    $$PWD/multifactor.cpp

FORMS += \
