#include "searchthread.h"

SearchThread::SearchThread(QString query, bool &found) :
    query(query),
    found(found)
{

}

void SearchThread::run()
{
    QSharedPointer<DatabaseManager> databaseManager(new DatabaseManager());
    QString result = databaseManager->executeQuery(query);

    if(!found)
    {
        if(!result.isEmpty())
        {
            found = true;;
            emit signalResultSearch(found, result);
            return;
        }
        emit signalResultSearch(false, "");
    }
}
