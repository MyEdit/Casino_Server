QT -= gui
QT += sql

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Commands/commandban.cpp \
    Commands/commandlist.cpp \
    Commands/commandnotification.cpp \
    Database/databasemanager.cpp \
    Games/game.cpp \
    Games/table.cpp \
    Network/PacketsActions/p_authorization.cpp \
    Network/PacketsActions/p_notification.cpp \
    Network/PacketsActions/p_query.cpp \
    Network/PacketsActions/p_querywithoutresponce.cpp \
    Network/PacketsActions/p_reconnection.cpp \
    Network/PacketsActions/p_sendmodel.cpp \
    Network/PacketsActions/p_sendtables.cpp \
    Network/networkserver.cpp \
    Users/player.cpp \
    Users/stuffuser.cpp \
    Utils/serializer.cpp \
    Commands/commandmanager.cpp \
    Commands/commandhelp.cpp \
    main.cpp \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -lws2_32

HEADERS += \
    Commands/commandban.h \
    Commands/commandlist.h \
    Commands/commandnotification.h \
    Constants.h \
    Database/databasemanager.h \
    Games/game.h \
    Games/table.h \
    Network/PacketTypes.h \
    Network/PacketsActions/p_authorization.h \
    Network/PacketsActions/p_notification.h \
    Network/PacketsActions/p_query.h \
    Network/PacketsActions/p_querywithoutresponce.h \
    Network/PacketsActions/p_reconnection.h \
    Network/PacketsActions/p_sendmodel.h \
    Network/PacketsActions/p_sendtables.h \
    Network/networkserver.h \
    Users/player.h \
    Users/stuffuser.h \
    Users/user.h \
    Utils/Message.h \
    Utils/serializer.h \
    Commands/commandmanager.h \
    Commands/command.h \
    Commands/commandhelp.h \
