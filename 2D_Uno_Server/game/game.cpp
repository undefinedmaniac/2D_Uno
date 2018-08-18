#include "game.h"

namespace game
{

Game::Game(IGameObserver* observer) : observer_(observer)
{
}

Game::~Game()
{
    for (unordered_map<Player*, PrivatePlayer*>::iterator i =
         playerMap_.begin(); i != playerMap_.end(); i++) {
        delete i->first;
        delete i->second;
    }
}

void Game::setObserver(IGameObserver& observer)
{
    observer_ = &observer;
}

void Game::startGame()
{

}

void Game::resetGame()
{

}

Player* Game::addPlayer(const string& name)
{
    PrivatePlayer* newPrivatePlayer = new PrivatePlayer(name);
    Player* newPlayer = new Player(newPrivatePlayer);

    std::pair<Player*, PrivatePlayer*> newPair(newPlayer, newPrivatePlayer);

    playerMap_.insert(newPair);
    turnList_.push_back(newPlayer);

    if (isGameRunning) {

    }
    else {

    }
}

void Game::removePlayer(Player* player)
{

}

const Card *Game::getTopCard() const
{
    return discardPile_.getTopCard();
}

bool Game::drawCard(Player* player)
{
    if (player == currentPlayer_) {
        bool success = drawCardHelper(player, 1);

        if (success)
            return true;

        advancePlayerTurn();
        return false;
    }
}

bool Game::playCard(Player* player, const Card* card)
{
    if (player == currentPlayer_ &&
        player->getCards().find(card) != player->getCards().end()) {

        const Card* topCard = getTopCard();

        CardColor color;

        if (topCard->getColor() == CardColor::Wildcard)
            color = wildcardColor_;
        else
            color = topCard->getColor();

        if (color == card->getColor() || topCard->getType() == card->getType()) {
            playerMap_[player]->removeCard(card);
            discardPile_.placeCard(card);

            if (observer_)
                observer_->cardPlayed(player, card);

            if (card->getType() == CardType::Skip)
                skipPlayerTurn();
            else if (card->getType() == CardType::Reverse)
                changeTurnDirection();
            else if (card->getType() == CardType::DrawTwo)
                giveNextPlayerCards(2);

            advancePlayerTurn();

            return true;
        }

        return false;
    }
}

bool Game::playCard(Player* player, const Card* card, CardColor newColor)
{
    if (player == currentPlayer_ &&
        player->getCards().find(card) != player->getCards().end() &&
        card->getColor() == CardColor::Wildcard) {
        wildcardColor_ = newColor;
        playerMap_[player]->removeCard(card);
        discardPile_.placeCard(card);

        if (observer_)
            observer_->cardPlayed(player, card);

        if (card->getType() == CardType::DrawFourWildcard)
            giveNextPlayerCards(4);

        advancePlayerTurn();

        return true;
    }

    return false;
}

Player* Game::getCurrentPlayer()
{
    return currentPlayer_;
}

bool Game::isTurnDirectionReversed()
{
    return isTurnDirectionReversed_;
}

void Game::shuffleDeck()
{
    deck_.shuffle();

    if (observer_)
        observer_->deckShuffled();
}

bool Game::drawCardHelper(Player *player, int nCards)
{
    for (int i = 0; i < nCards; i++) {
        if (deck_.count() == 0) {
            if (discardPile_.count() >= 2) {
                vector<const Card*> cards = discardPile_.takeLowerCards();

                for (const Card* card : cards)
                    deck_.placeCard(card);

                shuffleDeck();
            }
            else {
                //Ran out of cards
                return false;
            }
        }

        const Card* card = deck_.takeCard();
        playerMap_[player]->addCard(card);
    }

    return true;
}

bool Game::giveNextPlayerCards(int nCards)
{
    Player* target;

    if (isTurnDirectionReversed_)
        target = turnList_.back();
    else
        target = turnList_.front();

    drawCardHelper(target, nCards);
}

void Game::advancePlayerTurn()
{
    if (observer_ && currentPlayer_ != nullptr)
        observer_->playerTurnEnded(currentPlayer_);

    advancePlayerTurnHelper();

    if (observer_)
        observer_->playerTurnStarted(currentPlayer_);
}

void Game::advancePlayerTurnHelper()
{
    if (isTurnDirectionReversed_) {
        currentPlayer_ = turnList_.back();
        turnList_.pop_back();
        turnList_.push_front(currentPlayer_);
    } else {
        currentPlayer_ = turnList_.front();
        turnList_.pop_front();
        turnList_.push_back(currentPlayer_);
    }
}

void Game::skipPlayerTurn()
{
    Player* skippedPlayer = currentPlayer_;

    advancePlayerTurnHelper();

    if (observer_)
        observer_->turnSkipped(skippedPlayer);

    advancePlayerTurn();
}

void Game::changeTurnDirection()
{
    isTurnDirectionReversed_ = !isTurnDirectionReversed_;

    if (observer_)
        observer_->turnDirectionChanged(isTurnDirectionReversed_);
}

}
