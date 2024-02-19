#include <Network/networkserver.h>
#include <Commands/commandmanager.h>

NetworkServer network;
CommandManager commandManager;

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    network.init();
    commandManager.init();
    network.startListening();
    return 0;
}
