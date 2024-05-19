#ifndef STUFFUSER_H
#define STUFFUSER_H

#include "Users/user.h"

class StuffUser : public User
{
    static QList<StuffUser> stuffUsers;

public:
    StuffUser(const int& ID, const QString& name, const QString& login, const Roles role, const QByteArray& photo);
    StuffUser(const QByteArray& data);

    //METHODS
    QByteArray serializeUser() override;
};

#endif // STUFFUSER_H
