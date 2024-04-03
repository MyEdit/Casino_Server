﻿#ifndef USER_H
#define USER_H

#include <QString>
#include <network/PacketTypes.h>
#include <QSharedPointer>
#include <QIODevice>
#include <QDataStream>

class User
{
public:
    virtual ~User() {}

    //GETTERS
    virtual int getID() = 0;
    virtual QString getName() = 0;
    virtual QString getLogin() = 0;
    virtual Roles getRole() = 0;
    virtual QByteArray serializeUser() = 0;
};

#endif // USER_H
