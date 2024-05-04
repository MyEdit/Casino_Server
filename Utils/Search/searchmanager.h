#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H

#include <QObject>
#include <QRegularExpression>
#include "Utils/Search/searchthread.h"
#include "network/networkserver.h"
#include "network/PacketsActions/p_search.h"

class SearchManager : public QObject
{
    Q_OBJECT
    int rowCount;
    QList<SearchThread*> searchThreads;
    QMutex mutex;
    int countThread {QThread::idealThreadCount()};
    QList<bool> results;
    QString sort;
    QString table;
    QString where;
    QString column;
    QString searchText;
    ModelTypes modelTypes;
    QSharedPointer<SOCKET> clientSocket;
    QString sampleSearchQuery;
    bool found;

public:
    explicit SearchManager(const QString& sort, const QString& table, const QString& where, const QString& column, const QString& searchText, ModelTypes modelTypes, QSharedPointer<SOCKET> clientSocket);
    void launchSearch();
    ~SearchManager() {qDebug() << "Менеджер поиска удалён";}
    void handleSearchResult(bool found, const QString &row);
    int getRowCount();

private:
    void requestRowCount();

signals:
    void onSearchFinished(QSharedPointer<SOCKET>, ModelTypes, bool, int);
};

#endif // SEARCHMANAGER_H
