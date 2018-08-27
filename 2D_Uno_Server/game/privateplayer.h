#ifndef PRIVATEPLAYER_H
#define PRIVATEPLAYER_H

#include <string>
#include <unordered_map>
#include <algorithm>
#include <iostream>

#include "card.h"

using std::string;
using std::unordered_map;
using std::find;

namespace game
{

class PrivatePlayer
{
public:
    typedef unordered_map<unsigned int, const Card*> CardMap;

    PrivatePlayer(const string& name);

    string getName() const;
    const CardMap& getCards() const;

    void addCard(const Card* card);
    bool removeCard(const Card* card);

    unsigned int cardCount() const;

private:
    const string name_;
    CardMap cards_;
};

}

#endif // PRIVATEPLAYER_H
