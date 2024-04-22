#include "ticker.h"

QMutex Ticker::tickerMutex;
QSharedPointer<std::thread> Ticker::tickerThread;
QMap<QSharedPointer<QObject>, std::function<void()>> Ticker::callbacks;

void Ticker::addListener(QSharedPointer<QObject> object, const std::function<void ()> &callback) {
    QMutexLocker locker(&tickerMutex);
    callbacks[object] = callback;
}

void Ticker::start() {
    if (tickerThread != nullptr)
        tickerThread = QSharedPointer<std::thread>(new std::thread(&Ticker::runTickerLoop));
}

void Ticker::runTickerLoop() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        tickerMutex.lock();
        for (const auto& pair : callbacks) {
            pair();
        }
        tickerMutex.unlock();
    }
}
