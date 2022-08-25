TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        db.cpp \
        main.cpp \
        myserver.cpp \
        tcpserver.cpp \
        tcpsoket.cpp \
        thread.cpp \
        user.cpp

HEADERS += \
    db.h \
    myserver.h \
    tcpserver.h \
    tcpsoket.h \
    thread.h \
    user.h

#jsoncpp
LIBS += -L/usr/local/lib -levent -lpthread -lmysqlclient -ljsoncpp
