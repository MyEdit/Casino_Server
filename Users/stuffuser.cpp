#include "stuffuser.h"

StuffUser::StuffUser(int ID, QString name, QString login, Roles role)
{
    this->ID = ID;
    this->name = name;
    this->login = login;
    this->role = role;
}

int StuffUser::getID()
{
    return this->ID;
}

QString StuffUser::getName()
{
    return this->name;
}
QString StuffUser::getLogin()
{
    return this->login;
}

Roles StuffUser::getRole()
{
    return this->role;
}
