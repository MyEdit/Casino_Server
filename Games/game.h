#ifndef GAME_H
#define GAME_H

#include <QString>
#include <QSharedPointer>
#include <QDataStream>
#include <QIODevice>
#include <QList>
#include <QDate>

#include "network/networkserver.h"
#include "Users/player.h"
#include "Games/Cards/card.h"
#include "Games/table.h"
#include "Games/Cards/deck.h"
#include "Utils/ticker.h"

class Player;
class Table;

class Game : public QObject
{
protected:
    static QMap<QString, QSharedPointer<Game>> games;
    static QMutex accessGamesMutex;

    enum class GamePackets
    {
        P_TakeCard,
        P_TakeCardAnotherPlayer,
        P_PassMove,
        P_StartMove,
        P_Win,
        P_Lose
    };

    int tableID;
    bool gameRunning {false};
    QPair<QSharedPointer<Player>, int> activePlayer; //Pair<Игрок, Его_Индекс>
    QMap<QSharedPointer<Player>, QList<Card>> playersHands;
    QList<QSharedPointer<Player>> winners{};
    QSharedPointer<Deck> deck;
    QSharedPointer<Func> pointerOnTick;
    PacketTypes packettype = PacketTypes::P_GamePacket;
    QMap<GamePackets, std::function<void(QSharedPointer<SOCKET> clientSocket)>> gamePacketFunction;

    QList<QSharedPointer<Player>> getPlayers() const;
    void creditingProfitsCasino(double commissionCasino);
    void initPacketHandlerFunction();
    virtual void notifyOthersTakenCard(QSharedPointer<Player> thisPlayer);
    virtual void giveCardToPlayer(QSharedPointer<SOCKET> clientSocket, QSharedPointer<Player> player);
    virtual void passTurnToNextPlayer();
    virtual void onGameFinished();
    virtual void resetDeck();
    virtual void stopGame();
    virtual void onTick();
    virtual void handleMultipleWinners(QList<QSharedPointer<Player>> winners);
    virtual void changingBalanceWhenWin(QSharedPointer<Player> player, QSharedPointer<Table> table);
    virtual void changingBalanceWhenLos(QSharedPointer<Player> player, QSharedPointer<Table> table);
    virtual void changingBalanceEveryoneLoses(QSharedPointer<Table> table);

public:
    Game();
    ~Game() {};

    static void registerGame(QSharedPointer<Game> game);
    static QSharedPointer<Game> getGame(QString name);
    static QSharedPointer<Game> deserializeGame(const QByteArray& data);

    void setTable(const int& tableID);
    int getTableID();
    bool isGameRunning();

    virtual QSharedPointer<Game> getInstance() = 0;
    virtual QString getName() = 0;
    virtual int getMinPlayers() = 0;
    virtual QList<QSharedPointer<Player>> getWinners() const = 0;

    virtual void onGamePacketReceived(QSharedPointer<SOCKET> clientSocket);
    virtual bool canPlayerJoin(QSharedPointer<Player> player);
    virtual bool canStartGame();
    virtual void startGame();
    virtual void playerLeave(QSharedPointer<Player> player);
};

#endif // GAME_H
