#ifndef GWENTGAME_H
#define GWENTGAME_H

#include "gwentPlayer.h"
#include "gwentBoard.h"
#include "gwentMechanics.h"

class GwentGame{
public:
    GwentGame();
    GwentGame(GwentPlayer playerOne, GwentPlayer playerTwo);
    GwentGame(GwentPlayer playerOne, GwentPlayer playerTwo, GwentBoard board);

    //The current (turn) player.
    int getTurnPlayerId();
    void changeTurnPlayer();
    int getPlayerPointTotal(int playerId);

    //We need two players for the game.  It is fine to have them hard coded in this style and not in a structure because there will only ever be two players in the game.
    //The ai will be passed stuff it needs such as the board.
    GwentPlayer playerOne;
    GwentPlayer playerTwo;
    GwentBoard board;
    int turnPlayerId;
    int turnNumber;
    bool playerOnePassed;
    bool playerTwoPassed;

    //This will occur at the beginning of the game, and draw cards + intialize things like position member for cards and such.
    void startGame();

    //Called in startGame.  Will set the positions for all cards in the game correctly.  Can be expensive.
    void updateCardPositions();

private:
    //The round number (1, 2, 3).

    int roundNumber;

    //Ends the round.  Moves all cards to grave except for resilient.  Will trigger death effects, creates an event for round end.  If the game is not over, calls startRound.
    void endRound();

    //Starts a new round.  Mulligan and stuff depends on the round number, which is member variable round.
    //When the startRound event is triggered, things like olguerd and veteran effects activate.
    void startRound();

    bool gameComplete;
};


#endif // GWENTGAME_H
