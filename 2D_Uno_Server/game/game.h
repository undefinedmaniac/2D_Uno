#ifndef GAME_H
#define GAME_H

#include <unordered_map>
#include <deque>
#include <memory>
#include <iostream>

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
    enum class Result {
        Success,          // The operation was successful

        NotEnoughPlayers, // Not enough players to complete the operation

        GameStateInvalid, // The game is not in the correct state for the
                          // operation (running / stopped)


    };

    Game(unsigned int seed = 0, IGameObserver* observer = nullptr);

    void setObserver(IGameObserver& observer);

    Result start();
    void reset();

    Player *addPlayer(const string &name);
    void removePlayer(Player *player);

    const Card *getTopCard() const;
    const Card *drawCard();
    bool playCard(const Card *card);
    bool playCard(const Card *card, CardColor newColor);

    Player *getCurrentPlayer();
    bool endTurn();
    bool isTurnDirectionReversed() const;

private:
    void buildDeck();
    void shuffleDeck();

    void playCardHelper(const Card *card);
    vector<const Card*> drawCardHelper(Player *player, int nCards);
    unsigned int giveNextPlayerCards(int nCards);

    void startNextTurn();
    void toggleTurnDirection();

    void endGame(Player *winner);

    IGameObserver* observer_;

    mt19937 randomEngine_;

    vector<unique_ptr<Card>> cards_;
    vector<unique_ptr<Player>> players_;
    vector<unique_ptr<PrivatePlayer>> privatePlayers_;

    Deck deck_;
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
