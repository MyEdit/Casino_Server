#ifndef COMMAND_H
#define COMMAND_H

#include <vector>
#include <string>
#include <QString>
#include <Utils/Message.h>

class Command
{
public:
    virtual void execute(std::vector<std::string> args) = 0;
    virtual std::string getCommand() = 0;
    virtual std::string getHelpInfo() = 0;

    static void printUnknownCommandMessage()
    {
        Message::logInfo("Unknown command, type /help for help");
    }

    static void printErrorMessage()
    {
        Message::logError("An error occurred while executing the command");
    }

    static QString getTextAfterIndex(std::vector<std::string>& args, int index)
    {
        QString text{};
        for (size_t i = index; i < args.size(); ++i)
        {
            text += QString::fromStdString(args[i]) + " ";
        }
        text.chop(1);
        return text;
    }
};

#endif // COMMAND_H
