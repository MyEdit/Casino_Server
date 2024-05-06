#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSharedPointer>
#include <QRandomGenerator>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QSqlTableModel>
#include <QSqlRecord>
#include "Utils/Message.h"

class DatabaseManager
{
private:
    QSharedPointer<QSqlDatabase> db;

    void open();
    void close();

public:
    DatabaseManager();
    QString executeQuery(const QString& executequery);
    bool executeQueryDeleteWithoutResponce(const QString& executequery);
    bool executeQueryWithoutResponce(const QString& executequery);
    QSharedPointer<QSqlQueryModel> getModel(const QString& tableName, const int& offset, const QString& sort, const QString &where);
    QSharedPointer<QSqlQuery> executeQueryObject(const QString& executequery);
    QList<QSharedPointer<QSqlRecord>> executeQueryObjects(const QString& executeQuery);
};

#endif // DATABASEMANAGER_H
