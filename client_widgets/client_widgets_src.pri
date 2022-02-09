INCLUDEPATH += $$PWD

include(database_widgets/database_widgets_src.pri)

HEADERS += \
    $$PWD/clientwidget.h \
    $$PWD/databaseresultwidget.h \
    $$PWD/executewidget.h \
    $$PWD/signalanalysiswidget.h

SOURCES += \
    $$PWD/clientwidget.cpp \
    $$PWD/databaseresultwidget.cpp \
    $$PWD/executewidget.cpp \
    $$PWD/signalanalysiswidget.cpp

FORMS += \
    $$PWD/clientwidget.ui \
    $$PWD/databaseresultwidget.ui \
    $$PWD/executewidget.ui \
    $$PWD/signalanalysiswidget.ui

