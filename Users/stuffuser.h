#ifndef STUFFUSER_H
#define STUFFUSER_H

#include "Users/user.h"

class StuffUser : public User
{
    static QList<StuffUser> stuffUsers;

public:
    StuffUser(int ID, QString name, QString login, Roles role, QByteArray photo);
    StuffUser(const QByteArray& data);

    //GETTERS
    int getID() override;
    QString getName() override;
    QString getLogin() override;
    Roles getRole() override;
    QByteArray getPhoto() override;

    //METHODS
    QByteArray serializeUser() override;
};

#endif // STUFFUSER_H
