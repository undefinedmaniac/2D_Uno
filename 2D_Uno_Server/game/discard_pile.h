#ifndef DISCARD_PILE_H
#define DISCARD_PILE_H

#include <vector>

#include "card.h"

using std::vector;

namespace game
{

/**
 * @brief The DiscardPile class
 *
 * Represents a discard pile. Cards are added to the top of the pile and
 * removed from the bottom. The top card is visible at all times.
 */
class DiscardPile
{
public:

    /**
     * @brief placeCard Add a card to the top of the discard pile
     * @param card The card
     */
    void placeCard(const Card *card);

    /**
     * @brief getTopCard Get the top card
     * @return The card
     */
    const Card *getTopCard() const;

    /**
     * @brief takeLowerCards Remove all cards except the top card
     * @return The cards that were removed
     */
    vector<const Card*> takeLowerCards();

    /**
     * @brief takeAllCards Remove all cards
     * @return The cards that were removed
     */
    vector<const Card*> takeAllCards();

    /**
     * @brief count Get the number of cards in the discard pile
     * @return The number of cards
     */
    unsigned int count() const;

private:
    vector<const Card*> cards_;
};

}

#endif // DISCARD_PILE_H
