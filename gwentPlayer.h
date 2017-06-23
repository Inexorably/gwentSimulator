#ifndef GWENTPLAYER_H
#define GWENTPLAYER_H

#include "gwentCard.h"
#include <vector>
#include <deque>

#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <algorithm>    // std::shuffle


class GwentPlayer{
public:
    GwentPlayer();
    GwentPlayer(int id);

    //Some utility functions for the player.  Think, how should AI be stored.  Prob will run in game?
    void draw();
    void shuffleDeck();
    int id;

    //some var for boardState board
    std::deque<GwentCard> deck;
    std::vector<GwentCard> hand;

    int roundsWon;

};

#endif // GWENTPLAYER_H
