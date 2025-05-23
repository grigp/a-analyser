INCLUDEPATH += $$PWD

include(stabtest/stabtest_src.pri)
include(jump_plate/jump_plate_src.pri)
include(trenagers/trenagers_src.pri)
include(extendtest/extendtest_src.pri)
include(dynamic_tests/dynamic_tests_src.pri)
include(graphics_utils/graphics_utils_src.pri)
include(mas_wrestling/mas_wrestling_src.pri)

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

