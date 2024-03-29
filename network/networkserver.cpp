﻿#include "networkserver.h"

QMap<QSharedPointer<SOCKET>, QString> Conections{};
QMutex m_mutex{};

//Инициилизация
bool NetworkServer::init()
{
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1); //Запрашиваемая версия библиотеки winsock
    if (WSAStartup(DLLVersion, &wsaData) != 0) //Загрузка библиотеки winsock
    {
        Message::logError("WSA library failed to load!");
        return false;
    }
    configuration();
    return true;
}

//Заполняет информацию об адресе сервера
void NetworkServer::configuration()
{
    sizeofaddr = sizeof(serverAddress);
    serverAddress.sin_addr.s_addr = inet_addr(ADDRESS);
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_family = AF_INET; //Интернет протокол

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    bind(serverSocket, (SOCKADDR*)&serverAddress, sizeofaddr); //Привязываю адрес сокету
}

//Прослушивает порт
void NetworkServer::startListening()
{
    if (!serverAddress.sin_port)
    {
        Message::logError("Port listening failed to start");
        return;
    }

    listen(serverSocket, MAXCONNECTIONS); //Слушаем порт для ожидания коннекта с клиентом
    Message::logInfo("Listening port started successfully!");

    //Коннектим клиентов
    while (true)
    {
        if (Conections.size() >= MAXCONNECTIONS) //Ничего не делаем если достигнут максимум соединений
            continue;

        QSharedPointer<SOCKET> clientSocket(new SOCKET);
        *clientSocket = accept(serverSocket, (SOCKADDR*)&serverAddress, &sizeofaddr);

        if (clientSocket == 0)
        {
            Message::logWarn("The client was unable to connect to the server");
        }
        else
        {
            Message::logInfo("Client [" + getIPAdress(clientSocket) + "] connected");
            std::thread handler(clientHandler, clientSocket);
            handler.detach();
        }
    }
}

//В бесконечном цикле ожидает получения сообщения от клиента и вызывает packetHandler
void NetworkServer::clientHandler(QSharedPointer<SOCKET> clientSocket)
{
    PacketTypes packettype;
    while(true)
    {
        if (recv(*clientSocket, (char*)&packettype, sizeof(PacketTypes), 0) <= 0)
        {
            onClientDisconnected(clientSocket);
            return;
        }
        packetHandler(packettype, clientSocket);
    }
}

//По мере необходимости будет дополняться
void NetworkServer::packetHandler(PacketTypes packettype, QSharedPointer<SOCKET> clientSocket)
{
    switch(packettype)
    {
        case(PacketTypes::P_Authorization):
        {
            P_Authorization::authorizeClient(clientSocket);
            break;
        }
        case(PacketTypes::P_SendModel):
        {
            P_SendModel::getTypeModel(clientSocket);
            break;
        }
        case(PacketTypes::P_QueryWithoutResponce):
        {
            P_QueryWithoutResponce::executeQuery(clientSocket);
            break;
        }
        case(PacketTypes::P_Reconnection):
        {
            P_Reconnection::reconnectClient(clientSocket);
            break;
        }
        case(PacketTypes::P_ConnectPlayerToTable):
        {
            break;
        }
        case(PacketTypes::P_SendTables):
        {
            break;
        }
        case(PacketTypes::P_Query):
        {
            P_Query::getResultQuary(clientSocket);
            break;
        }
        default:
        {
            Message::logWarn("Client send unknown packettype");
            break;
        }
    }
}

//Отправка пакета подключенному клиенту
void NetworkServer::sendToClient(QSharedPointer<SOCKET> client, QString message)
{
    int message_size = message.toUtf8().size();
    send(*client, reinterpret_cast<const char*>(&message_size), sizeof(int), 0);
    send(*client, message.toUtf8().constData(), message_size, 0);
}

//Отправка пакета всем подключенным клиентам
void NetworkServer::sendToAllClient(QString message)
{
    QMap<QSharedPointer<SOCKET>, QString>::iterator it;
    for (it = Conections.begin(); it != Conections.end(); ++it)
    {
        QSharedPointer<SOCKET> client = it.key();
        sendToClient(client, message);
    }
}

//Парс и получение пакета от клиента с неизвестной длинной
QString NetworkServer::getMessageFromClient(QSharedPointer<SOCKET> clientSocket)
{
    int size;
    recv(*clientSocket, reinterpret_cast<char*>(&size), sizeof(int), 0);
    QByteArray buffer(size, 0);
    recv(*clientSocket, buffer.data(), size, 0);
    return QString(buffer);
}

//Возвращает IP адрес подключенного клиента
QString NetworkServer::getIPAdress(QSharedPointer<SOCKET> client)
{
    sockaddr_in clientAddress;
    int sizeofaddr = sizeof(clientAddress);
    ZeroMemory(&clientAddress, sizeof (clientAddress));
    getsockname(*client, (sockaddr*)&clientAddress, &sizeofaddr);
    return inet_ntoa((in_addr)clientAddress.sin_addr);
}

//Возвращает никнейм подключенного клиента
QString NetworkServer::getNickname(QSharedPointer<SOCKET> clientSocket)
{
    QMutexLocker locker(&m_mutex);
    return Conections[clientSocket];
}

QSharedPointer<SOCKET> NetworkServer::getSocketByNickname(QString nickname)
{
    QMutexLocker locker(&m_mutex);
    for (auto it = Conections.begin(); it != Conections.end(); ++it)
    {
        if (it.value() == nickname)
        {
            return it.key();
        }
    }
    return nullptr;
}

//Вызывается при дисконнекте клиента
void NetworkServer::onClientDisconnected(QSharedPointer<SOCKET> clientSocket)
{
    QMutexLocker locker(&m_mutex);
    Message::logInfo("Client [" + getIPAdress(clientSocket) + "] disconnected");
    closesocket(*clientSocket);
    Conections.remove(clientSocket);
}

//Добавляет клиента в мапу Conections
void NetworkServer::addConnect(QSharedPointer<SOCKET> clientSocket, QString login)
{
    QMutexLocker locker(&m_mutex);
    Conections.insert(clientSocket, login);
}

QList<QString> NetworkServer::getOnlineUsers()
{
    QMutexLocker locker(&m_mutex);
    return Conections.values();
}
