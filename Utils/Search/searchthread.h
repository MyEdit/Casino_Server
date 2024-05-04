#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include <QThread>
#include "Database/databasemanager.h"

class SearchThread : public QThread
{
    Q_OBJECT
    QString query;
    bool& found;

public:
    SearchThread(QString query, bool& found);
    ~SearchThread() {static int i=1; qDebug() << i++;}
private:
    void run() override;

signals:
    void signalResultSearch(bool, QString);
};

#endif // SEARCHTHREAD_H
