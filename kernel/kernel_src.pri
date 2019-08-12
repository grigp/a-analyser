INCLUDEPATH += $$PWD

include(database/database_src.pri)
include(results_data/results_data_src.pri)

HEADERS += \
    $$PWD/baseutils.h \
    $$PWD/exitcodes.h \
    $$PWD/log.h

SOURCES += \
    $$PWD/baseutils.cpp \
    $$PWD/log.cpp

FORMS += \
