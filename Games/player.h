#ifndef PLAYER_H
#define PLAYER_H

#include <QString>

class Player
{
    int ID{};
    QString name{};
    double balance{};
    QString login{};

public:
    Player(int ID, QString name, double balance, QString login);
};

#endif // PLAYER_H
