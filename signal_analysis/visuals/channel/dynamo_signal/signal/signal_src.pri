INCLUDEPATH += $$PWD

include(balistogram/balistogram_src.pri)

HEADERS += \
    $$PWD/dynamosignalvisual.h \
    $$PWD/dynamosignalvisualwidget.h

SOURCES += \
    $$PWD/dynamosignalvisual.cpp \
    $$PWD/dynamosignalvisualwidget.cpp

FORMS += \
    $$PWD/dynamosignalvisualwidget.ui
