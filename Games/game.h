#ifndef GAME_H
#define GAME_H

#include <QString>
#include <QSharedPointer>
#include <QDataStream>
#include <QIODevice>

class Game
{
    QString nameGame{};
    int minPlayers{2}; //У каждой игры свой минимальный лимит игроков для старта, пока вписываю тут. TODO: Нужен отдельный класс для игры BlackJack

public:
    Game();
    Game(QString nameGame);

    QString getName();
    int getMinPlayers();

    QByteArray serializeGame();
    static QSharedPointer<Game> deserializeGame(const QByteArray& data);
};

#endif // GAME_H
