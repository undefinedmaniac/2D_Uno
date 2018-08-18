#ifndef PRIVATEPLAYER_H
#define PRIVATEPLAYER_H

#include <string>
#include <unordered_set>
#include <algorithm>

#include "card.h"

using std::string;
using std::unordered_set;
using std::find;

namespace game
{

class PrivatePlayer
{
public:
    PrivatePlayer(const string& name);

    string getName() const;
    const unordered_set<const Card*>& getCards() const;

    void addCard(const Card* card);
    bool removeCard(const Card* card);

private:
    const string name_;
    unordered_set<const Card*> cards_;
};

}

#endif // PRIVATEPLAYER_H
