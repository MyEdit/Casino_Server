#include "player.h"

Player::Player(const int& ID, const QString& name, const double& balance, const QString& login, const Roles role, const QByteArray& photo)
{
    this->ID = ID;
    this->name = name;
    this->balance = balance;
    this->login = login;
    this->role = role;
    this->photo = photo;
}

Player::Player(const QByteArray& data)
{
    QDataStream stream(data);
    int ID;
    QString name, login;
    double balance;
    int roleInt;
    QByteArray photo;
    stream >> ID >> name >> balance >> login >> roleInt >> photo;
    Roles role = static_cast<Roles>(roleInt);

    this->ID = ID;
    this->name = name;
    this->balance = balance;
    this->login = login;
    this->role = role;
    this->photo = photo;
}

int Player::getID()
{
    QMutexLocker locker(&accessMutex);
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
    QMutexLocker locker(&accessMutex);
    return this->balance;
}

QByteArray Player::getPhoto()
{
    return photo;
}

void Player::setBalance(double newBalance)
{
    QSharedPointer<DatabaseManager> databaseManager(new DatabaseManager());
    QSharedPointer<SOCKET> playerSocket = NetworkServer::getSocketByNickname(this->getLogin());
    QString query = QString("UPDATE Users SET Balance = '%1' WHERE ID_User = '%2'")
            .arg(newBalance)
            .arg(ID);

    if (!databaseManager->executeQueryWithoutResponce(query))
    {
        Message::logError("Failed to set balance for player: " + this->getLogin());
        return;
    }

    PacketTypes packettype = PacketTypes::P_UpdateBalance;
    NetworkServer::sendToClient(playerSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(playerSocket, &newBalance, sizeof(double));

    QMutexLocker locker(&accessMutex);
    this->balance = newBalance;
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
    stream << photo;
    return byteArray;
}

