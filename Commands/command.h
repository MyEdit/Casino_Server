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
    static QString getUnknownCommandMessage()
    {
        return "Unknown command, type /help for help";
    }
    static void printErrorMessage()
    {
        Message::logError("An error occurred while executing the command");
    }
};

#endif // COMMAND_H
