#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <winsock2.h> // [Для себя] Для работы нужно добавить LIBS += -lws2_32 в .pro файл
#include <thread>
#include <Ws2tcpip.h>
#include <string>
#include <Constants.h>
#include <qmutex.h>
#include <QSharedPointer>
#include <qdebug.h>
#include <Network/PacketTypes.h>
#include <Network/PacketsActions/p_authorization.h>
#include <Utils/Message.h>

class NetworkServer
{
private:
    SOCKADDR_IN serverAddress;
    SOCKET serverSocket;
    int sizeofaddr;

    void configuration();
    static void packetHandler(PacketTypes packettype, QSharedPointer<SOCKET> clientSocket);
    static void onClientDisconnected(QSharedPointer<SOCKET> client);
    static void clientHandler(QSharedPointer<SOCKET> clientSocket);
    static void addConnect(QSharedPointer<SOCKET> clientSocket, QString login);
    static QString getIPAdress(QSharedPointer<SOCKET> client);
    static QString getNickname(QSharedPointer<SOCKET> clientSocket);
    static QString getMessageFromClient(QSharedPointer<SOCKET> clientSocket);

public:
    bool init();
    void startListening();
    static void sendToClient(QSharedPointer<SOCKET> client, QString message);
    static void sendToAllClient(QString message);

    template<typename T>
    void sendToClient(QSharedPointer<SOCKET> client, const T data, int size)
    {
        send(*client, (char*)data, size, 0);
    }

    friend class P_Authorization;
};

#endif // NETWORKSERVER_H
