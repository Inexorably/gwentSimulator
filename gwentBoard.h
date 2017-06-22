#ifndef BOARD_H
#define BOARD_H

#include "gwentCard.h"
#include <vector>

class GwentBoard
{
public:
    //Default constructor.
    GwentBoard();

    // Each game will have a board
    std::vector<GwentCard> playerOneMelee;
    std::vector<GwentCard> playerOneRanged;
    std::vector<GwentCard> playerOneSiege;

    std::vector<GwentCard> playerTwoMelee;
    std::vector<GwentCard> playerTwoRanged;
    std::vector<GwentCard> playerTwoSiege;

    //The graveyard and banished zone.
    std::vector<GwentCard> playerOneGraveyard;
    std::vector<GwentCard> playerOneBanished;

    std::vector<GwentCard> playerTwoGraveyard;
    std::vector<GwentCard> playerTwoBanished;


};

#endif // BOARD_H
