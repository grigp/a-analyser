INCLUDEPATH += $$PWD

include(database/database_src.pri)
include(results_data/results_data_src.pri)

HEADERS += \
    $$PWD/baseutils.h

SOURCES += \
    $$PWD/baseutils.cpp

FORMS += \
