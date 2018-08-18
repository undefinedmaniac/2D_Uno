#include "game.h"

namespace game
{

Game::Game(IGameObserver* observer) : observer_(observer)
{
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
    unique_ptr<PrivatePlayer> newPrivatePlayer(new PrivatePlayer(name));
    unique_ptr<Player> newPlayer(new Player(newPrivatePlayer.get()));

    privatePlayers_.insert(std::move(newPrivatePlayer));
    players_.insert(std::move(newPlayer));

    playerMap_[newPlayer.get()] = newPrivatePlayer.get();

    turnList_.push_back(newPlayer.get());

    if (isGameRunning) {

    }
    else {

    }
}

void Game::removePlayer(Player* player)
{
    //Put the cards back in the deck and shuffle
    unordered_set<const Card*> cards = player->getCards();
    for (const Card* card : cards)
        deck_.placeCard(card);
    shuffleDeck();

    //End their turn if they are currently playing
    if (currentPlayer_ == player) {
        advancePlayerTurn();
    }

    //Remove them from the turn list
    deque<Player*>::iterator turnListPosition =
            std::find(turnList_.begin(), turnList_.end(), player);

    if (turnListPosition != turnList_.end())
        turnList_.erase(turnListPosition);

    //Remove them from the player map
    PrivatePlayer* privatePlayer = playerMap_[player];

    unordered_map<Player*, PrivatePlayer*>::iterator playerMapPosition =
            playerMap_.find(player);

    if (playerMapPosition != playerMap_.end())
        playerMap_.erase(playerMapPosition);

    //Delete them early
    for (unordered_set<unique_ptr<Player>>::iterator i = players_.begin();
         i != players_.end(); i++)
    {
        if (i->get() == player) {
            players_.erase(i);
            break;
        }
    }

    for (unordered_set<unique_ptr<PrivatePlayer>>::iterator i = privatePlayers_.begin();
         i != privatePlayers_.end(); i++)
    {
        if (i->get() == privatePlayer) {
            privatePlayers_.erase(i);
            break;
        }
    }
}

const Card *Game::getTopCard() const
{
    return discardPile_.getTopCard();
}

bool Game::drawCard()
{
    if (currentPlayer_) {
        int numberOfCardsDrawn = drawCardHelper(currentPlayer_, 1);

        bool successful = numberOfCardsDrawn == 1;

        //End the player's turn if they are unable to draw cards
        if (!successful) {
            endPlayerTurn();
            startNextPlayerTurn();
        }

        return successful;
    }
}

bool Game::playCard(const Card* card)
{
    if (currentPlayer_) {
        bool playerHasCard = currentPlayer_->getCards().find(card) !=
                             currentPlayer_->getCards().end();

        bool cardIsNotWildcard = card->getColor() != CardColor::Wildcard;

        if (playerHasCard && cardIsNotWildcard && !hasPlayerPlayed_) {
            const Card* topCard = getTopCard();

            CardColor color;

            if (topCard->getColor() == CardColor::Wildcard)
                color = wildcardColor_;
            else
                color = topCard->getColor();

            if (color == card->getColor() ||
                topCard->getType() == card->getType())
            {
                playerMap_[currentPlayer_]->removeCard(card);
                discardPile_.placeCard(card);

                hasPlayerPlayed_ = true;

                if (observer_)
                    observer_->cardPlayed(currentPlayer_, card);

                switch (card->getType()) {
                    case CardType::Skip :
                        skipNextPlayerTurn_ = true;
                        break;

                    case CardType::Reverse :
                        changeTurnDirection();
                        break;

                    case CardType::DrawTwo : {
                        skipNextPlayerTurn_ = true;
                        giveNextPlayerCards(2);
                        break;
                    }

                    default :
                        break;
                }

                return true;
            }
    }
        return false;
    }
}

bool Game::playCard(const Card* card, CardColor newColor)
{
    if (currentPlayer_) {
        bool playerHasCard = currentPlayer_->getCards().find(card) !=
                             currentPlayer_->getCards().end();

        bool cardIsWildcard = card->getColor() == CardColor::Wildcard;

        bool colorIsValid = newColor == CardColor::Red ||
                            newColor == CardColor::Yellow ||
                            newColor == CardColor::Green ||
                            newColor == CardColor::Blue;

        if (playerHasCard && cardIsWildcard &&
            colorIsValid && !hasPlayerPlayed_) {
            wildcardColor_ = newColor;
            playerMap_[currentPlayer_]->removeCard(card);
            discardPile_.placeCard(card);

            hasPlayerPlayed_ = true;

            if (observer_)
                observer_->cardPlayed(currentPlayer_, card);

            if (card->getType() == CardType::DrawFourWildcard) {
                skipNextPlayerTurn_ = true;
                giveNextPlayerCards(4);
            }

            return true;
        }
    }

    return false;
}

Player* Game::getCurrentPlayer()
{
    return currentPlayer_;
}

bool Game::endTurn()
{
    if (hasPlayerPlayed_) {
        advancePlayerTurn();
        return true;
    }

    return false;
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

int Game::drawCardHelper(Player* player, int nCards)
{
    int numberOfCardsDrawn = 0;

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
                break;
            }
        }

        const Card* card = deck_.takeCard();
        playerMap_[player]->addCard(card);
        numberOfCardsDrawn++;

        if (observer_)
            observer_->cardDrawn(player, card);
    }

    return numberOfCardsDrawn;
}

int Game::giveNextPlayerCards(int nCards)
{
    Player* target;

    if (isTurnDirectionReversed_)
        target = turnList_.back();
    else
        target = turnList_.front();

    return drawCardHelper(target, nCards);
}

void Game::advancePlayerTurn()
{
    endPlayerTurn();

    if (skipNextPlayerTurn_) {
        skipPlayerTurn();
        skipNextPlayerTurn_ = false;
    }

    startNextPlayerTurn();
}

void Game::startNextPlayerTurn()
{
    if (turnList_.size() >= 2) {
        startNextPlayerTurnHelper();

        hasPlayerPlayed_ = false;

        if (observer_)
            observer_->playerTurnStarted(currentPlayer_);
    }
    else {
        //TO-DO Not enough players in the game!
    }
}

void Game::startNextPlayerTurnHelper()
{
    if (isTurnDirectionReversed_) {
        currentPlayer_ = turnList_.back();
        turnList_.pop_back();
    }
    else {
        currentPlayer_ = turnList_.front();
        turnList_.pop_front();
    }
}

void Game::endPlayerTurn()
{
    if (currentPlayer_) {
        endPlayerTurnHelper();

        if (observer_)
            observer_->playerTurnEnded(currentPlayer_);
    }
}

void Game::endPlayerTurnHelper()
{
    if (isTurnDirectionReversed_)
        turnList_.push_front(currentPlayer_);
    else
        turnList_.push_back(currentPlayer_);
}

void Game::skipPlayerTurn()
{
    Player* skippedPlayer;

    startNextPlayerTurnHelper();
    skippedPlayer = currentPlayer_;
    endPlayerTurnHelper();

    if (observer_)
        observer_->turnSkipped(skippedPlayer);
}

void Game::changeTurnDirection()
{
    isTurnDirectionReversed_ = !isTurnDirectionReversed_;

    if (observer_)
        observer_->turnDirectionChanged(isTurnDirectionReversed_);
}

}
