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

const PrivatePlayer::CardMap &Player::getCards() const
{
    return player_->getCards();
}

unsigned int Player::cardCount() const
{
    return player_->cardCount();
}

}
