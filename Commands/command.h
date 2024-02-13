#ifndef COMMAND_H
#define COMMAND_H

#include <vector>
#include <string>
#include <QString>

class Command
{
public:
    virtual void execute(std::vector<std::string> args) = 0; //Метод с логикой работы команды
    virtual std::string getCommand() = 0; //Метод возвращающий команду
    static QString getErrorMessage()
    {
        return "Unknown command, type /help for help";
    }
};

#endif // COMMAND_H
