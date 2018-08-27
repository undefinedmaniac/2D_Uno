#ifndef DECK_H
#define DECK_H

#include <vector>
#include <algorithm>

#include "card.h"

using std::vector;
using std::mt19937;
using std::shuffle;

namespace game
{

/**
 * @brief The Deck class
 *
 * Represents a deck of cards. Cards can be added or removed from the top of
 * the deck one at a time. Additionally, the deck can be shuffled according
 * to the random engine provided.
 */
class Deck
{
public:
    Deck(mt19937 &engine);

    /**
     * @brief placeCard Add a card to the top of the deck
     * @param card The card
     */
    void placeCard(const Card *card);

    /**
     * @brief takeCard Remove a card from the top of the deck
     * @return The card
     */
    const Card *takeCard();

    /**
     * @brief count Get the number of cards in the deck
     * @return The number of cards
     */
    unsigned int count() const;

    /**
     * @brief shuffle Shuffle the cards using the provied mt19937 engine
     * for randomization
     */
    void shuffle();

private:
    vector<const Card*> cards_;
    mt19937 &randomEngine_;
};

}

#endif // DECK_H
