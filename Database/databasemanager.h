#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSharedPointer>
#include <QRandomGenerator>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVariant>

class DatabaseManager
{
private:
    QSharedPointer<QSqlDatabase> db;

    void open();
    void close();

public:
    DatabaseManager();
    QString executeQuery(QString executequery);
    bool executeQueryWithoutResponce(QString executequery);
};

#endif // DATABASEMANAGER_H
