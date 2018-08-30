#ifndef GAME_CALL_MANAGER_H
#define GAME_CALL_MANAGER_H

#include <unordered_set>

using std::unordered_set;

namespace game
{

class Player;

class GameCallManager
{
public:
    GameCallManager();

    void callUno(Player *player);
    void resetPlayerCall(Player *player);

    bool hasPlayerCalledUno(Player *player);

private:
    unordered_set<Player*> playersWhoCalled_;
};

}

#endif // GAME_CALL_MANAGER_H
