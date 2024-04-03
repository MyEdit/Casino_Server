#ifndef PLAYER_H
#define PLAYER_H

#include "Users/user.h"

class Player : public User
{
    static QList<Player> players;

    int ID{};
    QString name{};
    QString login{};
    Roles role{};
    double balance{};

public:
    Player(int ID, QString name, double balance, QString login, Roles role);

    //GETTERS
    int getID() override;
    QString getName() override;
    QString getLogin() override;
    Roles getRole() override;
    double getBalance();
    QByteArray serializeUser() override;
    static QSharedPointer<Player> deserializeUser(const QByteArray& data);
};

#endif // PLAYER_H
