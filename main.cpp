#include <QCoreApplication>
#include <Network/networkserver.h>
#include <Commands/commandmanager.h>
#include <Utils/ticker.h>

NetworkServer network;
CommandManager commandManager;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Ticker::start();
    network.init();
    commandManager.init();
    network.startListening();

    return app.exec();
}
