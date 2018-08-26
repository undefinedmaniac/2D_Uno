#ifndef PLAYER_H
#define PLAYER_H

#include "privateplayer.h"

namespace game
{

class Player
{
public:
    Player(PrivatePlayer* player);

    string getName() const;
    const PrivatePlayer::CardMap &getCards() const;

private:
    PrivatePlayer* player_;
};

}

#endif // PLAYER_H
