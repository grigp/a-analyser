INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/channelsutils.h \
    $$PWD/channelsdefines.h \
    $$PWD/signalsdefines.h

SOURCES += \
    $$PWD/channelsutils.cpp

FORMS += \

include(signals_data/signals_data_src.pri)
include(containers/containers_src.pri)
