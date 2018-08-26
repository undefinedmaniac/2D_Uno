#include "game_turn_manager.h"

namespace game
{

void GameTurnManager::addPlayer(Player* player)
{
    if (isTurnDirectionRevered_)
        turnList_.push_front(player);
    else
        turnList_.push_back(player);
}

bool GameTurnManager::removePlayer(Player* player)
{
    if (player != currentPlayer_) {
        deque<Player*>::iterator position =
                std::find(turnList_.begin(), turnList_.end(), player);

        if (position != turnList_.end()) {
            turnList_.erase(position);
            return true;
        }
    }

    return false;
}

void GameTurnManager::resetTurns()
{
    currentPlayer_ = nullptr;
    isTurnDirectionRevered_ = false;
}

bool GameTurnManager::startFirstTurn(Player* firstPlayer, bool skipFirstPlayer)
{
    if (turnList_.size() >= 2) {
        while (getNextPlayer() != firstPlayer)
            advanceQueue();

        startNextTurn(skipFirstPlayer);

        return true;
    }

    return false;
}

Player *GameTurnManager::startNextTurn(bool skipPlayer, Player** skippedPlayer)
{
    if (currentPlayer_)
        putLastPlayer(currentPlayer_);

    if (skipPlayer) {
        if (skippedPlayer)
            (*skippedPlayer) = getNextPlayer();

        advanceQueue();
    }
    else {
        if (skippedPlayer)
            (*skippedPlayer) = nullptr;
    }

    currentPlayer_ = takeNextPlayer();

    return getCurrentPlayer();
}

bool GameTurnManager::toggleTurnDirection()
{
    isTurnDirectionRevered_ = !isTurnDirectionRevered_;

    return isTurnDirectionReversed();
}

Player *GameTurnManager::getCurrentPlayer()
{
    return currentPlayer_;
}

bool GameTurnManager::isTurnDirectionReversed() const
{
    return isTurnDirectionRevered_;
}

Player* GameTurnManager::takeNextPlayer()
{
    Player* nextPlayer;

    if (turnList_.size() >= 2 ||
            (turnList_.size() == 1 && currentPlayer_)) {
        if (isTurnDirectionRevered_) {
            nextPlayer = turnList_.back();
            turnList_.pop_back();
        }
        else {
            nextPlayer = turnList_.front();
            turnList_.pop_front();
        }
    }
    else {
        nextPlayer = nullptr;
    }

    return nextPlayer;
}

void GameTurnManager::putLastPlayer(Player *player)
{
    if (isTurnDirectionRevered_)
        turnList_.push_front(player);
    else
        turnList_.push_back(player);
}

Player* GameTurnManager::getNextPlayer()
{
    if (turnList_.size() >= 2 ||
            (turnList_.size() == 1 && currentPlayer_)) {
        if (isTurnDirectionRevered_)
            return turnList_.back();
        else
            return turnList_.front();
    }

    return nullptr;
}

void GameTurnManager::advanceQueue()
{
    Player* player = takeNextPlayer();

    if (player)
        putLastPlayer(player);
}

}
