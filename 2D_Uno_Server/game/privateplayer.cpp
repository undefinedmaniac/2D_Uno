#include "privateplayer.h"

namespace game
{

PrivatePlayer::PrivatePlayer(const string& name) : name_(name)
{
}

std::string PrivatePlayer::getName() const
{
    return name_;
}

const PrivatePlayer::CardMap& PrivatePlayer::getCards() const
{
    return cards_;
}

void PrivatePlayer::addCard(const Card* card)
{
    cards_[card->getId()] = card;
}

bool PrivatePlayer::removeCard(const Card* card)
{
    PrivatePlayer::CardMap::iterator position = cards_.find(card->getId());

    if (position != cards_.end()) {
        cards_.erase(position);
        return true;
    }

    return false;
}

}
