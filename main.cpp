#include <QCoreApplication>
#include <Network/networkserver.h>
#include <Commands/commandmanager.h>

NetworkServer network;
CommandManager commandManager;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    network.init();
    commandManager.init();
    network.startListening();

    return app.exec();
}
