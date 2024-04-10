#include "table.h"

QList<QSharedPointer<Table>> Table::tables;

Table::Table(Game game, TableSettings tableSettings)
{
    this->game = game;
    this->tableSettings = tableSettings;
}

Table::Table(const QByteArray& data)
{
    QDataStream stream(data);
    QByteArray gameData, settingsData;
    int currentNumPlayer;
    stream >> gameData >> settingsData >> currentNumPlayer;
    QSharedPointer<Game> game = Game::deserializeGame(gameData);
    TableSettings settings = TableSettings::deserializeTableSettings(settingsData);

    for (int i = 0; i < currentNumPlayer; ++i)
    {
        QByteArray playerData;
        stream >> playerData;
        QSharedPointer<Player> player(new Player(playerData));
        playes.append(player);
    }

    this->game = *game.get();
    this->tableSettings = settings;
}

void Table::addTable(QSharedPointer<Table> table)
{
    if(!tables.contains(table))
        tables.append(table);
}

QByteArray Table::serializeTable()
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    QByteArray gameData = game.serializeGame();
    QByteArray settingsData = tableSettings.serializeTableSettings();
    int currentNumPlayer = playes.size();

    stream << gameData << settingsData << currentNumPlayer;

    for (QSharedPointer<Player> player : playes)
    {
        QByteArray playerData = player->serializeUser();
        stream << playerData;
    }

    return data;
}

QSharedPointer<Table> Table::getTable(int ID)
{
    std::find_if(tables.begin(), tables.end(), [ID](const QSharedPointer<Table>& table)
    {
        return table->getSettings().ID == ID;
    });

    return nullptr;
}

Game Table::getGame()
{
    return game;
}

TableSettings Table::getSettings()
{
    return tableSettings;
}

int Table::getCurrentNumPlayer()
{
    return  playes.size();
}
