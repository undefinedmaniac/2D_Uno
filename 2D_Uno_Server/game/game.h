#ifndef GAME_H
#define GAME_H

#include <unordered_map>
#include <deque>
#include <memory>

#include "player.h"
#include "deck.h"
#include "discard_pile.h"
#include "game_turn_manager.h"

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

    bool startGame();
    void resetGame();

    Player* addPlayer(const string& name);
    void removePlayer(Player* player);

    const Card* getTopCard() const;
    bool drawCard();
    bool playCard(const Card* card);
    bool playCard(const Card* card, CardColor newColor);

    Player* getCurrentPlayer();
    bool endTurn();
    bool isTurnDirectionReversed() const;

private:
    void buildDeck();
    void shuffleDeck();

    void playCardHelper(const Card* card);
    int drawCardHelper(Player* player, int nCards);
    int giveNextPlayerCards(int nCards);

    void startNextTurn();
    void toggleTurnDirection();

    void endGame(Player* winner);

    IGameObserver* observer_;

    unique_ptr<default_random_engine> randomEngine_;

    vector<unique_ptr<Card>> cards_;
    vector<unique_ptr<Player>> players_;
    vector<unique_ptr<PrivatePlayer>> privatePlayers_;

    unique_ptr<Deck> deck_;
    DiscardPile discardPile_;
    CardColor wildcardColor_;

    PlayerMap playerMap_;
    bool hasPlayerPlayed_ = false;

    GameTurnManager turnManager_;
    bool skipNextPlayerTurn_ = false;

    bool isGameRunning_ = false;
};

}

#endif // GAME_H
