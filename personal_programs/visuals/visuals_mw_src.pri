INCLUDEPATH += $$PWD

include(visuals/visuals_mw_src.pri)

HEADERS += \
    $$PWD/ppvisualdescriptor.h \
    $$PWD/ppvisual.h \
    $$PWD/ppvisualsfactory.h

SOURCES += \
    $$PWD/ppvisualdescriptor.cpp \
    $$PWD/ppvisual.cpp \
    $$PWD/ppvisualsfactory.cpp

FORMS +=

