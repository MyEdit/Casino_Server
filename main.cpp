#include <QCoreApplication>
#include <network/networkserver.h>
#include <Commands/commandmanager.h>
#include <Utils/ticker.h>
#include <Games/blackjack.h>

NetworkServer network;
CommandManager commandManager;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    //Register default games
    Game::registerGame(QSharedPointer<BlackJack>(new BlackJack()));

    Ticker::init();
    network.init();
    commandManager.init();
    network.startListening();

    return app.exec();
}
