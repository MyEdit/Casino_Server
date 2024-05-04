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

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    //Register default games
    Game::registerGame(QSharedPointer<BlackJack>(new BlackJack()));

    Ticker::init();
    network.init();
    commandManager.init();
    network.startListening();

    return app.exec();
}
