INCLUDEPATH += $$PWD

include(database/database_src.pri)
include(results_data/results_data_src.pri)
include(norms/norms_src.pri)

HEADERS += \
    $$PWD/baseutils.h \
    $$PWD/exitcodes.h \
    $$PWD/log.h \
    $$PWD/signalaccess.h \
    $$PWD/fragment.h \
    $$PWD/basedefines.h

SOURCES += \
    $$PWD/baseutils.cpp \
    $$PWD/log.cpp \
    $$PWD/signalaccess.cpp \
    $$PWD/fragment.cpp \
    $$PWD/basedefines.cpp

FORMS += \
