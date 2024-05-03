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
    ~SearchThread() {qDebug() << "aaa";}
private:
    void run() override;

signals:
    void signalResultSearch(bool, QString);
    void signalNotFound(bool);
};

#endif // SEARCHTHREAD_H
