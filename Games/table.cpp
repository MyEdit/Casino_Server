#include "table.h"

QList<QSharedPointer<Table>> Table::tables;
QMutex Table::accessTablesMutex;

Table::Table(Game game, TableSettings tableSettings)
{
    this->game = game;
    this->tableSettings = tableSettings;
    createTimer();
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
    createTimer();
}

Table::~Table() {}

void Table::createTimer()
{
    startGameTimer = QSharedPointer<QTimer>(new QTimer(this));
    connect(startGameTimer.data(), &QTimer::timeout, this, &Table::updateGameTimer);
}

void Table::joinPlayer(QSharedPointer<Player> player)
{
    players.append(player);
    checkTimerCondition();
}

void Table::leavePlayer(QSharedPointer<Player> player)
{
    for (QSharedPointer<Player> p : players)
    {
        if (p->getLogin() == player->getLogin())
        {
            players.removeOne(p);
            break;
        }
    }

    checkTimerCondition();
}

void Table::checkTimerCondition()
{
    if (isGameRunning)
        return;

    if (!canStartGame() && startGameTimer->isActive()) //Если условие старта игры больше не выплняется
    {
        startGameTimer->stop();
        timeToStart = 10;
        sendTimerData();
        return;
    }

    if (canStartGame() && !startGameTimer->isActive()) //Если условие старта игры начало выполняться
    {
        startGameTimer->start(1000);
        timeToStart = 10;
        return;
    }
}

void Table::updateGameTimer()
{
    if (timeToStart > 0) {
        sendTimerData();
        --timeToStart;
    } else {
        startGame();
    }
}

void Table::sendTimerData()
{
    for (QSharedPointer<Player> player : this->players)
    {
        QSharedPointer<SOCKET> clientSocket = NetworkServer::getSocketUser(player);
        PacketTypes packettype = PacketTypes::P_UpdateGameTimer;
        NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
        NetworkServer::sendToClient(clientSocket, &timeToStart, sizeof(int));
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
    Message::logInfo("Game starting");
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
