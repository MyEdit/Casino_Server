#ifndef UPDATER_H
#define UPDATER_H

#include "Utils/ticker.h"
#include "Games/table.h"
#include "Database/databasemanager.h"

class Updater
{
    QSharedPointer<Func> pointerOnTick;

public:
    void init();

private:
    void startTablesUpdate();
    void onTick();
    void deleteTable(const QList<QSharedPointer<QSqlRecord>>& newResult);
    void createOrUpdateTable(QSharedPointer<Game> game, TableSettings tableSettings);
};

#endif // UPDATER_H
