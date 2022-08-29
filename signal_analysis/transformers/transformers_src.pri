INCLUDEPATH += $$PWD

include(transformers/transformers_src.pri)

HEADERS += \
    $$PWD/signaltransformer.h \
    $$PWD/signaltransformfactory.h \
    $$PWD/signaltransformerdefines.h \
    $$PWD/signaltransformerparamswidget.h \
    $$PWD/selecttransformerdialog.h

SOURCES += \
    $$PWD/signaltransformer.cpp \
    $$PWD/signaltransformfactory.cpp \
    $$PWD/signaltransformerparamswidget.cpp \
    $$PWD/selecttransformerdialog.cpp

FORMS += \
    $$PWD/signaltransformerparamswidget.ui \
    $$PWD/selecttransformerdialog.ui
