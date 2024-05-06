#include "ticker.h"

QMutex Ticker::tickerMutex;
QSharedPointer<std::thread> Ticker::tickerThread;
QList<QWeakPointer<std::function<void()>>> Ticker::callbacks;

void Ticker::addListener(const QWeakPointer<std::function<void()>> callback)
{
    QMutexLocker locker(&tickerMutex);
    callbacks.append(callback);
}

void Ticker::init()
{
    if (tickerThread.isNull())
        tickerThread = QSharedPointer<std::thread>(new std::thread(&Ticker::runTickerLoop));
}

void Ticker::runTickerLoop() {
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        tickerMutex.lock();

        callbacks.erase(std::remove_if(callbacks.begin(), callbacks.end(), [](const QWeakPointer<std::function<void()>>& func) {
            return !func.lock();
        }), callbacks.end());

        Message::logWarn(QString::number(callbacks.size()));

        for(auto func : callbacks)
            (*func.lock())();

        tickerMutex.unlock();
    }
}
