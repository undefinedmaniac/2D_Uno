#ifndef GAME_H
#define GAME_H

#include <unordered_map>
#include <deque>
#include <memory>
#include <iostream>

#include "utility/make_unique.h"

#include "player.h"
#include "deck.h"
#include "discard_pile.h"
#include "game_turn_manager.h"

#include "interfaces/igameobserver.h"

using std::unordered_map;
using std::pair;
using std::deque;
using std::unique_ptr;
using std::make_unique;

namespace game
{

typedef unordered_map<Player*, PrivatePlayer*> PlayerMap;

/**
 * @brief The Game class
 *
 * The game class provides an interface for playing Uno. The game is controlled
 * using the public methods of the class and activity can be watched by an
 * observer object which implements IGameObserver.
 */
class Game
{
public:
    /**
     * @brief The Result enum
     *
     * This enum contains error codes for the methods in this class.
     */
    enum class Result {
        Success,          // The operation was successful

        NotEnoughPlayers, // Not enough players to complete the operation

        GameStateInvalid, // The game is not in the correct state for the
                          // operation (running / stopped)

        PlayerNameTaken,  // The provided username is already taken

        InvalidAction,    // The attempted action violates game rules

        RanOutOfCards     // The operation failed because the deck ran out of
                          // cards
    };

    /**
     * @brief Game Construct a new Game class
     * @param seed The seed that will be used to randomize the game
     * @param observer An observer that can watch the internal activity of this
     * the object
     */
    Game(unsigned int seed = 0, IGameObserver* observer = nullptr);

    /**
     * @brief setObserver Set an observer object
     * @param observer The observer object
     */
    void setObserver(IGameObserver &observer);

    /**
     * @brief start Start the game
     * @return The result of the action
     *
     * Starts a game of Uno, dealing 7 cards to each player and drawing the
     * first card from the deck. This method will randomly select a starting
     * player and start their turn.
     *
     * Note: For this method to succeed, the game must not already be running
     * and there must be at least two players.
     */
    Result start();

    /**
     * @brief reset Reset the game
     *
     * Resets the game to a fresh state. Returns all cards to the deck and
     * shuffles. To start a new game, call start() again.
     *
     * Note: This method does NOT remove players from the game
     */
    void reset();

    /**
     * @brief addPlayer Adds a player to the game
     * @param name The username of the player
     * @param result The result of the action
     * @return On success, a pointer to the player. On failure, nullptr
     *
     * Adds a new player to the game. Player names must be unique - this
     * method will set result to Result::PlayerNameTaken if called with a
     * username that is already in use. Otherwise, result will be set to
     * Result::Success
     *
     * Additional information: if the game is in-progress, the player will be
     * delt 7 cards
     */
    Player *addPlayer(const string &name, Result *result = nullptr);

    /**
     * @brief removePlayer Removes a player from the game
     * @param player The player to remove
     *
     * Removes a player from the game, places their cards into the deck,
     * shuffles, and deletes their player object.
     *
     * Note: If the removed player is currently playing, their turn will be
     * forcibly ended. Additionally, if there are less than 2 players left
     * after this operation, reset() will automatically be called.
     */
    void removePlayer(Player *player);

    /**
     * @brief getTopCard Get the top card from the discard pile
     * @return The top card
     */
    const Card *getTopCard() const;

    /**
     * @brief drawCard Draw a single card for the current player
     * @param result The result of the action
     * @return On success, the card that was drawn. On failure, nullptr
     *
     * Note: This method may fail if there are no more cards left in the
     * deck / discard pile. In this case, the method will return nullptr
     * and the current player's turn will be skipped.
     *
     * Possible results from this method are Success, RanOutOfCards, and
     * GameStateInvalid
     */
    const Card *drawCard(Result *result);

    /**
     * @brief playCard Play a card (non-wildcards ONLY)
     * @param card The card to play
     * @return The result of the action
     *
     * Plays a card for the current player. The player must own the card
     * and the card must NOT be a wildcard (wildcards must be played with the
     * overloaded method below, which also requires a newColor).
     *
     * Possible results from this function are Success, InvalidAction, and
     * GameStateInvalid
     */
    Result playCard(const Card *card);

    /**
     * @brief playCard Play a card (non-wildcards ONLY)
     * @param card The card to play
     * @param newColor The new color
     * @return The result of the action
     *
     * Plays a card for the current player. The player must own the card
     * and the card must be a wildcard (non-wildcards must be played with the
     * overloaded method above, which does NOT require a newColor).
     *
     * Possible results from this function are Success, InvalidAction, and
     * GameStateInvalid
     */
    Result playCard(const Card *card, CardColor newColor);

    /**
     * @brief callUno Calls Uno for a player when they have only one card left
     *
     * Calls Uno for a player when they have only one card left. If they have
     * more than one card, the game will force them to draw two cards.
     */
    void callUno();

    /**
     * @brief callOutPlayer Call out another player for not saying Uno
     * @param target The player to call out
     * @return True if the other player was called out, false if they had
     * already called Uno or they had more than one card left
     *
     * Calls out another player who forgot to call Uno during their turn.
     * Forces the called out player to draw two cards.
     */
    bool callOutPlayer(Player *target);

    /**
     * @brief getCurrentPlayer Get the current player
     * @return The current player
     */
    Player *getCurrentPlayer();

    /**
     * @brief endTurn End the current player's turn
     * @return True if the turn was ended, false otherwise
     *
     * Note: The player's turn can only be ended after they have played
     * a card.
     */
    bool endTurn();

    /**
     * @brief isTurnDirectionReversed Checks if the turn direction is reversed
     * @return True if the turn direction is reversed, false otherwise
     */
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
