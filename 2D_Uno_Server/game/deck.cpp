#include "deck.h"

namespace game
{

Deck::Deck(mt19937 &engine) : randomEngine_(engine)
{
}

void Deck::placeCard(const Card *card)
{
    cards_.push_back(card);
}

const Card *Deck::takeCard()
{
    const Card* card = cards_.back();
    cards_.pop_back();
    return card;
}

unsigned int Deck::count() const
{
    return static_cast<unsigned int>(cards_.size());
}

void Deck::shuffle()
{
    std::shuffle(cards_.begin(), cards_.end(), randomEngine_);
}

}
