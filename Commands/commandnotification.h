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
    void execute(QStringList args) override;
    QString getCommand() override;
    QString getHelpInfo() override;
};

#endif // COMMANDNOTIFICATION_H
