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

GwentGame::GwentGame(){
    GwentPlayer playerOneV(1);
    GwentPlayer playerTwoV(2);
    playerOne = playerOneV;
    playerTwo = playerTwoV;
    turnNumber = 0;
    turnPlayerId = 1;
    playerOnePassed = false;
    playerTwoPassed = false;
}

//Get the id of the current turn player.
int GwentGame::getTurnPlayerId(){
    return turnPlayerId;
}

//Change the current turn player.
void GwentGame::changeTurnPlayer(){
    if (turnPlayerId == 1){
        turnPlayerId = 2;
    }
    else{
        turnPlayerId = 1;
    }
}

void GwentGame::setPlayerPassed(int playerId){
    if (playerId == 1){
        playerOnePassed = true;
    }
    else{
        playerTwoPassed = true;
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

//Called in startGame.  Will set the positions for all cards in the game correctly.  Can be expensive.
void GwentGame::updateCardPositions(){
    //We loop through the hand, deck, field, and graves and update positions of cards in those.
    //Position:
    //First number is board / graveyard / hand / deck respectively, 0 1 2 3, 4 for banished.
    //Second number is row number (only relevant for board).  Melee, ranged, siege is 0 1 2.
    //Third number is index in the vector / deque.
    //Fourth number is which player's side it is on, same as gwentPlayer.id (1 or 2).

    //To make this shorter convert to function calls.  Just annoying cause would need to overload to take deque and vector (deck vs others).
    //TODO: NOTE: This^.  Current method is autism.

    for (int i = 0; i < this->playerOne.hand.size(); i++){
        this->playerOne.hand[i].position.clear();
        this->playerOne.hand[i].position.push_back(2);
        this->playerOne.hand[i].position.push_back(-1);
        this->playerOne.hand[i].position.push_back(i);
        this->playerOne.hand[i].position.push_back(1);
    }
    for (int i = 0; i < this->playerTwo.hand.size(); i++){
        this->playerTwo.hand[i].position.clear();
        this->playerTwo.hand[i].position.push_back(2);
        this->playerTwo.hand[i].position.push_back(-1);
        this->playerTwo.hand[i].position.push_back(i);
        this->playerTwo.hand[i].position.push_back(2);
    }
    for (int i = 0; i < this->board.playerOneMelee.size(); i++){
        this->board.playerOneMelee[i].position.clear();
        this->board.playerOneMelee[i].position.push_back(0);
        this->board.playerOneMelee[i].position.push_back(0);
        this->board.playerOneMelee[i].position.push_back(i);
        this->board.playerOneMelee[i].position.push_back(1);
    }
    for (int i = 0; i < this->board.playerTwoMelee.size(); i++){
        this->board.playerTwoMelee[i].position.clear();
        this->board.playerTwoMelee[i].position.push_back(0);
        this->board.playerTwoMelee[i].position.push_back(0);
        this->board.playerTwoMelee[i].position.push_back(i);
        this->board.playerTwoMelee[i].position.push_back(2);
    }
    for (int i = 0; i < this->board.playerOneRanged.size(); i++){
        this->board.playerOneRanged[i].position.clear();
        this->board.playerOneRanged[i].position.push_back(0);
        this->board.playerOneRanged[i].position.push_back(1);
        this->board.playerOneRanged[i].position.push_back(i);
        this->board.playerOneRanged[i].position.push_back(1);
    }
    for (int i = 0; i < this->board.playerTwoRanged.size(); i++){
        this->board.playerTwoRanged[i].position.clear();
        this->board.playerTwoRanged[i].position.push_back(0);
        this->board.playerTwoRanged[i].position.push_back(1);
        this->board.playerTwoRanged[i].position.push_back(i);
        this->board.playerTwoRanged[i].position.push_back(2);
    }
    for (int i = 0; i < this->board.playerOneSiege.size(); i++){
        this->board.playerOneSiege[i].position.clear();
        this->board.playerOneSiege[i].position.push_back(0);
        this->board.playerOneSiege[i].position.push_back(2);
        this->board.playerOneSiege[i].position.push_back(i);
        this->board.playerOneSiege[i].position.push_back(1);
    }
    for (int i = 0; i < this->board.playerTwoSiege.size(); i++){
        this->board.playerTwoSiege[i].position.clear();
        this->board.playerTwoSiege[i].position.push_back(0);
        this->board.playerTwoSiege[i].position.push_back(2);
        this->board.playerTwoSiege[i].position.push_back(i);
        this->board.playerTwoSiege[i].position.push_back(2);
    }
    for (int i = 0; i < this->board.playerOneGraveyard.size(); i++){
        this->board.playerOneGraveyard[i].position.clear();
        this->board.playerOneGraveyard[i].position.push_back(1);
        this->board.playerOneGraveyard[i].position.push_back(-1);
        this->board.playerOneGraveyard[i].position.push_back(i);
        this->board.playerOneGraveyard[i].position.push_back(1);
    }
    for (int i = 0; i < this->board.playerTwoGraveyard.size(); i++){
        this->board.playerTwoGraveyard[i].position.clear();
        this->board.playerTwoGraveyard[i].position.push_back(1);
        this->board.playerTwoGraveyard[i].position.push_back(-1);
        this->board.playerTwoGraveyard[i].position.push_back(i);
        this->board.playerTwoGraveyard[i].position.push_back(2);
    }
    for (int i = 0; i < this->playerOne.deck.size(); i++){
        this->playerOne.deck[i].position.clear();
        this->playerOne.deck[i].position.push_back(3);
        this->playerOne.deck[i].position.push_back(-1);
        this->playerOne.deck[i].position.push_back(i);
        this->playerOne.deck[i].position.push_back(1);
    }
    for (int i = 0; i < this->playerTwo.deck.size(); i++){
        this->playerTwo.deck[i].position.clear();
        this->playerTwo.deck[i].position.push_back(3);
        this->playerTwo.deck[i].position.push_back(-1);
        this->playerTwo.deck[i].position.push_back(i);
        this->playerTwo.deck[i].position.push_back(2);
    }

}


//This will occur at the beginning of the game, and draw cards + intialize things like position member for cards and such.
void GwentGame::startGame(){
    //The players have now been created.  We should now cycle through the deck / hand post mulligan (not implemented) and assign the positions to the cards.
    //Shuffle each player's deck.
    this->playerOne.shuffleDeck();
    this->playerTwo.shuffleDeck();

    //Draw the 12 cards.
    for (int i = 0; i < 12; i++){
        this->playerOne.draw();
        this->playerTwo.draw();
    }

    //TODO: NOTE: Mulligan phase goes here.
    //Note that when we implement that st card that summons itself on mulligan need to also set position member by looping through board for the card.

    //Set the position members of the card. Loop through hand, deck.
    this->updateCardPositions();

    turnNumber = 0;
    turnPlayerId = randomInt(1,2);
}

//Ends the round.  Moves all cards to grave except for resilient.  Will trigger death effects, creates an event for round end.  If the game is not over, calls startRound.
void GwentGame::endRound(){
    //set playerpassed vars to false
    playerOnePassed = false;
    playerTwoPassed = false;
}
