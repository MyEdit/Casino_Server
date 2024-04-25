#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <QByteArray>
#include <QSqlTableModel>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QSharedPointer>

class Serializer
{
public:
    static QByteArray serializeDataModel(const QSharedPointer<QSqlQueryModel> model);
};

#endif // SERIALIZER_H
