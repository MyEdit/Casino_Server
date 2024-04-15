#ifndef TABLE_H
#define TABLE_H

#include <QSharedPointer>
#include <QList>
#include "Users/player.h"
#include "Games/game.h"
#include "Utils/Message.h"

//Перенеси бы в отдельный файл
struct TableSettings
{
    int ID;
    double minBet;
    double stepBet;
    double minBalance;
    int maxNumPlayer;

    QByteArray serializeTableSettings() const
    {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << ID << minBet << stepBet << minBalance << maxNumPlayer;
        return data;
    }

    static TableSettings deserializeTableSettings(const QByteArray& data)
    {
        QDataStream stream(data);
        TableSettings settings;
        stream >> settings.ID >> settings.minBet >> settings.stepBet >> settings.minBalance >> settings.maxNumPlayer;
        return settings;
    }
};

class Table
{
    Game game{};
    TableSettings tableSettings{};

    public:
    QList<QSharedPointer<Player>> playes{};

public:
    Table(Game game, TableSettings tableSettings);
    Table(const QByteArray& data);
    static QList<QSharedPointer<Table>> tables;

    //GETTERS
    TableSettings getSettings();
    Game getGame();
    int getCurrentNumPlayer();
    static QSharedPointer<Table> getTable(int ID);

    static void addTable(QSharedPointer<Table> table);

    //METHODS
    bool canPlayerJoin(QSharedPointer<Player> player);
    bool canStartGame();
    void startGame();
    void joinPlayer(QSharedPointer<Player> player);
    void leavePlayer(QSharedPointer<Player> player);
    QByteArray serializeTable();
    void setNewData(Game game, TableSettings tableSettings);
};

#endif // TABLE_H
