﻿#include "table.h"

QList<QSharedPointer<Table>> Table::tables;
QMutex Table::accessTablesMutex;

Table::Table(QSharedPointer<Game> game, TableSettings tableSettings)
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

    this->game = game;
    this->tableSettings = settings;
    setTicker();
}

void Table::setTicker()
{
    Ticker::addListener(QSharedPointer<QObject>(this), std::bind(&Table::onTick, this));
}

void Table::onTick()
{
    if (game->isGameRunning())
        return;

    if (!this->game->canStartGame()) //Если условие старта игры не выплняется
    {
        timeToStart = -1;
        isGameReady = false;
        sendTimerData();
        return;
    }

    if (this->game->canStartGame() && !isGameReady) //Если условие старта игры начало выполняться
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
        game->startGame();
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

void Table::addTable(QSharedPointer<Table> table)
{
    QMutexLocker locker(&accessTablesMutex);
    if(!tables.contains(table))
        tables.append(table);
}

bool Table::canPlayerJoin(QSharedPointer<Player> player)
{
    if (game->isGameRunning())
        return false;

    if (player->getBalance() < this->tableSettings.minBalance)
        return false;

    if (this->players.size() >= this->tableSettings.maxCountPlayers)
        return false;

    if (!this->game->canPlayerJoin(player))
        return false;

    return true;
}

void Table::updatePlayersList()
{
    QList<QSharedPointer<Player>> players;

    for(QSharedPointer<Player> p : this->players)
    {
        if(p != nullptr)
            players.append(p);
    }

    this->players = players;
}

QByteArray Table::serializeTable()
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    QString gameData = game->getName();
    QByteArray settingsData = tableSettings.serializeTableSettings();

    updatePlayersList();
    int currentNumPlayer = players.size();

    stream << gameData << settingsData << currentNumPlayer;

    for (QSharedPointer<Player> player : players)
    {
        QByteArray playerData = player->serializeUser();
        stream << playerData;
    }

    return data;
}

QSharedPointer<Table> Table::getTable(const int& ID)
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

QSharedPointer<Game> Table::getGame()
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

void Table::setNewData(QSharedPointer<Game> game, TableSettings tableSettings)
{
    this->game = game;
    this->tableSettings = tableSettings;
}

QList<QSharedPointer<Table>> Table::getTabels()
{
    QMutexLocker locker(&accessTablesMutex);
    return tables;
}
