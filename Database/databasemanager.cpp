﻿#include "databasemanager.h"
#include <QSqlError>

DatabaseManager::DatabaseManager()
{
    QString connectionName = "Connection_" + QString::number(QRandomGenerator::global()->generate());
    db = QSharedPointer<QSqlDatabase>::create(QSqlDatabase::addDatabase("QSQLITE", connectionName));
    db->setDatabaseName("Database/Database.sqlite");
}

void DatabaseManager::open()
{
    db->open();
}

void DatabaseManager::close()
{
    db->close();
}

QString DatabaseManager::executeQuery(const QString& executequery)
{
    open();
    QSqlQuery query(*db);

    if (!query.exec(executequery))
    {
        close();
        return nullptr;
    }

    if (!query.next())
    {
        close();
        return nullptr;
    }

    close();
    return query.value(0).toString();
}

QSharedPointer<QSqlQuery> DatabaseManager::executeQueryObject(const QString& executequery)
{
    open();
    QSharedPointer<QSqlQuery> query(new QSqlQuery(*db));

    if (!query->exec(executequery))
    {
        close();
        return nullptr;
    }

    if (!query->next())
    {
        close();
        return nullptr;
    }

    return query;
}

QList<QSharedPointer<QSqlRecord>> DatabaseManager::executeQueryObjects(const QString& executeQuery)
{
    open();
    QSharedPointer<QSqlQuery> query(new QSqlQuery(*db));

    QList<QSharedPointer<QSqlRecord>> result;

    if (!query->exec(executeQuery))
    {
        close();
        return result;
    }

    while (query->next())
    {
        QSharedPointer<QSqlRecord> record(new QSqlRecord(query->record()));
        result.append(record);
    }

    close();
    return result;
}


bool DatabaseManager::executeQueryWithoutResponce(const QString& executequery)
{
    open();
    QSqlQuery query(*db);
    bool success = query.exec(executequery);

    if (!success)
        Message::logError(query.lastError().text());

    close();
    return success;
}

bool DatabaseManager::executeQueryDeleteWithoutResponce(const QString& executequery)
{
    open();
    QSqlQuery query(*db);

    //PRAGMA foreign_keys = ON; - для включения каскадного удаления (приколы SQLite, что оно по умолчанию всегда выключено)
    if (!query.exec("PRAGMA foreign_keys = ON;"))
        Message::logError(query.lastError().text());

    bool success = query.exec(executequery);

    if (!success)
        Message::logError(query.lastError().text());

    close();
    return success;
}

QSharedPointer<QSqlQueryModel> DatabaseManager::getModel(const QString& tableName, const int& offset, const QString& sort, const QString& where)
{
    open();
    QSharedPointer<QSqlQueryModel> model(new QSqlTableModel());

    QString request("CREATE TEMPORARY TABLE temp_" + tableName + " AS SELECT ROW_NUMBER() OVER (" + sort + ") AS '', sorted_data.* FROM "
                    "(SELECT * FROM " + tableName + " WHERE 1=1 " + where + ") AS sorted_data where 1=1 LIMIT 50 OFFSET " + QString::number(offset));

    model->setQuery(request, *db);

    if (!model->lastError().isValid())
    {
        QString query = "SELECT * FROM temp_" + tableName;
        model->setQuery(query, *db);
    }

    close();

    return model;
}
