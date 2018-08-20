#ifndef DISCARD_PILE_H
#define DISCARD_PILE_H

#include <vector>

#include "card.h"

using std::vector;

namespace game
{

class DiscardPile
{
public:
    void placeCard(const Card* card);
    const Card* getTopCard() const;
    vector<const Card*> takeLowerCards();
    vector<const Card*> takeAllCards();
    int count() const;

private:
    vector<const Card*> cards_;
};

}

#endif // DISCARD_PILE_H
