#include "databasemanager.h"
#include <QSqlError>

DatabaseManager::DatabaseManager()
{
    QString connectionName = "Connection_" + QString::number(QRandomGenerator::global()->generate());
    db = QSharedPointer<QSqlDatabase>::create(QSqlDatabase::addDatabase("QSQLITE", connectionName));
//    db->setDatabaseName("D:/C++ Projects/Casino_Server/Database/Database.sqlite");
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

QString DatabaseManager::executeQuery(QString executequery)
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

QSharedPointer<QSqlQuery> DatabaseManager::executeQueryObject(QString executequery)
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

QSharedPointer<QSqlQueryModel> DatabaseManager::executeQueryObjects(QString executequery)
{

}
bool DatabaseManager::executeQueryWithoutResponce(QString executequery)
{
    open();
    QSqlQuery query(*db);
    bool success = query.exec(executequery);

    if (!success)
        Message::logError(query.lastError().text());

    close();
    return success;
}


//TODO: это костыль, но как то всё же нужно выполнить запрос на разрешение каскадного удаления(для каждого соединения оно выключено по умолчанию) и сам запрос на удаление
bool DatabaseManager::executeQueryDeleteWithoutResponce(QString executequery)
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

QSharedPointer<QSqlQueryModel> DatabaseManager::getModel(QString tableName, int offset, QString sort)
{
    open();
    QSharedPointer<QSqlQueryModel> model(new QSqlTableModel());

    QString request("CREATE TEMPORARY TABLE temp_" + tableName + " AS SELECT ROW_NUMBER() OVER (" + sort + ") AS №, sorted_data.* FROM "
                    "(SELECT * FROM " + tableName + ") AS sorted_data where  1=1 LIMIT 50 OFFSET " + QString::number(offset));

    model->setQuery(request, *db);

    if (!model->lastError().isValid())
    {
        QString query = "SELECT * FROM temp_" + tableName;
        model->setQuery(query, *db);
    }

    close();
    return model;
}
