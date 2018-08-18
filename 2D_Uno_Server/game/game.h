#ifndef GAME_H
#define GAME_H

#include <unordered_map>
#include <deque>

#include "player.h"
#include "deck.h"
#include "discard_pile.h"

#include "interfaces/igameobserver.h"

using std::unordered_map;
using std::pair;
using std::deque;

namespace game
{

class Game
{
public:
    Game(IGameObserver* observer = nullptr);

    ~Game();

    void setObserver(IGameObserver& observer);

    void startGame();
    void resetGame();

    Player* addPlayer(const string& name);
    void removePlayer(Player* player);

    const Card* getTopCard() const;
    bool drawCard(Player* player);
    bool playCard(Player* player, const Card* card);
    bool playCard(Player* player, const Card* card, CardColor newColor);

    Player* getCurrentPlayer();
    bool isTurnDirectionReversed();

private:
    void shuffleDeck();

    bool drawCardHelper(Player* player, int nCards);
    bool giveNextPlayerCards(int nCards);

    void advancePlayerTurn();
    void advancePlayerTurnHelper();
    void skipPlayerTurn();
    void changeTurnDirection();

    IGameObserver* observer_;

    Deck deck_;
    DiscardPile discardPile_;
    CardColor wildcardColor_;

    unordered_map<Player*, PrivatePlayer*> playerMap_;

    Player* currentPlayer_ = nullptr;
    deque<Player*> turnList_;
    bool isTurnDirectionReversed_ = false;

    bool isGameRunning = false;
};

}

#endif // GAME_H
