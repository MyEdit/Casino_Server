#include "blackjack.h"

BlackJack::BlackJack()
{

}

//Особые проверки самой игры на коннект игрока
//В зависимости от игры, условия наверное могут разниться. В случае если условий нет - возвращаем true
bool BlackJack::canPlayerJoin(QSharedPointer<Player> player)
{
    return true;
}

bool BlackJack::canStartGame()
{
    if (this->players.size() < this->minPlayers)
        return false;

    return true;
}
