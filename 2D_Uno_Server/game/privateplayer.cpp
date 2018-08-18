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

const unordered_set<const Card*>& PrivatePlayer::getCards() const
{
    return cards_;
}

void PrivatePlayer::addCard(const Card* card)
{
    cards_.insert(card);
}

bool PrivatePlayer::removeCard(const Card* card)
{
    unordered_set<const Card*>::iterator position = cards_.find(card);

    if (position != cards_.end()) {
        cards_.erase(position);
        return true;
    }

    return false;
}

}
