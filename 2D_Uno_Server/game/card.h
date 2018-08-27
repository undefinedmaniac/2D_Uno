#ifndef CARD_H
#define CARD_H

#include "enums/card_color.h"
#include "enums/card_type.h"

namespace game
{

/**
 * @brief The Card class
 *
 * Represents a single card containing a color, type, and unique ID.
 * Every card that is constructed will have a unique ID and is uncopyable.
 */
class Card
{
public:
    /**
     * @brief Card Construct a new card with the given color and type
     * @param color The card's color
     * @param type The card's type
     */
    Card(CardColor color, CardType type);

    Card(const Card &other) = delete;
    Card &operator=(const Card &other) = delete;

    /**
     * @brief getColor
     * @return The card's color
     */
    CardColor getColor() const;

    /**
     * @brief getType
     * @return The card's type
     */
    CardType getType() const;

    /**
     * @brief getId
     * @return The card's unique ID
     */
    unsigned int getId() const;

private:
    static unsigned int idCount_;

    const CardColor color_;
    const CardType type_;
    const unsigned int id_;
};

}

#endif // CARD_H
