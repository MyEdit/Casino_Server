#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include <QObject>
#include <QThread>
#include "Database/databasemanager.h"

class SearchThread : public QObject
{
    Q_OBJECT
    QString query;
    bool& found;

public:
    SearchThread(QString query, bool& found);
    ~SearchThread()
    {
        static int i = 1;
        qDebug() << "Поток поиска " << i++ << "/" << QThread::idealThreadCount() << " удалён";
    }

    void process();

signals:
    void signalResultSearch(bool, QString);
    void finished();
};

#endif // SEARCHTHREAD_H
