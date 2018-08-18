#include "deck.h"

namespace game
{

Deck::Deck(default_random_engine& engine) : randomEngine_(&engine),
    usingOutsideRandomEngine_(true)
{
}

Deck::Deck() : usingOutsideRandomEngine_(false)
{
    // obtain a time-based seed:
    long long seed = system_clock::now().time_since_epoch().count();
    randomEngine_ = unique_ptr<default_random_engine> (new default_random_engine(seed));
}

Deck::~Deck()
{
    if (usingOutsideRandomEngine_)
        randomEngine_.release();
}

void Deck::placeCard(const Card* card)
{
    cards_.push_back(card);
}

const Card* Deck::takeCard()
{
    const Card* card = cards_.back();
    cards_.pop_back();
    return card;
}

int Deck::count() const
{
    return static_cast<int>(cards_.size());
}

void Deck::shuffle()
{
    std::shuffle(cards_.begin(), cards_.end(), *randomEngine_);
}

}
