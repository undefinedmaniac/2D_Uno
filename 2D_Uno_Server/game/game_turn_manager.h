#ifndef GAME_TURN_MANAGER_H
#define GAME_TURN_MANAGER_H

#include <deque>
#include <algorithm>
#include <iostream>

using std::deque;

namespace game
{

class Player;

/**
 * @brief The GameTurnManager class
 *
 * Manages turns for the Game class. The turn system is based on a queue
 * that moves either forward or backward (based on the value of
 * isTurnDirectionReversed()). Add players to the turn list with addPlayer()
 * and removePlayer(). Start the first player's turn with startFirstTurn() and
 * advance turns with startNextTurn().
 */
class GameTurnManager
{
public:
    /**
     * @brief addPlayer Adds a player to the turn list
     * @param player The player to add
     */
    void addPlayer(Player* player);

    /**
     * @brief removePlayer Removes a player from the turn list
     * @param player The player to remove
     * @return True if the removal was successful, false otherwise
     *
     * Note: The current player cannot be removed until their turn has ended
     */
    bool removePlayer(Player* player);

    /**
     * @brief resetTurns Resets the object so that startFirstTurn() may be
     * called again. This function does not discard stored players, therefore
     * addPlayer() and removePlayer() do not need to be called again.
     */
    void resetTurns();

    /**
     * @brief startFirstTurn Starts the first player's turn
     * @param firstPlayer The first player
     * @param skipFirstPlayer True if the first player's turn should
     * be immediately skipped
     * @return True if the operation was successful, false if there
     * were not enough players (at least 2).
     */
    bool startFirstTurn(Player* firstPlayer, bool skipFirstPlayer);

    /**
     * @brief startNextTurn Starts the next player's turn
     * @param skipPlayer If true, skips the next player's turn and starts
     * the following player's turn. If false, starts the next player's turn
     * @param skippedPlayer If a pointer is provided, it will be assigned to
     * point at the player who was skipped. Otherwise, the pointer will be
     * assigned to nullptr.
     * @return Returns the value of getCurrentPlayer() after the operation
     * completes.
     */
    Player* startNextTurn(bool skipPlayer, Player** skippedPlayer = nullptr);

    /**
     * @brief toggleTurnDirection Switches the turn direction
     * @return Returns the value of isTurnDirectionRevered()
     */
    bool toggleTurnDirection();

    /**
     * @brief getCurrentPlayer Gets the current player
     * @return The current player or nullptr if there is no current player
     */
    Player* getCurrentPlayer();

    /**
     * @brief getNextPlayer Gets the next player
     * @return The next player or nullptr if there is no next player
     */
    Player* getNextPlayer();

    /**
     * @brief isTurnDirectionReversed Gets the direction of the turns
     * @return True if the direction is reversed, otherwise false
     */
    bool isTurnDirectionReversed() const;

private:
    Player* takeNextPlayer();
    void putLastPlayer(Player* player);
    void advanceQueue();

    Player* currentPlayer_ = nullptr;
    deque<Player*> turnList_;
    bool isTurnDirectionRevered_ = false;
};

}

#endif // GAME_TURN_MANAGER_H
