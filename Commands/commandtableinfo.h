#ifndef COMMANDTABLEINFO_H
#define COMMANDTABLEINFO_H

#include <Commands/command.h>
#include <Commands/commandmanager.h>
#include <Games/table.h>

class CommandTableInfo : public Command
{
public:
    void execute(const QStringList &args) override;
    QString getCommand() override;
    QString getHelpInfo() override;
};

#endif // COMMANDTABLEINFO_H
