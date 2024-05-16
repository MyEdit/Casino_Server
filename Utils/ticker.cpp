#include "ticker.h"

QMutex Ticker::tickerMutex;
QSharedPointer<std::thread> Ticker::tickerThread;
QList<QWeakPointer<Func>> Ticker::callbacks;

void Ticker::addListener(const QWeakPointer<Func> callback)
{
    QMutexLocker locker(&tickerMutex);
    callbacks.append(callback);
}

void Ticker::init()
{
    if (tickerThread.isNull())
        tickerThread = QSharedPointer<std::thread>(new std::thread(&Ticker::runTickerLoop));
}

void Ticker::runTickerLoop()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        for(auto func : getValidCallbacks())
            (*func)();
    }
}

QList<QSharedPointer<Func>> Ticker::getValidCallbacks()
{
    QList<QSharedPointer<Func>> validCallbacks;
    QMutexLocker locker(&tickerMutex);

    callbacks.erase(std::remove_if(callbacks.begin(), callbacks.end(), [](const QWeakPointer<Func>& func) {
        return !func.lock();
    }), callbacks.end());

    for(auto func : callbacks)
    {
        if(auto sharedFunc = func.lock())
            validCallbacks.append(sharedFunc);
    }

    return validCallbacks;
}
