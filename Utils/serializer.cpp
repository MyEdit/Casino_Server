#include "serializer.h"

QByteArray Serializer::serializeDataModel(QSharedPointer<QSqlTableModel> model)
{
    QJsonArray rowsArray;
    QStringList headers;

    for (int j = 0; j < model->columnCount(); ++j)
        headers.append(model->headerData(j, Qt::Horizontal).toString());

    QJsonObject headerObject;
    headerObject["headers"] = QJsonArray::fromStringList(headers);
    rowsArray.append(headerObject);

    for (int i = 0; i < model->rowCount(); ++i)
    {
        QJsonObject rowObject;
        for (int j = 0; j < model->columnCount(); ++j)
            rowObject[headers[j]] = model->data(model->index(i, j)).toString();

        rowsArray.append(rowObject);
    }

    QJsonDocument jsonDoc(rowsArray);
    QByteArray jsonData = jsonDoc.toJson();

    return jsonData;
}
