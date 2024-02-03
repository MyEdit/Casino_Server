#include "databasemanager.h"

DatabaseManager::DatabaseManager()
{
    QString connectionName = "Connection_" + QString::number(QRandomGenerator::global()->generate());
    db = QSharedPointer<QSqlDatabase>::create(QSqlDatabase::addDatabase("QSQLITE", connectionName));
    db->setDatabaseName("D:/C++ Projects/Casino_Server/Database/Database.sqlite");
}

void DatabaseManager::open()
{
    db->open();
}

void DatabaseManager::close()
{
    db->close();
}

QString DatabaseManager::executeQuery(QString executequery)
{
    open();
    QSqlQuery query(executequery, *db);

    if (!query.exec())
        return nullptr;

    if (!query.next())
        return nullptr;

    close();

    return query.value(0).toString();
}

bool DatabaseManager::executeQueryWithoutResponce(QString executequery)
{
    QSqlQuery query(executequery, *db);

    if (!query.exec())
        return false;

    return true;
}
