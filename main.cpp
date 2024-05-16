#include <QCoreApplication>
#include <network/networkserver.h>
#include <Commands/commandmanager.h>
#include <Utils/ticker.h>
#include <Utils/updater.h>
#include <Games/blackjack.h>

NetworkServer network;
CommandManager commandManager;
Updater updater;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Ticker::init();
    network.init();
    commandManager.init();

    //Register default games
    Game::registerGame(QSharedPointer<BlackJack>(new BlackJack()));

    updater.init();
    network.startListening();

    return app.exec();
}
