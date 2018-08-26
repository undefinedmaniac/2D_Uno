#include <iostream>
#include "game/game.h"
#include "game/interfaces/igameobserver.h"
#include "observer.h"

int main()
{
    Observer observer;

    game::Game game(0, &observer);

    game::Player* player1 = game.addPlayer("Player 1");
    game::Player* player2 = game.addPlayer("Player 2");
    game::Player* player3 = game.addPlayer("Player 3");

    game.startGame();

    std::cout << game.playCard(player2->getCards().at(83)) << std::endl;

    std::cout << game.endTurn() << std::endl;

    std::cout << game.playCard(player3->getCards().at(85)) << std::endl;

    std::cout << game.endTurn() << std::endl;

    std::cout << game.playCard(player2->getCards().at(89)) << std::endl;

    std::cout << game.endTurn() << std::endl;

    game.drawCard();
    game.drawCard();
    game.drawCard();

    std::cout << game.playCard(player3->getCards().at(80)) << std::endl;

    std::cout << game.endTurn() << std::endl;

    std::cout << game.playCard(player1->getCards().at(106), game::CardColor::Yellow) << std::endl;

    std::cout << game.endTurn() << std::endl;

    std::cout << game.playCard(player2->getCards().at(34)) << std::endl;

    std::cout << game.endTurn() << std::endl;

    std::cout << game.playCard(player3->getCards().at(49)) << std::endl;

    std::cout << game.endTurn() << std::endl;

    std::cout << game.playCard(player2->getCards().at(30)) << std::endl;

    std::cout << game.endTurn() << std::endl;

    game.drawCard();

    return 0;
}
