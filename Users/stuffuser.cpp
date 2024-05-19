#include "stuffuser.h"

StuffUser::StuffUser(const int& ID, const QString& name, const QString& login, const Roles role, const QByteArray& photo)
{
    this->ID = ID;
    this->name = name;
    this->login = login;
    this->role = role;
    this->photo = photo;
}

StuffUser::StuffUser(const QByteArray& data)
{
    QDataStream stream(data);
    int ID;
    QString name, login;
    int roleInt;
    QByteArray photo;
    stream >> ID >> name >> login >> roleInt >> photo;
    Roles role = static_cast<Roles>(roleInt);

    this->ID = ID;
    this->name = name;
    this->login = login;
    this->role = role;
    this->photo = photo;
}

QByteArray StuffUser::serializeUser()
{
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream << ID;
    stream << name;
    stream << login;
    stream << static_cast<int>(role);
    stream << photo;
    return byteArray;
}
