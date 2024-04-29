#include "blackjack.h"

BlackJack::BlackJack()
{

}

//Особые проверки самой игры на коннект игрока
//В зависимости от игры, условия наверное могут разниться (Например если делать поуровневый доступ игрокам и играм)
//В случае если условий нет - возвращаем true
bool BlackJack::canPlayerJoin(QSharedPointer<Player> player)
{
    return player != nullptr; //Всандалил сюда player != nullptr чтобы QT не давал варн за неиспользованный параметр player
}

bool BlackJack::canStartGame()
{
    if (this->players.size() < this->minPlayers)
        return false;

    return true;
}
