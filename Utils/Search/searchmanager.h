#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H

#include <QThread>
#include <QRegularExpression>
#include "Utils/Search/searchthread.h"
#include "network/networkserver.h"
#include "network/PacketsActions/p_search.h"

class SearchThread;

class SearchManager : public QThread
{
    Q_OBJECT
    int rowCount;
    QList<QSharedPointer<SearchThread>> searchThreads;
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
    ~SearchManager() {qDebug() << "ss";}

private:
    void run() override;
    void requestRowCount();
    void terminateSearchThreads();
    void handleSearchResult(bool found, const QString &row);
    void notFound(bool found);

signals:
    void onSearchFinished(QSharedPointer<SOCKET>, ModelTypes, bool, int);
};

#endif // SEARCHMANAGER_H
