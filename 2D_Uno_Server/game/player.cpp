#include "player.h"

namespace game
{

Player::Player(PrivatePlayer* player) : player_(player)
{
}

std::string Player::getName() const
{
    return player_->getName();
}

const unordered_set<const Card*> &Player::getCards() const
{
    return player_->getCards();
}

}
