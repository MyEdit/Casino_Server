#ifndef USER_H
#define USER_H

#include <QString>
#include <network/PacketTypes.h>
#include <QSharedPointer>
#include <QIODevice>
#include <QDataStream>

class User
{
public:
    int ID{};
    QString name{};
    QString login{};
    Roles role{};
    QByteArray photo{};

    virtual ~User() {}

    //GETTERS
    int getID() const;
    const QString& getName();
    const QString& getLogin();
    Roles getRole() const;
    const QByteArray& getPhoto();
    virtual QByteArray serializeUser() = 0;
};

#endif // USER_H
