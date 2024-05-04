#ifndef P_SEARCH_H
#define P_SEARCH_H

#include "network/networkserver.h"
#include "Utils/Search/searchmanager.h"

class P_Search : public QObject
{
    Q_OBJECT
public:
    static void getSearchQuary(QSharedPointer<SOCKET> clientSocket);
    static void sendResult(QSharedPointer<SOCKET> clientSocket, const QString& result, const ModelTypes modelTypes);
    static void sendNotFound(QSharedPointer<SOCKET> clientSocket);
};

#endif // P_SEARCH_H
