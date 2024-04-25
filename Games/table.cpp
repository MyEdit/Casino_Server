#include "table.h"

QList<QSharedPointer<Table>> Table::tables;
QMutex Table::accessTablesMutex;

Table::Table(Game game, TableSettings tableSettings)
{
    this->game = game;
    this->tableSettings = tableSettings;
    setTicker();
}

Table::Table(const QByteArray& data)
{
    QDataStream stream(data);
    QByteArray gameData, settingsData;
    int currentNumPlayer;
    stream >> gameData >> settingsData >> currentNumPlayer;
    QSharedPointer<Game> game = Game::deserializeGame(gameData);
    TableSettings settings = TableSettings::deserializeTableSettings(settingsData);

    for (int i = 0; i < currentNumPlayer; ++i)
    {
        QByteArray playerData;
        stream >> playerData;
        QSharedPointer<Player> player(new Player(playerData));
        players.append(player);
    }

    this->game = *game.get();
    this->tableSettings = settings;
    setTicker();
}

void Table::setTicker()
{
    Ticker::addListener(QSharedPointer<QObject>(this), std::bind(&Table::onTick, this));
}

void Table::onTick()
{
    if (isGameRunning)
        return;

    if (!canStartGame()) //Если условие старта игры не выплняется
    {
        timeToStart = -1;
        isGameReady = false;
        sendTimerData();
        return;
    }

    if (canStartGame() && !isGameReady) //Если условие старта игры начало выполняться
    {
        timeToStart = 10;
        isGameReady = true;
    }

    if (timeToStart >= 0)
    {
        sendTimerData();
        --timeToStart;
    }
    else
        startGame();
}

void Table::joinPlayer(QSharedPointer<Player> player)
{
    QMutexLocker locker(&accessTablesMutex);
    players.append(player);
    timeToStart = 10;
}

void Table::leavePlayer(QSharedPointer<Player> player)
{
    QMutexLocker locker(&accessTablesMutex);
    for (QSharedPointer<Player> p : players)
    {
        if (p->getLogin() == player->getLogin())
        {
            players.removeOne(p);
            break;
        }
    }
    timeToStart = 10;
}

void Table::sendTimerData()
{
    for (QSharedPointer<Player> player : this->players)
    {
        QString time = QString::number(timeToStart);
        QSharedPointer<SOCKET> clientSocket = NetworkServer::getSocketUser(player);
        PacketTypes packettype = PacketTypes::P_UpdateGameProcessing;
        NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
        NetworkServer::sendToClient(clientSocket, time);
    }
}

bool Table::canStartGame()
{
    if (this->players.size() < this->game.getMinPlayers())
        return false;

    return true;
}

void Table::startGame()
{
    isGameRunning = true;

    QSharedPointer<SOCKET> clientSocket = NetworkServer::getSocketUser(players.at(0));
    PacketTypes packettype = PacketTypes::P_StartMove;
    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, "Ваш");

    for(QSharedPointer<Player> player : players)
    {
        if(player->getLogin() == players.at(0)->getLogin())
            continue;

        QSharedPointer<SOCKET> clientSocket = NetworkServer::getSocketUser(player);
        PacketTypes packettype = PacketTypes::P_UpdateGameProcessing;
        NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
        NetworkServer::sendToClient(clientSocket, players.at(0)->getName());
    }
}

void Table::stopGame()
{
    isGameRunning = false;
}

void Table::addTable(QSharedPointer<Table> table)
{
    QMutexLocker locker(&accessTablesMutex);
    if(!tables.contains(table))
        tables.append(table);
}

bool Table::canPlayerJoin(QSharedPointer<Player> player)
{
    if (player->getBalance() < this->tableSettings.minBalance)
        return false;

    if (this->players.size() >= this->tableSettings.maxCountPlayers)
        return false;

    if (isGameRunning)
        return false;

    return true;
}

QByteArray Table::serializeTable()
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    QByteArray gameData = game.serializeGame();
    QByteArray settingsData = tableSettings.serializeTableSettings();
    int currentNumPlayer = players.size();

    stream << gameData << settingsData << currentNumPlayer;

    for (QSharedPointer<Player> player : players)
    {
        QByteArray playerData = player->serializeUser();
        stream << playerData;
    }

    return data;
}

QSharedPointer<Table> Table::getTable(int ID)
{
    QMutexLocker locker(&accessTablesMutex);
    for (QSharedPointer<Table> table : tables)
    {
        if (table->getSettings().ID == ID)
        {
            return table;
        }
    }

    return nullptr;
}

Game Table::getGame()
{
    return game;
}

TableSettings Table::getSettings()
{
    return tableSettings;
}

int Table::getCurrentNumPlayer()
{
    return  players.size();
}

QList<QSharedPointer<Player>> Table::getPlayers()
{
    return players;
}

void Table::setNewData(Game game, TableSettings tableSettings)
{
    this->game = game;
    this->tableSettings = tableSettings;
}

QList<QSharedPointer<Table>> Table::getTabels()
{
    QMutexLocker locker(&accessTablesMutex);
    return tables;
}
