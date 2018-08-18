#include "card.h"

namespace game
{

Card::Card(CardColor color, CardType type) : color_(color), type_(type)
{
}

CardColor Card::getColor() const
{
    return color_;
}

CardType Card::getType() const
{
    return type_;
}

}

