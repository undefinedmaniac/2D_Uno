#ifndef OBSERVER_H
#define OBSERVER_H

#include <iostream>
#include "game/enums/card_color.h"
#include "game/enums/card_type.h"
#include "game/player.h"
#include "game/card.h"
#include "game/interfaces/igameobserver.h"

std::string convertColor(game::CardColor color) {
    using game::CardColor;

    switch (color) {
    case CardColor::Blue:
        return "Blue";
    case CardColor::Red:
        return "Red";
    case CardColor::Yellow:
        return "Yellow";
    case CardColor::Green:
        return "Green";
    case CardColor::Wildcard:
        return "Wild";
    }

    return "";
}

std::string convertType(game::CardType type) {
    using game::CardType;

    switch (type) {
    case CardType::Zero:
        return "Zero";
    case CardType::One:
        return "One";
    case CardType::Two:
        return "Two";
    case CardType::Three:
        return "Three";
    case CardType::Four:
        return "Four";
    case CardType::Five:
        return "Five";
    case CardType::Six:
        return "Six";
    case CardType::Seven:
        return "Seven";
    case CardType::Eight:
        return "Eight";
    case CardType::Nine:
        return "Nine";
    case CardType::Skip:
        return "Skip";
    case CardType::Reverse:
        return "Reverse";
    case CardType::Wildcard:
        return "Wild";
    case CardType::DrawTwo:
        return "Draw two";
    case CardType::DrawFourWildcard:
        return "Draw four wild";
    }

    return "";
}

void print(const std::string &message) {
    std::cout << message << std::endl;
}

class Observer : public game::IGameObserver
{
public:
    void gameStarted(const game::Card *firstCard, game::CardColor startingColor) override {
        std::cout << "Game started - First Card - Color: " << convertColor(firstCard->getColor()) << " Type: " << convertType(firstCard->getType()) <<
                     " Starting Color: " << convertColor(startingColor) << " ID: " << firstCard->getId() << std::endl;
    }
    void gameReset() override {
        print("Game Reset!");
    }
    void gameEnded(game::Player *winner) override {
        std::cout << "Game ended - Winner: " << winner->getName() << std::endl;
    }
    void playerCardCountChanged(game::Player *player, int newCount) override {
        std::cout << "Player card count changed: " << player->getName() << " NewCount: " << newCount << std::endl;
    }
    void deckShuffled() override {
        print("Deck shuffled!");
    }
    void deckCountChanged(int newCount) override {
        std::cout << "Deck count changed: " << newCount << std::endl;
    }
    void discardPileCountChanged(int newCount) override {
        std::cout << "Discard count changed: " << newCount << std::endl;
    }
    void cardPlayed(game::Player *player, const game::Card *card) override {
        std::cout << "Card Played - Player: " << player->getName() << " Color: " <<
                     convertColor(card->getColor()) << " Type: " << convertType(card->getType()) << " ID: " << card->getId() << std::endl;
    }
    void cardPlayed(game::Player *player, const game::Card *card, game::CardColor newColor) override {
        std::cout << "Card Played - Player: " << player->getName() << " Color: " <<
                     convertColor(card->getColor()) << " Type: " << convertType(card->getType()) <<
                     " NewColor: " << convertColor(newColor) << " ID: " << card->getId() << std::endl;
    }
    void cardDrawn(game::Player *player, const game::Card *card) override {
        std::cout << "Card Drawn - Player: " << player->getName() << " Color: " <<
                     convertColor(card->getColor()) << " Type: " << convertType(card->getType()) << " ID: " << card->getId() << std::endl;
    }
    void playerTurnStarted(game::Player *player) override {
        std::cout << "Turn started: " << player->getName() << std::endl;
    }
    void playerTurnEnded(game::Player *player) override {
        std::cout << "Turn ended: " << player->getName() << std::endl;
    }
    void turnSkipped(game::Player *player) override {
        std::cout << "Turn skipped: " << player->getName() << std::endl;
    }
    void turnDirectionChanged(bool isReversed) override {
        std::cout << "Is turn directon reversed: " << isReversed << std::endl;
    }
    void playerCalledUno(game::Player* player) override {
        std::cout << "Player called uno: " << player->getName() << std::endl;
    }
    void playerForgotToCallUno(game::Player* caller, game::Player* target) override {
        std::cout << "Player forogt to call uno - Caller: " <<
                     caller->getName() << " Target: " << target->getName() << std::endl;
    }
};

#endif // OBSERVER_H
