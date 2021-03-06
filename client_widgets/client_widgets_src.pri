INCLUDEPATH += $$PWD

include(database_widgets/database_widgets_src.pri)

HEADERS += \
    $$PWD/clientwidget.h \
    $$PWD/databaseresultwidget.h \
    $$PWD/executewidget.h

SOURCES += \
    $$PWD/clientwidget.cpp \
    $$PWD/databaseresultwidget.cpp \
    $$PWD/executewidget.cpp

FORMS += \
    $$PWD/clientwidget.ui \
    $$PWD/databaseresultwidget.ui \
    $$PWD/executewidget.ui

