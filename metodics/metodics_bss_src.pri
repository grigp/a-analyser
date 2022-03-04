INCLUDEPATH += $$PWD

include(bedside_scales/bedside_scales_src.pri)

HEADERS += \
    $$PWD/metodictemplate.h \
    $$PWD/metodicsfactory.h \
    $$PWD/metodicdefines.h \
    $$PWD/testcalculator.h

SOURCES += \
    $$PWD/metodictemplate.cpp \
    $$PWD/metodicsfactory.cpp \
    $$PWD/testcalculator.cpp

FORMS += \

