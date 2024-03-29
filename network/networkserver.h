﻿#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <winsock2.h> // [для себя] для работы нужно добавить LIBS += -lws2_32 в .pro файл
#include <thread>
#include <Ws2tcpip.h>
#include <QString>
#include "Constants.h"
#include <QMutex>
#include <QSharedPointer>
#include "Network/PacketTypes.h"
#include "Network/PacketsActions/p_authorization.h"
#include "Network/PacketsActions/p_sendmodel.h"
#include "Network/PacketsActions/p_query.h"
#include "Network/PacketsActions/p_querywithoutresponce.h"
#include "Network/PacketsActions/p_reconnection.h"
#include "Utils/Message.h"

class NetworkServer
{
    SOCKADDR_IN serverAddress;
    SOCKET serverSocket;
    int sizeofaddr;

private:
    void configuration();
    static void packetHandler(PacketTypes packettype, QSharedPointer<SOCKET> clientSocket);
    static void clientHandler(QSharedPointer<SOCKET> clientSocket);
    static void addConnect(QSharedPointer<SOCKET> clientSocket, QString login);
    static QString getIPAdress(QSharedPointer<SOCKET> client);

public:
    bool init();
    void startListening();
    static void sendToClient(QSharedPointer<SOCKET> client, QString message);
    static void sendToAllClient(QString message);
    static void onClientDisconnected(QSharedPointer<SOCKET> client);
    static QString getNickname(QSharedPointer<SOCKET> clientSocket);
    static QString getMessageFromClient(QSharedPointer<SOCKET> clientSocket);
    static QSharedPointer<SOCKET> getSocketByNickname(QString nickname);
    static QList<QString> getOnlineUsers();

    template<typename T>
    static void sendToClient(QSharedPointer<SOCKET> client, const T data, int size)
    {
        send(*client, reinterpret_cast<const char*>(data), size, 0);
    }

    friend class P_Authorization;
    friend class P_SendModel;
    friend class P_Query;
    friend class P_Reconnection;
};

#endif // NETWORKSERVER_H
