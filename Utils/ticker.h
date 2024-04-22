#ifndef TICKER_H
#define TICKER_H

#include <QObject>
#include <thread>
#include <chrono>
#include <QThread>
#include <Utils/Message.h>
#include <QMutex>
#include <QMap>
#include <functional>

class Ticker : public QObject {
    Q_OBJECT

private:
    static QMutex tickerMutex;
    static QSharedPointer<std::thread> tickerThread;
    static QMap<QSharedPointer<QObject>, std::function<void()>> callbacks;

    static void runTickerLoop();

public:
    static void addListener(QSharedPointer<QObject> object, const std::function<void()>& callback);
    static void start();
};

#endif // TICKER_H
