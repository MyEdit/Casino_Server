#include "game.h"

Game::Game()
{

}

Game::Game(QString nameGame)
{
    this->nameGame = nameGame;
}

QString Game::getName()
{
    return nameGame;
}

int Game::getMinPlayers()
{
    return minPlayers;
}

QByteArray Game::serializeGame()
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << nameGame;
    return data;
}

QSharedPointer<Game> Game::deserializeGame(const QByteArray& data)
{
    QDataStream stream(data);
    QString nameGame;
    stream >> nameGame;
    return QSharedPointer<Game>(new Game(nameGame));
}
