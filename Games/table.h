#ifndef TABLE_H
#define TABLE_H

#include <QSharedPointer>
#include <QList>
#include <QMutex>
#include <QString>
#include "Users/player.h"
#include "Games/game.h"
#include "Utils/Message.h"
#include "QTimer"
#include "network/networkserver.h"
#include "Utils/ticker.h"

struct TableSettings
{
    int ID;
    double minBet;
    double stepBet;
    double minBalance;
    int maxCountPlayers;

    QByteArray serializeTableSettings() const
    {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << ID << minBet << stepBet << minBalance << maxCountPlayers;
        return data;
    }

    static TableSettings deserializeTableSettings(const QByteArray& data)
    {
        QDataStream stream(data);
        TableSettings settings;
        stream >> settings.ID >> settings.minBet >> settings.stepBet >> settings.minBalance >> settings.maxCountPlayers;
        return settings;
    }
};

class Game;
class Table : public QObject
{
    Q_OBJECT

    QSharedPointer<Game> game;
    TableSettings tableSettings;
    QList<QSharedPointer<Player>> players;
    static QMutex accessTablesMutex;
    static QList<QSharedPointer<Table>> tables;
    int timeToStart = 10;
    bool isGameReady = false;

public:
    Table(QSharedPointer<Game> game, TableSettings tableSettings);
    Table(const QByteArray& data);

    //GETTERS
    TableSettings getSettings();
    QSharedPointer<Game> getGame();
    int getCurrentNumPlayer();
    QList<QSharedPointer<Player>> getPlayers();
    static QSharedPointer<Table> getTable(const int& ID);
    static QList<QSharedPointer<Table>> getTabels();

    //METHODS
    bool canPlayerJoin(QSharedPointer<Player> player);
    void joinPlayer(QSharedPointer<Player> player);
    void leavePlayer(QSharedPointer<Player> player);
    void setNewData(QSharedPointer<Game> game, TableSettings tableSettings);
    void updatePlayersList();
    QByteArray serializeTable();
    static void addTable(QSharedPointer<Table> table);

private:
    void sendTimerData();
    void setTicker();
    void onTick();
};

#endif // TABLE_H
