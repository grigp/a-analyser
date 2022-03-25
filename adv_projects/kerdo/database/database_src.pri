INCLUDEPATH += $$PWD

include(data_ui/data_ui_src.pri)

HEADERS += \
    $$PWD/database.h \
    $$PWD/dataprovider.h \
    $$PWD/datadefines.h

SOURCES += \ 
    $$PWD/database.cpp \
    $$PWD/dataprovider.cpp \
    $$PWD/datadefines.cpp

FORMS += \

