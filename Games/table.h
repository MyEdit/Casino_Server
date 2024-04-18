#ifndef TABLE_H
#define TABLE_H

#include <QSharedPointer>
#include <QList>
#include <QMutex>
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
    QList<QSharedPointer<Player>> players{};
    static QMutex accessTablesMutex;
    static QList<QSharedPointer<Table>> tables;

public:
    Table(Game game, TableSettings tableSettings);
    Table(const QByteArray& data);

    //GETTERS
    TableSettings getSettings();
    Game getGame();
    int getCurrentNumPlayer();
    QList<QSharedPointer<Player>> getPlaers();
    static QSharedPointer<Table> getTable(int ID);
    static QList<QSharedPointer<Table>> getTabels();

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
