#include "gwentPlayer.h"

GwentPlayer::GwentPlayer(){
    id = -1;
}

GwentPlayer::GwentPlayer(int id){
    this->id = id;
    this->pointTotal = 0;
}


void GwentPlayer::draw(){
    //Return if deck is empty.
    if (deck.size() == 0)
        return;

    //Draw the top card of the deck.
    hand.push_back(deck.front());
    deck.pop_front();
}

int GwentPlayer::getPointTotal(){
    return pointTotal;
}

void GwentPlayer::shuffleDeck(){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle (this->deck.begin(), this->deck.end(), std::default_random_engine(seed));
}
