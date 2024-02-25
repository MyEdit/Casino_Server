#ifndef COMMANDHELP_H
#define COMMANDHELP_H

#include <Commands/command.h>
#include <Commands/commandmanager.h>
#include <Utils/Message.h>

class CommandHelp : public Command
{
public:
    void execute(QStringList args) override;
    QString getCommand() override;
    QString getHelpInfo() override;
};

#endif // COMMANDHELP_H
