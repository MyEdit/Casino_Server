#ifndef COMMANDNOTIFICATION_H
#define COMMANDNOTIFICATION_H

#include <Commands/command.h>
#include <Commands/commandmanager.h>
#include <Network/networkserver.h>
#include <Network/PacketsActions/p_notification.h>
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
