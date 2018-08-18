#ifndef GAME_H
#define GAME_H

#include <unordered_map>
#include <deque>
#include <memory>

#include "player.h"
#include "deck.h"
#include "discard_pile.h"

#include "interfaces/igameobserver.h"

using std::unordered_map;
using std::pair;
using std::deque;
using std::unique_ptr;

namespace game
{

typedef unordered_map<Player*, PrivatePlayer*> PlayerMap;

class Game
{
public:
    Game(IGameObserver* observer = nullptr);

    void setObserver(IGameObserver& observer);

    void startGame();
    void resetGame();

    Player* addPlayer(const string& name);
    void removePlayer(Player* player);

    const Card* getTopCard() const;
    bool drawCard();
    bool playCard(const Card* card);
    bool playCard(const Card* card, CardColor newColor);

    Player* getCurrentPlayer();
    bool endTurn();
    bool isTurnDirectionReversed();

private:
    void shuffleDeck();

    int drawCardHelper(Player* player, int nCards);
    int giveNextPlayerCards(int nCards);

    void advancePlayerTurn();
    void startNextPlayerTurn();
    void startNextPlayerTurnHelper();
    void endPlayerTurn();
    void endPlayerTurnHelper();
    void skipPlayerTurn();
    void changeTurnDirection();

    IGameObserver* observer_;

    Deck deck_;
    DiscardPile discardPile_;
    CardColor wildcardColor_;

    unordered_set<unique_ptr<Player>> players_;
    unordered_set<unique_ptr<PrivatePlayer>> privatePlayers_;
    PlayerMap playerMap_;
    bool hasPlayerPlayed_;

    Player* currentPlayer_ = nullptr;
    deque<Player*> turnList_;
    bool isTurnDirectionReversed_ = false;
    bool skipNextPlayerTurn_ = false;

    bool isGameRunning = false;
};

}

#endif // GAME_H
