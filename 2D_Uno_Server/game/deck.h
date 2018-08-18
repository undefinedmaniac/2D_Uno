#ifndef DECK_H
#define DECK_H

#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <memory>

#include "card.h"

using std::vector;
using std::default_random_engine;
using std::unique_ptr;
using std::shuffle;
using std::chrono::system_clock;

namespace game
{

class Deck
{
public:
    Deck(default_random_engine& engine);
    Deck();

    ~Deck();

    void placeCard(const Card* card);
    const Card* takeCard();
    int count() const;

    void shuffle();

private:
    vector<const Card*> cards_;
    unique_ptr<default_random_engine> randomEngine_;
    const bool usingOutsideRandomEngine_;
};

}

#endif // DECK_H
