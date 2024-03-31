#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSharedPointer>
#include <QRandomGenerator>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QSqlTableModel>
#include "Utils/Message.h"

class DatabaseManager
{
private:
    QSharedPointer<QSqlDatabase> db;

    void open();
    void close();

public:
    DatabaseManager();
    QString executeQuery(QString executequery);
    bool executeQueryDeleteWithoutResponce(QString executequery);
    bool executeQueryWithoutResponce(QString executequery);
    QSharedPointer<QSqlQueryModel> getModel(QString tableName, int offset, QString sort);
};

#endif // DATABASEMANAGER_H
