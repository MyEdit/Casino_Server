#ifndef COMMANDBAN_H
#define COMMANDBAN_H

#include <Commands/command.h>
#include <Commands/commandmanager.h>
#include <Database/databasemanager.h>

class CommandBan : public Command
{
public:
    void execute(std::vector<std::string> args) override;
    std::string getCommand() override;
    std::string getHelpInfo() override;
};

#endif // COMMANDBAN_H
