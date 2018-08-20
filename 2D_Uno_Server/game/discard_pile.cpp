#include "discard_pile.h"

namespace game
{

void DiscardPile::placeCard(const Card* card)
{
    cards_.push_back(card);
}

const Card* DiscardPile::getTopCard() const
{
    if (!cards_.empty())
        return cards_.back();

    return nullptr;
}

vector<const Card*> DiscardPile::takeLowerCards()
{
    if (cards_.size() >= 2) {
        const Card* card = getTopCard();
        cards_.pop_back();

        vector<const Card*> removed_cards = cards_;

        cards_.clear();
        placeCard(card);

        return removed_cards;
    }

    return vector<const Card*>();
}

vector<const Card*> DiscardPile::takeAllCards()
{
    vector<const Card*> cards = cards_;
    cards_.clear();

    return cards;
}

int DiscardPile::count() const
{
    return static_cast<int>(cards_.size());
}

}
