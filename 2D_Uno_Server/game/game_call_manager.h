#ifndef GAME_CALL_MANAGER_H
#define GAME_CALL_MANAGER_H

#include <unordered_set>

using std::unordered_set;

namespace game
{

class Player;

/**
 * @brief The GameCallManager class
 *
 * Tracks players who have called Uno.
 */
class GameCallManager
{
public:
    GameCallManager();

    /**
     * @brief callUno Call Uno for a player
     * @param player The player
     */
    void callUno(Player *player);

    /**
     * @brief resetPlayerCall Reset the player's call status to false
     * @param player The player
     */
    void resetPlayerCall(Player *player);

    /**
     * @brief hasPlayerCalledUno Check if a player has called Uno
     * @param player The player
     * @return True if they have, false otherwise
     */
    bool hasPlayerCalledUno(Player *player);

private:
    unordered_set<Player*> playersWhoCalled_;
};

}

#endif // GAME_CALL_MANAGER_H
