#include "player.h"

Player::Player(int ID, QString name, double balance, QString login, Roles role)
{
    this->ID = ID;
    this->name = name;
    this->balance = balance;
    this->login = login;
    this->role = role;
}

int Player::getID()
{
    return this->ID;
}

QString Player::getName()
{
    return this->name;
}
QString Player::getLogin()
{
    return this->login;
}

Roles Player::getRole()
{
    return this->role;
}

double Player::getBalance()
{
    return this->balance;
}

QSharedPointer<Player> Player::deserializeUser(const QByteArray& data)
{
    QDataStream stream(data);
    int id;
    QString name, login;
    double balance;
    int roleInt;
    stream >> id >> name >> balance >> login >> roleInt;
    Roles role = static_cast<Roles>(roleInt);

    return QSharedPointer<Player>::create(id, name, balance, login, role);
}

QByteArray Player::serializeUser()
{
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream << ID;
    stream << name;
    stream << balance;
    stream << login;
    stream << static_cast<int>(role);
    return byteArray;
}
