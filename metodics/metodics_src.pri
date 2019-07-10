INCLUDEPATH += $$PWD

include(stabtest/stabtest_src.pri)
include(jumptest/jumptest_src.pri)
include(teppingtest/teppingtest_src.pri)

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

