INCLUDEPATH += $$PWD

include(devices/devices_src.pri)
include(ports/ports_src.pri)

HEADERS += \
    $$PWD/driversfactory.h \
    $$PWD/driver.h \
    $$PWD/deviceprotocols.h \
    $$PWD/connection.h \
    $$PWD/devicecontroldialog.h \
    $$PWD/addconnectiondialog.h \
    $$PWD/driverdefines.h

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

