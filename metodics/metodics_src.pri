INCLUDEPATH += $$PWD

include(stabtest/stabtest_src.pri)
include(jump_plate/jump_plate_src.pri)
include(take_put/take_put_src.pri)
include(tetris/tetris_src.pri)
include(extendtest/extendtest_src.pri)
include(dynamic_tests/dynamic_tests_src.pri)

HEADERS += \
    $$PWD/metodictemplate.h \
    $$PWD/metodicsfactory.h \
    $$PWD/metodicdefines.h \
    $$PWD/testcalculator.h \
    $$PWD/graphiccommon.h

SOURCES += \
    $$PWD/metodictemplate.cpp \
    $$PWD/metodicsfactory.cpp \
    $$PWD/testcalculator.cpp \
    $$PWD/graphiccommon.cpp

FORMS += \

