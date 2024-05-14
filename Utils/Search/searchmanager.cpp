#include "searchmanager.h"

SearchManager::SearchManager(const QString& sort, const QString& table, const QString& where, const QString& column, const QString& searchText, ModelTypes modelTypes, QSharedPointer<SOCKET> clientSocket) :
    sort(sort),
    table(table),
    where(where),
    column(column),
    searchText(searchText),
    modelTypes(modelTypes),
    clientSocket(clientSocket)
{
    requestRowCount();
    sampleSearchQuery = "SELECT numbered_rows.row FROM (SELECT ROW_NUMBER() OVER (%1) AS row, * FROM %2 LIMIT %3 OFFSET %4) AS numbered_rows WHERE numbered_rows.%5 LIKE '%6'";
    sampleSearchQueryWhere = "SELECT numbered_rows.row FROM (SELECT ROW_NUMBER() OVER (%1) AS row, * FROM %2 WHERE %3 LIMIT %4 OFFSET %5) AS numbered_rows WHERE numbered_rows.%6 LIKE '%7'";
    found = false;
}

void SearchManager::launchSearch()
{
    int rowsPerThread = rowCount / countThread + 1; // Кол-во строк на каждый поток

    for (int i = 0; i < countThread; ++i)
    {
        int offset = i * rowsPerThread;
        int limit = rowsPerThread;

        QString searchQuery;

        if(where.isEmpty())
            searchQuery = sampleSearchQuery.arg(sort).arg(table).arg(limit).arg(offset).arg(column).arg(searchText);
        else
            searchQuery = sampleSearchQueryWhere.arg(sort).arg(table).arg(where).arg(limit).arg(offset).arg(column).arg(searchText);

        QThread* thread = new QThread;
        SearchThread* worker = new SearchThread(searchQuery, found);
        worker->moveToThread(thread);

        connect(thread, &QThread::started, worker, &SearchThread::process);
        connect(worker, &SearchThread::finished, thread, &QThread::quit);
        connect(worker, &SearchThread::finished, worker, &SearchThread::deleteLater);
        connect(worker, &SearchThread::signalResultSearch, this, &SearchManager::handleSearchResult,  Qt::DirectConnection);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);

        searchThreads.append(worker);

        thread->start();
    }
}

void SearchManager::requestRowCount()
{
    QSharedPointer<DatabaseManager> databaseManager(new DatabaseManager());
    QString query;
    if(!where.isEmpty())
        query = "SELECT COUNT(*) FROM " + table + " WHERE " + where;
    else
        query = "SELECT COUNT(*) FROM " + table;
    rowCount = databaseManager->executeQuery(query).toInt();
}

void SearchManager::handleSearchResult(bool found, const QString& row)
{
    mutex.lock();
    results.push_back(found);

    if(results.size() == searchThreads.size() || found)
    {
        if(results.size() == searchThreads.size())
            P_Search::sendNotFound(clientSocket);
        else
            P_Search::sendResult(clientSocket, row, modelTypes);

        mutex.unlock();
        delete this;
        return;
    }
    mutex.unlock();
}

int SearchManager::getRowCount()
{
    return rowCount;
}
