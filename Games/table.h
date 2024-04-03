#ifndef TABLE_H
#define TABLE_H

#include <QSharedPointer>
#include <QList>
#include "Games/player.h"
#include "Games/game.h"

struct TableSettings
{
    int ID;
    double minBet;
    double stepBet;
    double minBalance;
};

class Table
{
    static QList<Table> tables;

    Game game{};
    TableSettings tableSettings{};
    QList<QSharedPointer<Player>> playes{};

public:
    Table(Game game, TableSettings tableSettings);

    //GETTERS
    TableSettings getSettings();
    Game getGame();

    //METHODS
    bool canPlayerJoin(QSharedPointer<Player>);
    bool canStartGame();
    void startGame();
    void joinPlayer(Player player);
    void leavePlayer(Player player);
};

#endif // TABLE_H
