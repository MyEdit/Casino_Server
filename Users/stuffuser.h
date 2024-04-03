#ifndef STUFFUSER_H
#define STUFFUSER_H

#include "Users/user.h"

class StuffUser : public User
{
    static QList<StuffUser> stuffUsers;

    int ID{};
    QString name{};
    QString login{};
    Roles role{};

public:
    StuffUser(int ID, QString name, QString login, Roles role);

    //GETTERS
    int getID() override;
    QString getName() override;
    QString getLogin() override;
    Roles getRole() override;
};

#endif // STUFFUSER_H
