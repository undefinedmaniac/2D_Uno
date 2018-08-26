#include "game.h"

namespace game
{

Game::Game(unsigned int seed, IGameObserver* observer) :
    observer_(observer), randomEngine_(seed), deck_(randomEngine_)
{
}

void Game::setObserver(IGameObserver& observer)
{
    observer_ = &observer;
}

bool Game::startGame()
{
    if (players_.size() >= 2 && !isGameRunning_) {

        buildDeck();
        shuffleDeck();

        for (const unique_ptr<Player>& player : players_)
            drawCardHelper(player.get(), 7);

        const Card* firstCard = deck_.takeCard();
        CardColor firstColor;

        if (firstCard->getColor() == CardColor::Wildcard) {
            vector<CardColor> colors = {CardColor::Red, CardColor::Yellow,
                                        CardColor::Green, CardColor::Blue};
            std::uniform_int_distribution<> colorSelector(0, 3);

            unsigned int selection = static_cast<unsigned int>
                    (colorSelector(randomEngine_));

            wildcardColor_ = colors[selection];
            firstColor = wildcardColor_;
        } else {
            firstColor = firstCard->getColor();
        }

        discardPile_.placeCard(firstCard);

        bool skipFirstPlayer = false;
        int numberOfCardsToDraw = 0;

        switch (firstCard->getType()) {
            case CardType::Reverse :
                toggleTurnDirection();
                break;

            case CardType::Skip :
                skipFirstPlayer = true;
                break;

            case CardType::DrawTwo :
                skipFirstPlayer = true;
                numberOfCardsToDraw = 2;
                break;

            case CardType::DrawFourWildcard :
                skipFirstPlayer = true;
                numberOfCardsToDraw = 4;
                break;

            default:
                break;
        }

        int maximumNumber = static_cast<int>(players_.size() - 1);
        std::uniform_int_distribution<> playerSelector(0, maximumNumber);

        unsigned int selection = static_cast<unsigned int>
                (playerSelector(randomEngine_));

        Player* firstPlayer = players_[selection].get();

        drawCardHelper(firstPlayer, numberOfCardsToDraw);        

        turnManager_.startFirstTurn(firstPlayer, skipFirstPlayer);

        isGameRunning_ = true;

        if (observer_) {
            observer_->gameStarted(firstCard, firstColor);
            observer_->playerTurnStarted(firstPlayer);
        }

        return true;
    }

    return false;
}

void Game::resetGame()
{
    for (const unique_ptr<PrivatePlayer>& player : privatePlayers_) {
        for (std::pair<unsigned int, const Card*> pair : player->getCards()) {
            const Card *card = pair.second;
            player->removeCard(card);
            deck_.placeCard(card);
        }
    }

    for (const Card* card : discardPile_.takeAllCards())
        deck_.placeCard(card);

    shuffleDeck();

    turnManager_.resetTurns();

    hasPlayerPlayed_ = false;
    skipNextPlayerTurn_ = false;
    isGameRunning_ = false;

    if (observer_)
        observer_->gameReset();
}

Player* Game::addPlayer(const string& name)
{
    unique_ptr<PrivatePlayer> newPrivatePlayer(new PrivatePlayer(name));
    unique_ptr<Player> newPlayer(new Player(newPrivatePlayer.get()));

    PrivatePlayer *privatePlayerPtr = newPrivatePlayer.get();
    Player *playerPtr = newPlayer.get();

    privatePlayers_.push_back(std::move(newPrivatePlayer));
    players_.push_back(std::move(newPlayer));

    playerMap_[playerPtr] = privatePlayerPtr;

    turnManager_.addPlayer(playerPtr);

    return playerPtr;
}

void Game::removePlayer(Player* player)
{
    //Put the cards back in the deck and shuffle
    for (std::pair<unsigned int, const Card*> pair : player->getCards())
        deck_.placeCard(pair.second);
    shuffleDeck();

    //End their turn if they are currently playing
    if (turnManager_.getCurrentPlayer() == player)
        startNextTurn();

    //Remove them from the turn list
    turnManager_.removePlayer(player);

    //Remove them from the player map
    PrivatePlayer* privatePlayer = playerMap_[player];

    unordered_map<Player*, PrivatePlayer*>::iterator playerMapPosition =
            playerMap_.find(player);

    if (playerMapPosition != playerMap_.end())
        playerMap_.erase(playerMapPosition);

    //Delete them early
    for (vector<unique_ptr<Player>>::iterator i = players_.begin();
         i != players_.end(); i++)
    {
        if (i->get() == player) {
            players_.erase(i);
            break;
        }
    }

    for (vector<unique_ptr<PrivatePlayer>>::iterator i = privatePlayers_.begin();
         i != privatePlayers_.end(); i++)
    {
        if (i->get() == privatePlayer) {
            privatePlayers_.erase(i);
            break;
        }
    }

    if (players_.size() < 2) {
        resetGame();
    }
}

const Card *Game::getTopCard() const
{
    return discardPile_.getTopCard();
}

bool Game::drawCard()
{
    if (isGameRunning_) {
        int numberOfCardsDrawn = drawCardHelper(turnManager_.getCurrentPlayer(), 1);
        bool successful = numberOfCardsDrawn == 1;

        //End the player's turn if they are unable to draw cards
        if (!successful)
            startNextTurn();

        return successful;
    }

    return false;
}

bool Game::playCard(const Card* card)
{
    if (isGameRunning_) {
        Player *currentPlayer = turnManager_.getCurrentPlayer();

        bool playerHasCard = currentPlayer->getCards().find(card->getId()) !=
                             currentPlayer->getCards().end();

        bool cardIsNotWildcard = card->getColor() != CardColor::Wildcard;

        if (playerHasCard && cardIsNotWildcard && !hasPlayerPlayed_) {
            const Card* topCard = getTopCard();

            if (!topCard)
                return false;

            //Get the topcard's color
            CardColor color;

            if (topCard->getColor() == CardColor::Wildcard)
                color = wildcardColor_;
            else
                color = topCard->getColor();

            if (color == card->getColor() ||
                topCard->getType() == card->getType())
            {
                playCardHelper(card);

                switch (card->getType()) {
                    case CardType::Skip :
                        skipNextPlayerTurn_ = true;
                        break;

                    case CardType::Reverse :
                        toggleTurnDirection();
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
    }
    return false;
}

bool Game::playCard(const Card* card, CardColor newColor)
{
    if (isGameRunning_) {
        Player* currentPlayer = turnManager_.getCurrentPlayer();

        bool playerHasCard = currentPlayer->getCards().find(card->getId()) !=
                             currentPlayer->getCards().end();

        bool cardIsWildcard = card->getColor() == CardColor::Wildcard;

        bool colorIsValid = newColor == CardColor::Red ||
                            newColor == CardColor::Yellow ||
                            newColor == CardColor::Green ||
                            newColor == CardColor::Blue;

        if (playerHasCard && cardIsWildcard &&
            colorIsValid && !hasPlayerPlayed_) {
            wildcardColor_ = newColor;

            playCardHelper(card);

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
    return turnManager_.getCurrentPlayer();
}

bool Game::endTurn()
{
    if (isGameRunning_ && hasPlayerPlayed_) {
        startNextTurn();
        return true;
    }

    return false;
}

bool Game::isTurnDirectionReversed() const
{
    return turnManager_.isTurnDirectionReversed();
}

void Game::buildDeck()
{
    vector<CardColor> colors = {CardColor::Red, CardColor::Yellow,
                                CardColor::Green, CardColor::Blue};

    vector<CardType> types = {CardType::One, CardType::Two,
                              CardType::Three, CardType::Four, CardType::Five,
                              CardType::Six, CardType::Seven, CardType::Eight,
                              CardType::Nine, CardType::Skip,
                              CardType::Reverse, CardType::DrawTwo};

    for (CardColor color : colors) {
        cards_.push_back(unique_ptr<Card>(new Card(color, CardType::Zero)));

        for (int i = 0; i < 2; i++) {
            for (CardType type : types) {
                cards_.push_back(unique_ptr<Card>(new Card(color, type)));
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        unique_ptr<Card> wildcard(new Card(CardColor::Wildcard,
                                           CardType::Wildcard));

        unique_ptr<Card> drawWildcard(new Card(CardColor::Wildcard,
                                               CardType::DrawFourWildcard));

        cards_.push_back(std::move(wildcard));
        cards_.push_back(std::move(drawWildcard));
    }

    for (const unique_ptr<Card>& card : cards_) {
        deck_.placeCard(card.get());
    }
}

void Game::shuffleDeck()
{
    deck_.shuffle();

    if (observer_)
        observer_->deckShuffled();
}

void Game::playCardHelper(const Card *card)
{
    Player* currentPlayer = turnManager_.getCurrentPlayer();

    playerMap_[currentPlayer]->removeCard(card);
    discardPile_.placeCard(card);

    hasPlayerPlayed_ = true;

    if (observer_)
        observer_->cardPlayed(currentPlayer, card);
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
    Player* target = turnManager_.getNextPlayer();

    if (!target)
        return 0;

    return drawCardHelper(target, nCards);
}

void Game::startNextTurn()
{
    Player* oldPlayer = turnManager_.getCurrentPlayer();
    Player* newPlayer;
    Player* skippedPlayer;
    newPlayer = turnManager_.startNextTurn(skipNextPlayerTurn_,
                                           &skippedPlayer);

    hasPlayerPlayed_ = false;
    skipNextPlayerTurn_ = false;

    if (observer_) {
        observer_->playerTurnEnded(oldPlayer);

        if (skippedPlayer)
            observer_->turnSkipped(skippedPlayer);

        observer_->playerTurnStarted(newPlayer);
    }
}

void Game::toggleTurnDirection()
{
    turnManager_.toggleTurnDirection();

    if (observer_)
        observer_->turnDirectionChanged(
                    turnManager_.isTurnDirectionReversed());
}

void Game::endGame(Player *winner)
{
    if (observer_)
        observer_->gameEnded(winner);

    resetGame();
}

}
