INCLUDEPATH += $$PWD

include(classic/classic_src.pri)
include(mas_wrestling/mas_wrestling_src.pri)

HEADERS += \
    $$PWD/factorsdefines.h \
    $$PWD/factorsfactory.h \
    $$PWD/multifactor.h \
    $$PWD/multifactordescriptor.h

SOURCES += \
    $$PWD/factorsfactory.cpp \
    $$PWD/multifactor.cpp \
    $$PWD/multifactordescriptor.cpp

FORMS += \

