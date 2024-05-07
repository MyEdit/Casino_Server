#ifndef PLAYER_H
#define PLAYER_H

#include "Users/user.h"
#include "Database/databasemanager.h"
#include "network/networkserver.h"

class Player : public User
{
    static QList<Player> players;
    QMutex accessMutex;
    double balance{};

public:
    Player() {}
    Player(const int& ID, const QString& name, const double& balance, const QString& login, const Roles role, const QByteArray& photo);
    Player(const QByteArray& data);

    //GETTERS
    int getID() override;
    QString getName() override;
    QString getLogin() override;
    Roles getRole() override;
    double getBalance();
    QByteArray getPhoto() override;

    //METHODS
    QByteArray serializeUser() override;
    void setBalance(double newBalance);
};

#endif // PLAYER_H
