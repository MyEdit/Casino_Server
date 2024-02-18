#ifndef COMMANDNOTIFICATION_H
#define COMMANDNOTIFICATION_H

#include <Commands/command.h>
#include <Commands/commandmanager.h>
#include <network/networkserver.h>
#include <Utils/Message.h>
#include <QString>

class CommandNotification : public Command
{
public:
    void execute(std::vector<std::string> args) override;
    std::string getCommand() override;
    std::string getHelpInfo() override;
};

#endif // COMMANDNOTIFICATION_H
