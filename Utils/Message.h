#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDebug>
#include <QTime>

class Message
{
public:
    template <typename T>
    static void logInfo(const T& value)
    {
        qDebug() << qPrintable(getTime()) << "[INFO]" << qPrintable(value);
    }

    template <typename T>
    static void logWarn(const T& value)
    {
        qDebug() << qPrintable(getTime()) << "[WARN]" << qPrintable(value);
    }

    template <typename T>
    static void logError(const T& value)
    {
        qDebug() << qPrintable(getTime()) << "[ERROR]" << qPrintable(value);
    }

    //Может быть в будущем если примерные утилитарные функции не связанные меж собой скопяться, их можно объединить в новом классе Utils
    static QString getTime()
    {
        return QTime::currentTime().toString("[HH:mm:ss]");
    }
};

#endif // MESSAGE_H
