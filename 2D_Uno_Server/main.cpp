#include <iostream>
#include "game/game.h"
#include "game/interfaces/igameobserver.h"
#include "observer.h"
#include "network/liststreamsocket.h"

int main()
{
    /*Observer observer;

    game::Game game(0, &observer);

    game::Player* player1 = game.addPlayer("Player 1");
    game::Player* player2 = game.addPlayer("Player 2");
    game::Player* player3 = game.addPlayer("Player 3");

    game.start();

    game.removePlayer(player2);
    game.removePlayer(player3);*/

    std::vector<std::string> list = {"Hi", "There", "Tanner"};

    std::cout << formatList(list) << std::endl;

    return 0;
}
