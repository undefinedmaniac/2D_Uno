#ifndef DECK_H
#define DECK_H

#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <memory>

#include "card.h"

using std::vector;
using std::mt19937;
using std::shared_ptr;
using std::shuffle;
using std::chrono::system_clock;

namespace game
{

class Deck
{
public:
    Deck(mt19937 &engine);
    Deck(unsigned int seed = 0);

    void placeCard(const Card* card);
    const Card* takeCard();
    int count() const;

    void shuffle();

private:
    vector<const Card*> cards_;
    mt19937 randomEngine_;
};

}

#endif // DECK_H
