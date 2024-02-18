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

//А для чего запрашивать целую модель если цель этой функции возврат одной строки?
/*
QString DatabaseManager::executeQuery(QString executequery)
{
    open();
    QSqlQueryModel query;

    query.setQuery(executequery, *db);

    if (query.lastError().isValid())
        qDebug() << query.lastError(); //А если все таки произошла ошибка, мы получим лишь сообщение в консоль которую не факт что в реальных условиях мониторят 24/7 и отправим клиенту неопределенное сообщение в виде ответа, при парсе которого он скорее всего крашнется

    close();
    return query.data(query.index(0, 0)).toString();
}
*/
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

QSharedPointer<QSqlTableModel> DatabaseManager::getModel(QString tableName, int offset)
{
    open();
    QSharedPointer<QSqlTableModel> model(new QSqlTableModel(nullptr, *db)); //Эта запись эквивалентна этой - QSharedPointer<QSqlTableModel> model = QSharedPointer<QSqlTableModel>::create(nullptr, *db);, единственное отличие в том, что тут мы сразу в конструкторе передаем все параметры
    model->setTable(tableName);
    model->setFilter(" 1=1 LIMIT 50 OFFSET " + QString::number(offset));
    model->select();
    close();
    return model;
}
