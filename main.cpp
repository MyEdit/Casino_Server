#include <Network/networkserver.h>

NetworkServer network;

int main()
{
    network.init();
    network.startListening();

    return 0;
}
