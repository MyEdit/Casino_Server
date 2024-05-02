#ifndef GAME_H
#define GAME_H

#include <QString>
#include <QSharedPointer>
#include <QDataStream>
#include <QIODevice>

#include <network/networkserver.h>
#include <Users/player.h>

class Game
{
    static QMap<QString, QSharedPointer<Game>> games;
    static QMutex accessGamesMutex;

public:
    Game() {};
    ~Game() {};

    static void registerGame(QSharedPointer<Game> game);
    static QSharedPointer<Game> getGame(QString name);
    static QSharedPointer<Game> deserializeGame(const QByteArray& data);

    virtual QSharedPointer<Game> getInstance() = 0;
    virtual QString getName() = 0;
    virtual int getMinPlayers() = 0;
    virtual bool canPlayerJoin(QSharedPointer<Player> player) = 0;
    virtual bool canStartGame() = 0;
    virtual bool isGameRunning() = 0;
    virtual void startGame() = 0;
    virtual void stopGame() = 0;
    virtual void giveCardToPlayer(QSharedPointer<SOCKET> clientSocket, QSharedPointer<Player> player) = 0;
    virtual void setTable(const int& tableID) = 0;
    virtual void onGamePacketReceived(QSharedPointer<SOCKET> clientSocket) = 0;
    virtual int getTableID() = 0;
};

#endif // GAME_H
