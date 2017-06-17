#include "gwentPlayer.h"

GwentPlayer::GwentPlayer(int id){
    this->id = id;
    this->pointTotal = 0;
}


GwentPlayer::draw(){
    //Return if deck is empty.
    if (deck.size() == 0)
        return;

    //Draw the top card of the deck.
    hand.push_back(deck.front());
    deck.pop_front();
}
