#ifndef IGAMEOBSERVER_H
#define IGAMEOBSERVER_H

namespace game
{

class Card;
class Player;
enum class CardColor;

class IGameObserver
{
public:
    virtual ~IGameObserver();

    virtual void gameStarted(const Card* firstCard,
                             CardColor startingColor) = 0;
    virtual void gameReset() = 0;
    virtual void gameEnded(Player* winner) = 0;

    virtual void playerCardCountChanged(Player* player, int newCount) = 0;

    virtual void deckShuffled() = 0;
    virtual void deckCountChanged(int newCount) = 0;
    virtual void discardPileCountChanged(int newCount) = 0;
    virtual void cardPlayed(Player* player, const Card* card) = 0;
    virtual void cardPlayed(Player* player, const Card* card,
                            CardColor newColor) = 0;
    virtual void cardDrawn(Player* player, const Card* card) = 0;

    virtual void playerTurnStarted(Player* player) = 0;
    virtual void playerTurnEnded(Player* player) = 0;
    virtual void turnSkipped(Player* player) = 0;
    virtual void turnDirectionChanged(bool isReversed) = 0;

    virtual void playerCalledUno(Player* player);
    virtual void playerForgotToCallUno(Player* caller, Player* target);
};

}

#endif // IGAMEOBSERVER_H
