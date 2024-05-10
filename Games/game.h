﻿#ifndef GAME_H
#define GAME_H

#include <QString>
#include <QSharedPointer>
#include <QDataStream>
#include <QIODevice>
#include <QList>

#include "network/networkserver.h"
#include "Users/player.h"
#include "Games/Cards/card.h"
#include "Games/table.h"
#include "Games/Cards/deck.h"
#include "Utils/ticker.h"

class Player;
class Game : public QObject
{
    static QMap<QString, QSharedPointer<Game>> games;
    static QMutex accessGamesMutex;

protected:
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
    QPair<QSharedPointer<Player>, int> activePlayer;
    QMap<QSharedPointer<Player>, QList<Card>> playersHands;
    QList<QSharedPointer<Player>> winners{};
    QSharedPointer<Deck> deck;
    QSharedPointer<Func> pointerOnTick;
    PacketTypes packettype = PacketTypes::P_GamePacket;

    QList<QSharedPointer<Player>> getPlayers() const;
    virtual void notifyOthersTakenCard(QSharedPointer<Player> thisPlayer);
    virtual void giveCardToPlayer(QSharedPointer<SOCKET> clientSocket, QSharedPointer<Player> player);
    virtual void passTurnToNextPlayer();
    virtual void onGameFinished();
    virtual void resetDeck();
    virtual void stopGame();
    virtual void onTick();
    virtual void handleMultipleWinners(QList<QSharedPointer<Player>> winners);

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
};

#endif // GAME_H
