INCLUDEPATH += $$PWD

include(classic/classic_src.pri)
include(target/target_src.pri)
include(vector/vector_src.pri)
include(ids/ids_src.pri)
include(ratio_prb/ratio_prb_src.pri)
include(jump/jump_src.pri)
include(tren_result/tren_result_src.pri)
include(stab_dynamic/stab_dynamic_src.pri)

HEADERS += \
    $$PWD/factorsdefines.h \
    $$PWD/factorsfactory.h \
    $$PWD/multifactor.h

SOURCES += \
    $$PWD/factorsfactory.cpp \
    $$PWD/multifactor.cpp

FORMS += \

