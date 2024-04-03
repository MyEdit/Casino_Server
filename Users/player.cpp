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
