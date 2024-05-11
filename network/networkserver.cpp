#include "networkserver.h"

QMap<PacketTypes, std::function<void(QSharedPointer<SOCKET> clientSocket)>> NetworkServer::packetHandlerFunction;
QMap<QSharedPointer<SOCKET>, QSharedPointer<User>> Conections{};
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

    if(packetHandlerFunction.size() == 0)
        initPacketHandlerFunction();

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
            QtConcurrent::run(clientHandler, clientSocket);
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

void NetworkServer::initPacketHandlerFunction()
{
    packetHandlerFunction =
    {
        {PacketTypes::P_Authorization,          [&](QSharedPointer<SOCKET> clientSocket) {P_Authorization::authorizeClient(clientSocket);}},
        {PacketTypes::P_SendModel,              [&](QSharedPointer<SOCKET> clientSocket) {P_SendModel::getTypeModel(clientSocket);}},
        {PacketTypes::P_QueryWithoutResponce,   [&](QSharedPointer<SOCKET> clientSocket) {P_QueryWithoutResponce::executeQuery(clientSocket);}},
        {PacketTypes::P_Reconnection,           [&](QSharedPointer<SOCKET> clientSocket) {P_Reconnection::reconnectClient(clientSocket);}},
        {PacketTypes::P_ConnectPlayerToTable,   [&](QSharedPointer<SOCKET> clientSocket) {P_ConnectPlayerToTable::connectPlayerToTable(clientSocket);}},
        {PacketTypes::P_PlayerLeaveTable,       [&](QSharedPointer<SOCKET> clientSocket) {P_ConnectPlayerToTable::playerLeaveTable(clientSocket);}},
        {PacketTypes::P_SendTables,             [&](QSharedPointer<SOCKET> clientSocket) {P_SendTables::sendTables(clientSocket);}},
        {PacketTypes::P_Query,                  [&](QSharedPointer<SOCKET> clientSocket) {P_Query::getResultQuary(clientSocket);}},
        {PacketTypes::P_GamePacket,             [&](QSharedPointer<SOCKET> clientSocket) {P_GamePacket::onGamePacketReceived(clientSocket);}},
        {PacketTypes::P_Search,                 [&](QSharedPointer<SOCKET> clientSocket) {P_Search::getSearchQuary(clientSocket);}},
    };
}

void NetworkServer::packetHandler(const PacketTypes packettype, QSharedPointer<SOCKET> clientSocket)
{
    if(packetHandlerFunction.contains(packettype))
        packetHandlerFunction[packettype](clientSocket);
    else
        Message::logWarn("Client send unknown packettype");
}

//Отправка пакета подключенному клиенту
void NetworkServer::sendToClient(QSharedPointer<SOCKET> client, const QString& message)
{
    int message_size = message.toUtf8().size();
    send(*client, reinterpret_cast<const char*>(&message_size), sizeof(int), 0);
    send(*client, message.toUtf8().constData(), message_size, 0);
}

//Отправка пакета всем подключенным клиентам
void NetworkServer::sendToAllClient(const QString& message)
{
    QMap<QSharedPointer<SOCKET>, QSharedPointer<User>>::iterator it;
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

    if(size == 0)
        return "";

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
    getpeername(*client, (sockaddr*)&clientAddress, &sizeofaddr);
    return inet_ntoa((in_addr)clientAddress.sin_addr);
}

//Возвращает никнейм подключенного клиента
QString NetworkServer::getNickname(QSharedPointer<SOCKET> clientSocket)
{
    QMutexLocker locker(&m_mutex);
    return Conections[clientSocket]->getLogin();
}

QSharedPointer<User> NetworkServer::getUser(QSharedPointer<SOCKET> clientSocket)
{
    QMutexLocker locker(&m_mutex);
    return Conections.value(clientSocket);
}

QSharedPointer<SOCKET> NetworkServer::getSocketUser(QSharedPointer<User> user)
{
    QMutexLocker locker(&m_mutex);
    for (auto it = Conections.keyValueBegin(); it != Conections.keyValueEnd(); ++it)
    {
        if(it.base().value()->getID() == user->getID())
            return it.base().key();
    }

    return nullptr;
}

QList<QSharedPointer<SOCKET>> NetworkServer::getAdminSockets()
{
    QList<QSharedPointer<SOCKET>> adminSockets;

    for (auto it = Conections.keyValueBegin(); it != Conections.keyValueEnd(); ++it)
    {
        if (it.base().value()->getRole() == Roles::Admin)
            adminSockets.append(it.base().key());
    }

    return adminSockets;
}

QSharedPointer<SOCKET> NetworkServer::getSocketByNickname(const QString& nickname)
{
    QMutexLocker locker(&m_mutex);
    for (auto it = Conections.begin(); it != Conections.end(); ++it)
    {
        if (it.value()->getLogin() == nickname)
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
bool NetworkServer::addConnect(QSharedPointer<SOCKET> clientSocket, QSharedPointer<User> user)
{
    QMutexLocker locker(&m_mutex);

    for (const auto& pair : Conections)
    {
        if (pair.get()->getName() == user->getName())
        {
            P_Notification::sendNotification(clientSocket, TypeMessage::Error, "You are logged in another location");
            return false;
        }
    }

    Conections.insert(clientSocket, user);
    return true;
}

QList<QString> NetworkServer::getOnlineUsers()
{
    QMutexLocker locker(&m_mutex);

    QList<QString> users;
    for (auto it = Conections.begin(); it != Conections.end(); ++it)
        users.append(it.value()->getLogin());

    return users;
}
