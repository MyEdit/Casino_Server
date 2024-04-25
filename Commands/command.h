#ifndef COMMAND_H
#define COMMAND_H

#include <vector>
#include <string>
#include <QString>
#include <Utils/Message.h>

class Command
{
public:
    virtual ~Command() {}
    virtual void execute(const QStringList& args) = 0;
    virtual QString getCommand() = 0;
    virtual QString getHelpInfo() = 0;

    static void printUnknownCommandMessage()
    {
        Message::logInfo("Unknown command, type /help for help");
    }

    static void printErrorMessage()
    {
        Message::logError("An error occurred while executing the command");
    }

    static QString getTextAfterIndex(const QStringList &args, const int& index)
    {
        QString text{};
        for (int i = index; i < args.size(); ++i)
        {
            text += args[i] + " ";
        }
        text.chop(1);
        return text;
    }
};

#endif // COMMAND_H
