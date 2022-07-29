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
        IHex.cpp \
        configengine.cpp \
        main.cpp \
        #test_IMEM.cpp \
        #test_IMMEDIATE.cpp \
        #test_ALIGNMENTregister.cpp \
        #test_MemInt.cpp \
        #test_ControlUnit.cpp \
        #test_RV_1.cpp \
        #test_B_PSTRB.cpp \
        #test_ALU.cpp \
        #test_B_PWRITE.cpp \
        #test_BPADDR.cpp \
        #test_PWDATA.cpp \
        #test_rdata_o.cpp \
        test_AMBA.cpp \
        #test_IMM_IR_CU.cpp \
        #test_RegFile.cpp \
        xmlucrom.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ALU.h \
    ALU_register.h \
    AMBA.h \
    AMBA_Slave1_dataMEM.h \
    AMBA_SlaveDecoder.h \
    AMBA_Slave_InstructionMem.h \
    AddressCounter.h \
    B_PADDR.h \
    B_PSTRB.h \
    B_PWDATA.h \
    B_PWRITE.h \
    B_STATEMACHINE.h \
    B_rdata_o.h \
    Control_Unit.h \
    IHex.h \
    IMMEDIATE.h \
    IMM_IR_CU.h \
    IR.h \
    InstrNumber.h \
    Instruction.h \
    MI_AND.h \
    MI_ORbytestrb.h \
    MI_ORtrigger.h \
    MI_StateMachine.h \
    MI_StateOutput.h \
    MI_byte_extend.h \
    MI_half_extend.h \
    MI_plus_PADDR.h \
    MI_registre.h \
    MI_shift_left.h \
    MI_shift_right.h \
    MI_sizestrobes.h \
    MI_zeroextend.h \
    Memory_Interface.h \
    PC.h \
    PCregister.h \
    ROMData_to_input.h \
    ROMmem.h \
    RV_1.h \
    Regfile.h \
    TargetAdd.h \
    configengine.h \
    fetching_size.h \
    iPC_Control.h \
    mux2to1.h \
    mux3to1.h \
    mux4to1.h \
    trace.h \
    xmlucrom.h
