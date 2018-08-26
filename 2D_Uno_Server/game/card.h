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
    unsigned int getId() const;

private:
    static unsigned int idCount_;

    const CardColor color_;
    const CardType type_;
    const unsigned int id_;
};

}

#endif // CARD_H
