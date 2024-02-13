#ifndef COMMANDHELP_H
#define COMMANDHELP_H

#include <Commands/command.h>
#include <Utils/Message.h>

class CommandHelp : public Command
{
public:
    void execute(std::vector<std::string> args) override;
    std::string getCommand() override;
};

#endif // COMMANDHELP_H
