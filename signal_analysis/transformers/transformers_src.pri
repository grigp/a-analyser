INCLUDEPATH += $$PWD

include(transformers/transformers_src.pri)

HEADERS += \
    $$PWD/signaltransformer.h \
    $$PWD/signaltransformfactory.h \
    $$PWD/signaltransformerdefines.h

SOURCES += \
    $$PWD/signaltransformer.cpp \
    $$PWD/signaltransformfactory.cpp

FORMS += \
