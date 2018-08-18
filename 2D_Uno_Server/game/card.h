#ifndef CARD_H
#define CARD_H

#include "enums/card_color.h"
#include "enums/card_type.h"

namespace game
{

class Card
{
public:
    Card(CardColor color, CardType type);

    CardColor getColor() const;
    CardType getType() const;

private:
    const CardColor color_;
    const CardType type_;
};

}

#endif // CARD_H
