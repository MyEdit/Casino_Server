#ifndef TABLE_H
#define TABLE_H

#include <QSharedPointer>
#include <QList>
#include "Users/player.h"
#include "Games/game.h"

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
    QList<QSharedPointer<Player>> playes{};

public:
    Table(Game game, TableSettings tableSettings);

    static QList<QSharedPointer<Table>> tables;

    //GETTERS
    TableSettings getSettings();
    Game getGame();
    int getCurrentNumPlayer();
    static void addTable(QSharedPointer<Table> table);

    //METHODS
    bool canPlayerJoin(QSharedPointer<Player>);
    bool canStartGame();
    void startGame();
    void joinPlayer(Player player);
    void leavePlayer(Player player);
    QByteArray serializeTable();
    static QSharedPointer<Table> deserializeTable(const QByteArray& data);
};

#endif // TABLE_H
