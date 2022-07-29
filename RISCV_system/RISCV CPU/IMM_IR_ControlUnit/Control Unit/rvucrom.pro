# QT       += core gui
QT -= gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#CONFIG += c++11
#CONFIG -= c++17
CONFIG += gnu++17

QMAKE_LIBS += C:\bin\SystemC\lib\libsystemc.a
QMAKE_INCDIR += C:\bin\SystemC\include

QMAKE_LIBS += C:\bin\boost_1_79_0\stage\lib\libboost_program_options-mgw8-mt-d-x64-1_79.a
QMAKE_INCDIR += C:\bin\boost_1_79_0

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    configengine.cpp \
    main.cpp \
    xmlucrom.cpp

HEADERS += \
    configengine.h \
    xmlucrom.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
