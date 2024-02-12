#include <Network/networkserver.h>
#include <Commands/commandmanager.h>

NetworkServer network;
CommandManager commandManager;

int main()
{
    network.init();
    commandManager.init();
    network.startListening();
    return 0;
}
