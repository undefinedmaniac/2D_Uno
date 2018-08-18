#include "discard_pile.h"

namespace game
{

void DiscardPile::placeCard(const Card* card)
{
    cards_.push_back(card);
}

const Card* DiscardPile::getTopCard() const
{
    return cards_.back();
}

vector<const Card*> DiscardPile::takeLowerCards()
{
    const Card* card = getTopCard();
    cards_.pop_back();

    vector<const Card*> removed_cards = cards_;

    cards_.clear();
    placeCard(card);

    return removed_cards;
}

int DiscardPile::count() const
{
    return static_cast<int>(cards_.size());
}

}
