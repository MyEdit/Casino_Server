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
    found = false;
}

void SearchManager::launchSearch()
{
    int rowsPerThread = rowCount / countThread + 1; // Кол-во строк на каждый поток

    for (int i = 0; i < countThread; ++i)
    {
        int offset = i * rowsPerThread;
        int limit = rowsPerThread;

        QString searchQuery = sampleSearchQuery.arg(sort).arg(table).arg(limit).arg(offset).arg(column).arg(searchText);
        QSharedPointer<SearchThread> thread(new SearchThread(searchQuery, found));

        connect(thread.get(), &SearchThread::signalResultSearch, this, &SearchManager::handleSearchResult, Qt::DirectConnection);
        searchThreads.append(thread);

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

void SearchManager::terminateSearchThreads()
{
    QMutableListIterator<QSharedPointer<SearchThread>> it(searchThreads);
    while (it.hasNext())
    {
        QSharedPointer<SearchThread> thread = it.next();
        if (thread.data() != QThread::currentThread())
        {
            thread->quit();
            thread->wait();
            it.remove();
            thread->disconnect();
        }
    }
}

void SearchManager::handleSearchResult(bool found, const QString& row)
{
    QMutexLocker locker(&mutex);
    results.push_back(found);

    if(results.size() == searchThreads.size() || found)
    {
        if(results.size() == searchThreads.size())
            P_Search::sendNotFound(clientSocket);
        else
            P_Search::sendResult(clientSocket, row, modelTypes);

        terminateSearchThreads();

        quit();
        deleteLater();
    }
}

void SearchManager::run()
{
    launchSearch();
}
