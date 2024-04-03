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
    QByteArray serializeUser() override;
    static QSharedPointer<StuffUser> deserializeUser(const QByteArray& data);
};

#endif // STUFFUSER_H
