#include "card.h"

namespace game
{

unsigned int Card::idCount_ = 0;

Card::Card(CardColor color, CardType type) : color_(color), type_(type), id_(idCount_)
{
    idCount_++;
}

CardColor Card::getColor() const
{
    return color_;
}

CardType Card::getType() const
{
    return type_;
}

unsigned int Card::getId() const
{
    return id_;
}

}
