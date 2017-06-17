#include "gwentGame.h"

/*Random for later:
 *     //Random Source: https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range
    //Usage: auto random_integer = uni(rng);
    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(0, int(deck.size()-1)); // guaranteed unbiased
    auto random_integer = uni(rng);
    */

/***********************************************************************************************/
/*******************************Implementing gwentGame functions********************************/
/***********************************************************************************************/

//Get the id of the current turn player.
int GwentGame::getTurnPlayerId(){
    return turnPlayerId;
}

//Change the current turn player.
void GwentGame::changeTurnPlayer(){
    if (turnPlayerId == 1){
        turnPlayerId == 2;
    }
    else{
        turnPlayerId == 1;
    }
}

int GwentGame::getPlayerPointTotal(int playerId){
    if (playerId == 1){
        return playerOne.getPointTotal();
    }
    else{
        return playerTwo.getPointTotal();
    }
}
