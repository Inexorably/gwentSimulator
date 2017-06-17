#ifndef GWENTPLAYER_H
#define GWENTPLAYER_H

#include "gwentCard.h"
#include <vector>
#include <deque>


class GwentPlayer
{
public:
    GwentPlayer( int id );

    //Some utility functions for the player.  Think, how should AI be stored.  Prob will run in game?
    int getPointTotal();
    void draw();
    void shuffleDeck();

private:
    int id;
    int pointTotal;
    int roundsWon;

    //some var for boardState board
    std::deque<GwentCard> deck;
    std::vector<GwentCard> hand;

};

#endif // GWENTPLAYER_H
