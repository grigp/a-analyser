INCLUDEPATH += $$PWD

include(classic/classic_src.pri)
include(target/target_src.pri)
include(vector/vector_src.pri)
include(ids/ids_src.pri)
include(spectr_stab/spectr_stab_src.pri)
include(spectr_single_sig/spectr_single_sig_src.pri)
include(ratio_prb/ratio_prb_src.pri)
include(jump/jump_src.pri)

HEADERS += \
    $$PWD/factorsdefines.h \
    $$PWD/factorsfactory.h \
    $$PWD/multifactor.h \
    $$PWD/multifactordescriptor.h

SOURCES += \
    $$PWD/factorsfactory.cpp \
    $$PWD/multifactor.cpp \
    $$PWD/multifactordescriptor.cpp

FORMS += \

