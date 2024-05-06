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

typedef std::function<void(void)> Func;

class Ticker : public QObject {
    Q_OBJECT

private:
    static QMutex tickerMutex;
    static QSharedPointer<std::thread> tickerThread;
    static QList<QWeakPointer<std::function<void(void)>>> callbacks;

    static void runTickerLoop();

public:
    typedef std::function<void(void)> Func;

    static void addListener(const QWeakPointer<std::function<void()>> callback);
    static void init();
};

#endif // TICKER_H
