#ifndef PLAYER_H
#define PLAYER_H

#include "privateplayer.h"

namespace game
{

/**
 * @brief The Player class
 *
 * Represents a player in the game. Players have a username and a collection of
 * cards. Internally, the Player class referenes a PrivatePlayer object which
 * contains the actual data.
 */
class Player
{
public:
    Player(PrivatePlayer* player);

    /**
     * @brief getName Get the username of the player
     * @return The username
     */
    string getName() const;

    /**
     * @brief getCards Get the player's cards
     * @return The cards
     */
    const PrivatePlayer::CardMap &getCards() const;

    /**
     * @brief cardCount Get the number of cards in the player's hand
     * @return The number of cards
     */
    unsigned int cardCount() const;

private:
    PrivatePlayer* player_;
};

}

#endif // PLAYER_H
