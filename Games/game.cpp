﻿#include "game.h"

QMap<QString, QSharedPointer<Game>> Game::games;
QMutex Game::accessGamesMutex;

void Game::registerGame(QSharedPointer<Game> game)
{
    QMutexLocker locker(&accessGamesMutex);
    games.insert(game->getName(), game);
}

QSharedPointer<Game> Game::getGame(QString name)
{
    QMutexLocker locker(&accessGamesMutex);
    QSharedPointer<Game> game = games[name];

    if (game == nullptr)
        return nullptr;

    return game->getInstance();
}

QSharedPointer<Game> Game::deserializeGame(const QByteArray& data)
{
    QDataStream stream(data);
    QString nameGame;
    stream >> nameGame;
    return Game::getGame(nameGame);
}
