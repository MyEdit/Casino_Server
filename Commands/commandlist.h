#ifndef COMMANDLIST_H
#define COMMANDLIST_H

#include <Commands/command.h>
#include <Commands/commandmanager.h>
#include <Network/networkserver.h>
#include <Utils/Message.h>

class CommandList : public Command
{
public:
    void execute(QStringList args) override;
    QString getCommand() override;
    QString getHelpInfo() override;
};

#endif // COMMANDLIST_H
