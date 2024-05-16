#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <winsock2.h> // [для себя] для работы нужно добавить LIBS += -lws2_32 в .pro файл
#include <thread>
#include <Ws2tcpip.h>
#include <QString>
#include "Constants.h"
#include <QMutex>
#include <QSharedPointer>
#include <QtConcurrent/QtConcurrent>
#include "network/PacketTypes.h"
#include "network/PacketsActions/p_authorization.h"
#include "network/PacketsActions/p_sendmodel.h"
#include "network/PacketsActions/p_query.h"
#include "network/PacketsActions/p_querywithoutresponce.h"
#include "network/PacketsActions/p_reconnection.h"
#include "network/PacketsActions/p_sendtables.h"
#include "network/PacketsActions/p_connectplayertotable.h"
#include "network/PacketsActions/p_gamepacket.h"
#include "network/PacketsActions/p_search.h"
#include "Utils/Message.h"
#include "Users/user.h"

class NetworkServer
{
    SOCKADDR_IN serverAddress;
    SOCKET serverSocket;
    int sizeofaddr;
    static QMap<PacketTypes, std::function<void(QSharedPointer<SOCKET> clientSocket)>> packetHandlerFunction;
    static QMap<QSharedPointer<SOCKET>, QPair<QSharedPointer<User>, QSharedPointer<QMutex>>> Conections;
    static QMutex m_mutex;

private:
    void configuration();
    static void packetHandler(const PacketTypes packettype, QSharedPointer<SOCKET> clientSocket);
    static void clientHandler(QSharedPointer<SOCKET> clientSocket);
    static bool addConnect(QSharedPointer<SOCKET> clientSocket, QSharedPointer<User> user);
    static QString getIPAdress(QSharedPointer<SOCKET> client);
    void initPacketHandlerFunction();

public:
    bool init();
    void startListening();
    static void sendToClient(QSharedPointer<SOCKET> client, const QString& message);
    static void onClientDisconnected(QSharedPointer<SOCKET> client);
    static QString getNickname(QSharedPointer<SOCKET> clientSocket);
    static QSharedPointer<User> getUser(QSharedPointer<SOCKET> clientSocket);
    static QSharedPointer<SOCKET> getSocketUser(QSharedPointer<User> user);
    static QSharedPointer<QMutex> getClientMutex(QSharedPointer<SOCKET> clientSocket);
    static QString getMessageFromClient(QSharedPointer<SOCKET> clientSocket);
    static QSharedPointer<SOCKET> getSocketByNickname(const QString& nickname);
    static QList<QString> getOnlineUsers();
    static QList<QSharedPointer<SOCKET>> getAdminSockets();
    static QList<QSharedPointer<SOCKET>> getPlayerSockets();

    template<typename T>
    static void sendToClient(QSharedPointer<SOCKET> client, const T data, const int& size)
    {
        send(*client, reinterpret_cast<const char*>(data), size, 0);
    }

    template<typename T>
    static T getMessageFromClient(QSharedPointer<SOCKET> client)
    {
        T returnData{};
        recv(*client, reinterpret_cast<char*>(&returnData), sizeof(T), 0);
        return returnData;
    }

    template<typename T>
    static typename std::enable_if<std::is_same<T, QByteArray>::value, T>::type
    getMessageFromClient(QSharedPointer<SOCKET> client, int size)
    {
        QByteArray returnData(size, 0);
        recv(*client, returnData.data(), size, 0);
        return returnData;
    }

    friend class P_Authorization;
    friend class P_SendModel;
    friend class P_Query;
    friend class P_Reconnection;
    friend class P_ConnectPlayerToTable;
    friend class P_GamePacket;
};

#endif // NETWORKSERVER_H
