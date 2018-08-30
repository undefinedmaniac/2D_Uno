#include "game_call_manager.h"

namespace game
{

GameCallManager::GameCallManager()
{
}

void GameCallManager::callUno(Player *player)
{
    playersWhoCalled_.insert(player);
}

void GameCallManager::resetPlayerCall(Player *player)
{
    unordered_set<Player*>::iterator i = playersWhoCalled_.find(player);

    if (i != playersWhoCalled_.end())
        playersWhoCalled_.erase(i);
}

bool GameCallManager::hasPlayerCalledUno(Player *player)
{
    return playersWhoCalled_.find(player) != playersWhoCalled_.end();
}

}
