#include <QCoreApplication>
#include <network/networkserver.h>
#include <Commands/commandmanager.h>
#include <Utils/ticker.h>
#include <Utils/updater.h>
#include <Games/blackjack.h>

NetworkServer network;
CommandManager commandManager;
Updater updater;

void initIP()
{
    QString filename = "conf.txt";
    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Message::logError("отсутстувует conf.txt");
        exit(-1);
    }

    QTextStream in(&file);
    while(!in.atEnd())
    {
        network.ADDRESS = in.readLine();
        return;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    initIP();

    Ticker::init();
    network.init();
    commandManager.init();

    //Register default games
    Game::registerGame(QSharedPointer<BlackJack>(new BlackJack()));

    updater.init();
    network.startListening();

    return app.exec();
}
