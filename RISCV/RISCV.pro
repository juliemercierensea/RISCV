QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

CONFIG -= c++17
CONFIG += gnu++17

QMAKE_LIBS += C:\Users\jmerc\Documents\RiscV\SystemC\lib\libsystemc.a
QMAKE_INCDIR += C:\Users\jmerc\Documents\RiscV\SystemC\include


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        test.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ALU.h \
    AMBA_Slave1_dataMEM.h \
    AMBA_SlaveDecoder.h \
    AMBA_Slave_InstructionMem.h \
    AMBA_master.h \
    AMBA_selbits.h \
    AddressCounter.h \
    IHex.h \
    IMMEDIATE.h \
    IR.h \
    InstrNumber.h \
    Instruction.h \
    MI_AND.h \
    MI_CPUside.h \
    MI_IMEM_contents.h \
    MI_ORbytestrb.h \
    MI_ORtrigger.h \
    MI_StateMachine.h \
    MI_StateOutput.h \
    MI_byte_extend.h \
    MI_half_extend.h \
    MI_plus_PADDR.h \
    MI_registre.h \
    MI_select_bits.h \
    MI_shift_left.h \
    MI_shift_right.h \
    MI_size_strobes.h \
    MI_zeroextend.h \
    PC.h \
    ROMData_to_input.h \
    ROMmem.h \
    Regfile.h \
    SelRI_value.h \
    TargetAdd.h \
    mux2to1.h \
    mux3to1.h \
    mux4to1.h
