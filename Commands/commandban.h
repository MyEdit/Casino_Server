#ifndef COMMANDBAN_H
#define COMMANDBAN_H

#include <Commands/command.h>
#include <Commands/commandmanager.h>
#include <Database/databasemanager.h>

class CommandBan : public Command
{
public:
    void execute(QStringList args) override;
    QString getCommand() override;
    QString getHelpInfo() override;
};

#endif // COMMANDBAN_H
