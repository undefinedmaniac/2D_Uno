#ifndef CARD_TYPE_H
#define CARD_TYPE_H

namespace game
{

enum class CardType : int
{
    Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Skip, Reverse,
    DrawTwo, Wildcard, DrawFourWildcard
};

}

#endif // CARD_TYPE_H
