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
    QSqlQuery query(executequery, *db);

    if (!query.exec())
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

bool DatabaseManager::executeQueryWithoutResponce(QString executequery)
{
    open();
    QSqlQuery query(*db);
    bool success = query.exec(executequery);
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
        QString zapros = "SELECT * FROM temp_" + tableName;
        model->setQuery(zapros, *db);
    }

    close();
    return model;
}
