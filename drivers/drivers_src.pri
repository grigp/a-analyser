INCLUDEPATH += $$PWD

include(stabilan/drv_stabilan_src.pri)
include(jump_plate/drv_jumpplate_src.pri)

HEADERS += \
    $$PWD/driversfactory.h \
    $$PWD/driver.h \
    $$PWD/deviceprotocols.h \
    $$PWD/connection.h \
    $$PWD/devicecontroldialog.h \
    $$PWD/addconnectiondialog.h

SOURCES += \
    $$PWD/driversfactory.cpp \
    $$PWD/driver.cpp \
    $$PWD/deviceprotocols.cpp \
    $$PWD/connection.cpp \
    $$PWD/devicecontroldialog.cpp \
    $$PWD/addconnectiondialog.cpp

FORMS += \
    $$PWD/devicecontroldialog.ui \
    $$PWD/addconnectiondialog.ui

