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
        GeneralTest.cpp \
        IHex.cpp \
        main.cpp \
        test_StateMachine.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    IHex.h \
    IMEM.h \
    OR_bytestrb.h \
    State.h \
    StateOutput.h \
    mux2to1.h \
    mux3to1.h \
    mux4to1.h \
    register.h \
    selectbits.h \
    shiftleft.h \
    sizestrobes.h

DISTFILES += \
    test1.disasm \
    test1.hex
