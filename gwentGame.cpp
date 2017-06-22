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

    for (size_t i = 0; i < this->playerOne.hand.size(); i++){
        this->playerOne.hand[i].zone = 2;
        this->playerOne.hand[i].row = -1;
        this->playerOne.hand[i].index = i;
        this->playerOne.hand[i].side = 1;
    }
    for (size_t i = 0; i < this->playerTwo.hand.size(); i++){
        this->playerTwo.hand[i].zone = 2;
        this->playerTwo.hand[i].row = -1;
        this->playerTwo.hand[i].index = i;
        this->playerTwo.hand[i].side = 2;
    }
    for (size_t i = 0; i < this->board.playerOneMelee.size(); i++){
        this->board.playerOneMelee[i].zone = 0;
        this->board.playerOneMelee[i].row = 0;
        this->board.playerOneMelee[i].index = i;
        this->board.playerOneMelee[i].side = 1;
    }
    for (size_t i = 0; i < this->board.playerTwoMelee.size(); i++){
        this->board.playerTwoMelee[i].zone = 0;
        this->board.playerTwoMelee[i].row = 0;
        this->board.playerTwoMelee[i].index = i;
        this->board.playerTwoMelee[i].side = 2;
    }
    for (size_t i = 0; i < this->board.playerOneRanged.size(); i++){
        this->board.playerOneRanged[i].zone = 0;
        this->board.playerOneRanged[i].row = 1;
        this->board.playerOneRanged[i].index = i;
        this->board.playerOneRanged[i].side = 1;
    }
    for (size_t i = 0; i < this->board.playerTwoRanged.size(); i++){
        this->board.playerTwoRanged[i].zone = 0;
        this->board.playerTwoRanged[i].row = 1;
        this->board.playerTwoRanged[i].index = i;
        this->board.playerTwoRanged[i].side = 2;
    }
    for (size_t i = 0; i < this->board.playerOneSiege.size(); i++){
        this->board.playerOneSiege[i].zone = 0;
        this->board.playerOneSiege[i].row = 2;
        this->board.playerOneSiege[i].index = i;
        this->board.playerOneSiege[i].side = 1;
    }
    for (size_t i = 0; i < this->board.playerTwoSiege.size(); i++){
        this->board.playerTwoSiege[i].zone = 0;
        this->board.playerTwoSiege[i].row = 2;
        this->board.playerTwoSiege[i].index = i;
        this->board.playerTwoSiege[i].side = 2;
    }
    for (size_t i = 0; i < this->board.playerOneGraveyard.size(); i++){
        this->board.playerOneGraveyard[i].zone = 1;
        this->board.playerOneGraveyard[i].row = -1;
        this->board.playerOneGraveyard[i].index = i;
        this->board.playerOneGraveyard[i].side = 1;
    }
    for (size_t i = 0; i < this->board.playerTwoGraveyard.size(); i++){
        this->board.playerTwoGraveyard[i].zone = 1;
        this->board.playerTwoGraveyard[i].row = -1;
        this->board.playerTwoGraveyard[i].index = i;
        this->board.playerTwoGraveyard[i].side = 2;
    }
    for (size_t i = 0; i < this->playerOne.deck.size(); i++){
        this->playerOne.deck[i].zone = 3;
        this->playerOne.deck[i].row = -1;
        this->playerOne.deck[i].index = i;
        this->playerOne.deck[i].side = 1;
    }
    for (size_t i = 0; i < this->playerTwo.deck.size(); i++){
        this->playerTwo.deck[i].zone = 3;
        this->playerTwo.deck[i].row = -1;
        this->playerTwo.deck[i].index = i;
        this->playerTwo.deck[i].side = 2;
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

//Old mechanics are here.
#include <QDebug>

/*********************************************************/

//Random engine
#include <random>

std::random_device rd;     // only used once to initialise (seed) engine
std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)

int randomInt(int min, int max){
    std::uniform_int_distribution<int> uni(min, max); // guaranteed unbiased
    return uni(rng);
}

/*********************************************************/

//Function implementations.  Confirmed as working if Confirmed is noted above.

//Confirmed.
//Adds a tag to the target card (such as resilient, regressing, etc).
void GwentGame::addTag(GwentCard &target, const QString tag){
    target.tags.push_back(tag);
}

//Confirmed.
//Adds armor to the target.
void GwentGame::addArmor(GwentCard &target, const int amount){
    target.currentArmor += amount;
}

//Confirmed.
//Boost the target for the amount.  The board will not change with this effect, so we don't need to pass it.
void GwentGame::boost(GwentCard &target, const int amount){
    target.currentPower += amount;
}

//Banish the card at the position.
void GwentGame::banish(GwentCard &target){
    //Position works as follows:
    //Note that position is passed in the form of a 4d size_t var.
    //First number is board / graveyard / hand / deck respectively, 0 1 2 3.  4 for banished.
    //Second number is row number (only relevant for board).  Melee, ranged, siege is 0 1 2.
    //Third number is index in the vector / deque.
    //Fourth number is which player's side it is on, same as GwentPlayer.id (1 or 2).

    //When a card is banished / destroyed / discard, its power is reset.
    target.resetToBaseCopy();

    GwentCard original = target;

    //To which player's banished zone depends upon where it was banished.
    if (target.side == 1){
        target.zone = 4;
        target.row = -1;
        target.index = (int)board.playerOneBanished.size();
        board.playerOneBanished.push_back(target);
        //qDebug() << target.name << " banished to " << QString::number(target.position[0]) << QString::number(target.position[1]) << QString::number(target.position[2]) << QString::number(target.position[3]);
    }
    else{
        target.zone = 4;
        target.index = (int)board.playerTwoBanished.size();
        board.playerTwoBanished.push_back(target);
    }
    //We now remove the card from where it was originally.
    removeCard(original);

    //qDebug() << target.name << " banished to " << QString::number(target.position.size());// << QString::number(target.position[1]) << QString::number(target.position[2]) << QString::number(target.position[3]);
}

//Consume functions.  If the target is in the hand / field it is sent to grave, else if in grave it is banished.  Note: should activate a "consumed trigger", as other mechanics shall with their triggers.
//Takes card consuming as an input too such that it can buff the consumed card.  Only grave hag will need to be hard coded for this.
void GwentGame::consume(GwentCard &cardConsuming, GwentCard &target){
    GwentCard original = target;

    //Boost the card that consumed the target by the current power of the target.
    boost(cardConsuming, target.currentPower);

    //When a card is banished / destroyed / discard, its power is reset.
    target.resetToBaseCopy();

    //Note that position is passed in the form of a 4d size_t var.
    //First number is board / graveyard / hand / deck respectively, 0 1 2 3, 4 for banished.
    //Second number is row number (only relevant for board).  Melee, ranged, siege is 0 1 2.
    //Third number is index in the vector / deque.
    //Fourth number is which player's side it is on, same as gwentPlayer.id (1 or 2).

    //To which player's graveyard/banished zone depends upon where it was consumed from.
    if (target.side == 1){
        //In the case the the target is consumed from the hand / deck / board, it is sent to the graveyard.
        if (target.zone == 0 || target.zone == 2 || target.zone == 3){
            //We update the position of target.
            target.zone = 1;
            target.row = -1;
            target.index = board.playerOneGraveyard.size();
            board.playerOneGraveyard.push_back(target);
            removeCard(original);
        }
        //In the case it is consumed from grave, it is banished.
        else if (target.zone == 1){
            //We update the position of target.
            target.zone = 4;
            target.index = board.playerOneBanished.size();
            target.row = -1;
            board.playerOneBanished.push_back(target);
            removeCard(original);
        }
    }
    else{
        //In the case the the target is consumed from the hand / deck / board, it is sent to the graveyard.
        if (target.zone == 0 || target.zone == 2 || target.zone == 3){
            //We update the position of target.
            target.zone = 1;
            target.index = board.playerTwoGraveyard.size();
            target.row = -1;
            board.playerTwoGraveyard.push_back(target);
            removeCard(original);
        }
        //In the case it is consumed from grave, it is banished.
        else if (target.zone == 1){
            //We update the position of target.
            target.zone = 4;
            target.index = board.playerTwoBanished.size();
            target.row = -1;
            board.playerTwoBanished.push_back(target);
            removeCard(original);
        }
    }
}

//Takes the target and damage amount as an input.  We pass the target by reference.  If the target dies, we move it to graveyard / banish it.
//We also pass the player as an argument as some cards can damage cards in hand (such as Nilfguard revealing the highest card in one's hand and setting it to one strength).
void GwentGame::damage(GwentCard &target, int amount){
    //Handle the case where the target does not die.
    if (target.currentArmor >= amount){
        target.currentArmor -= amount;
        return;
    }
    else if (target.currentArmor > 0){
        amount -= target.currentArmor;
        target.currentArmor = 0;
    }
    //Now either the target will tank the post armor damage or die.
    //Account for target tanking it.
    if (target.currentPower > amount){
        target.currentPower -= amount;
        return;
    }
    //Else the target dies.
    else if (target.currentPower <= amount){
        destroy(target);
    }
    return;
}

//Demote the card to silver if gold.  If bronze stays bronze.
void GwentGame::demote(GwentCard &target){
    if (target.rank == "Gold")
        target.rank = "Silver";
    else if (target.rank == "Silver")
        target.rank = "Bronze";
}

//Destroy the target.
void GwentGame::destroy(GwentCard &target){
    GwentCard copy = target;

    //When a card is banished / destroyed / discard, its power is reset.
    target.resetToBaseCopy();

    //To which player's graveyard zone depends upon where it was killed.
    if (target.side == 1){
        //We update the position of target.
        target.zone = 1;
        target.index = board.playerOneGraveyard.size();
        target.row = -1;
        board.playerOneGraveyard.push_back(target);
    }
    else{
        //We update the position of target.
        target.zone = 1;
        target.index = board.playerTwoGraveyard.size();
        target.row = -1;
        board.playerTwoGraveyard.push_back(target);
    }
    //We now remove the card from where it was originally.
    removeCard(copy);
}

//Discard the card to graveyard from hand.  Takes the index in hand and uses that information to send to the graveyard.
//Takes the player as a passed by reference input such that we can remove and send the card to the graveyard.
//We could store the card position in the card variable.
//Thus, functions such as this require us to pass the position.
void GwentGame::discard(GwentCard &target){
    //NOTE: HANDLE POSITION UPDATING PROPERLY AS DONE WITH GWENTCARD ORIGINAL IN BANISH FUNCTION.
    GwentCard copy = target;
    //When a card is banished / destroyed / discard, its power is reset.
    target.resetToBaseCopy();


    //Note that position is passed in the form of a 3d size_t var.
    //First number is board / graveyard / hand / deck respectively, 0 1 2 3, 4 for banished.
    //Second number is row number (only relevant for board).  Melee, ranged, siege is 0 1 2.
    //Third number is index in the vector / deque.
    //Fourth number is which player's side it is on, same as gwentPlayer.id (1 or 2).

    //To which player's grave zone depends upon where it was banished.
    if (target.side == 1){
        //We update the position of target.
        target.zone = 1;
        target.index = board.playerOneGraveyard.size();
        target.row = -1;
        board.playerOneGraveyard.push_back(target);
    }
    else{
        //We update the position of target.
        target.zone = 1;
        target.row = -1;
        target.index = board.playerOneGraveyard.size();
        board.playerTwoGraveyard.push_back(target);
    }
    //We now remove the card from where it was originally.
    removeCard(copy);
}

//While draw is a member function of GwentPlayer, it is also a mechanic from cards such as spies.
void GwentGame::draw(GwentPlayer &player){
    player.draw();
}

//Heal the target.  Can take just the card as input as will not change the board.
void GwentGame::heal(GwentCard &target){
    if (target.isWounded())
        target.currentPower = target.basePower;
}

//Returns a vector of pointers to the highestCards on the row.
std::vector<GwentCard*> GwentGame::highestOnRow(std::vector<GwentCard> row){
    GwentCard temp("Temp", "", 0, "", "", "", 0);
    std::vector<GwentCard*> highestPower;
    if (row.empty()){
        highestPower.clear();
        highestPower.push_back(nullptr);
        return highestPower;
    }
    highestPower.push_back(&temp);
    for (size_t i = 0; i < row.size(); i++){
        if (row[i].currentPower > highestPower[0]->currentPower){
            highestPower.clear();
            highestPower.push_back(&row[i]);
        }
        else if (row[i].currentPower == highestPower[0]->currentPower){
            highestPower.push_back(&row[i]);
        }
    }
    return highestPower;
}

//Returns a vector of pointers to the highest card on the board.
std::vector<GwentCard*> GwentGame::highest(){
    GwentCard temp("Temp", "", 0, "", "", "", 0);
    std::vector<GwentCard*> highestPower;
    highestPower.push_back(&temp);
    for (size_t i = 0; i < board.playerOneMelee.size(); i++){
        if (board.playerOneMelee[i].currentPower > highestPower[0]->currentPower){
            highestPower.clear();
            highestPower.push_back(&board.playerOneMelee[i]);
        }
        else if (board.playerOneMelee[i].currentPower == highestPower[0]->currentPower){
            highestPower.push_back((&board.playerOneMelee[i]));
        }
    }
    for (size_t i = 0; i < board.playerOneRanged.size(); i++){
            if (board.playerOneRanged[i].currentPower > highestPower[0]->currentPower){
                highestPower.clear();
                highestPower.push_back(&board.playerOneRanged[i]);
            }
            else if (board.playerOneRanged[i].currentPower == highestPower[0]->currentPower){
                highestPower.push_back((&board.playerOneRanged[i]));
            }
    }
    for (size_t i = 0; i < board.playerOneSiege.size(); i++){
            if (board.playerOneSiege[i].currentPower > highestPower[0]->currentPower){
                highestPower.clear();
                highestPower.push_back(&board.playerOneSiege[i]);
            }
            else if (board.playerOneSiege[i].currentPower == highestPower[0]->currentPower){
                highestPower.push_back((&board.playerOneSiege[i]));
            }
    }
    for (size_t i = 0; i < board.playerTwoMelee.size(); i++){
            if (board.playerTwoMelee[i].currentPower > highestPower[0]->currentPower){
                highestPower.clear();
                highestPower.push_back(&board.playerTwoMelee[i]);
            }
            else if (board.playerTwoMelee[i].currentPower == highestPower[0]->currentPower){
                highestPower.push_back((&board.playerTwoMelee[i]));
            }
    }
    for (size_t i = 0; i < board.playerTwoRanged.size(); i++){
            if (board.playerTwoRanged[i].currentPower > highestPower[0]->currentPower){
                highestPower.clear();
                highestPower.push_back(&board.playerTwoRanged[i]);
            }
            else if (board.playerTwoRanged[i].currentPower == highestPower[0]->currentPower){
                highestPower.push_back((&board.playerTwoRanged[i]));
            }
    }
    for (size_t i = 0; i < board.playerTwoSiege.size(); i++){
            if (board.playerTwoSiege[i].currentPower > highestPower[0]->currentPower){
                highestPower.clear();
                highestPower.push_back(&board.playerTwoSiege[i]);
            }
            else if (board.playerTwoSiege[i].currentPower == highestPower[0]->currentPower){
                highestPower.push_back((&board.playerTwoSiege[i]));
            }
    }
    if (highestPower[0]->name == "Temp"){
        highestPower[0] = nullptr;
    }
    return highestPower;
}

//Locks the target, does not affect board.
void GwentGame::lock(GwentCard &target){
    //Clear the tags on the target if the target is going to be locked.
    if (!target.locked)
        target.tags.clear();
    target.locked = !target.locked;
}

//Find the lowest card(s) on the row and returns pointers to them.
std::vector<GwentCard*> GwentGame::lowestOnRow(std::vector<GwentCard> row){
    GwentCard temp("Temp", "", 99999, "", "", "", 99999);
    std::vector<GwentCard*> lowestPower;
    if (row.empty()){
        lowestPower.clear();
        lowestPower.push_back(nullptr);
        return lowestPower;
    }
    lowestPower.push_back(&temp);
    for (size_t i = 0; i < row.size(); i++){
        if (row[i].currentPower < lowestPower[0]->currentPower){
            lowestPower.clear();
            lowestPower.push_back(&row[i]);
        }
        else if (row[i].currentPower == lowestPower[0]->currentPower){
            lowestPower.push_back(&row[i]);
        }
    }
    return lowestPower;
}

//Finds the lowest card(s) on the board.
std::vector<GwentCard*> GwentGame::lowest(){
    GwentCard temp("Temp", "", 0, "", "", "", 0);
    std::vector<GwentCard*> lowestPower;
    lowestPower.push_back(&temp);
    for (size_t i = 0; i < board.playerOneMelee.size(); i++){
        if (board.playerOneMelee[i].currentPower < lowestPower[0]->currentPower){
            lowestPower.clear();
            lowestPower.push_back(&board.playerOneMelee[i]);
        }
        else if (board.playerOneMelee[i].currentPower == lowestPower[0]->currentPower){
            lowestPower.push_back((&board.playerOneMelee[i]));
        }
    }
    for (size_t i = 0; i < board.playerOneRanged.size(); i++){
            if (board.playerOneRanged[i].currentPower < lowestPower[0]->currentPower){
                lowestPower.clear();
                lowestPower.push_back(&board.playerOneRanged[i]);
            }
            else if (board.playerOneRanged[i].currentPower == lowestPower[0]->currentPower){
                lowestPower.push_back((&board.playerOneRanged[i]));
            }
    }
    for (size_t i = 0; i < board.playerOneSiege.size(); i++){
            if (board.playerOneSiege[i].currentPower < lowestPower[0]->currentPower){
                lowestPower.clear();
                lowestPower.push_back(&board.playerOneSiege[i]);
            }
            else if (board.playerOneSiege[i].currentPower == lowestPower[0]->currentPower){
                lowestPower.push_back((&board.playerOneSiege[i]));
            }
    }
    for (size_t i = 0; i < board.playerTwoMelee.size(); i++){
            if (board.playerTwoMelee[i].currentPower < lowestPower[0]->currentPower){
                lowestPower.clear();
                lowestPower.push_back(&board.playerTwoMelee[i]);
            }
            else if (board.playerTwoMelee[i].currentPower == lowestPower[0]->currentPower){
                lowestPower.push_back((&board.playerTwoMelee[i]));
            }
    }
    for (size_t i = 0; i < board.playerTwoRanged.size(); i++){
            if (board.playerTwoRanged[i].currentPower < lowestPower[0]->currentPower){
                lowestPower.clear();
                lowestPower.push_back(&board.playerTwoRanged[i]);
            }
            else if (board.playerTwoRanged[i].currentPower == lowestPower[0]->currentPower){
                lowestPower.push_back((&board.playerTwoRanged[i]));
            }
    }
    for (size_t i = 0; i < board.playerTwoSiege.size(); i++){
            if (board.playerTwoSiege[i].currentPower < lowestPower[0]->currentPower){
                lowestPower.clear();
                lowestPower.push_back(&board.playerTwoSiege[i]);
            }
            else if (board.playerTwoSiege[i].currentPower == lowestPower[0]->currentPower){
                lowestPower.push_back((&board.playerTwoSiege[i]));
            }
    }
    if (lowestPower[0]->name == "Temp"){
        lowestPower[0] = nullptr;
    }
    return lowestPower;
}

//Mulligan the card at the target position (in hand).
void GwentGame::mulligan(const size_t index, GwentPlayer &player){
    //Remember to implement blacklist vector (likely member of player).
    //TODO: NOTE: will require a blacklist vector of type GwentCard to be temp held in the player.  Can clear after mulligan phase ends in each round.
    return;
}

//Play a card from hand.
void GwentGame::play(GwentCard &target, int row, int index, int side){
    //We can code this is spawning a copy and removing from the deck using the remove function.
    //The reason we make a copy is that in the spawn function the position member of the target card will be modified (as it should be).  We need this position for the remove function too.
    GwentCard copy = target;
    spawn(copy, row, index, side);
    //We now remove the card from the deck.
    removeCard(target);
}

//Promote: convert the target to gold until the end of the game.
void GwentGame::promote(GwentCard &target){
    target.rank = 'g';
}

//Remove a card (such as from the board / hand / deck) using its position member.
void GwentGame::removeCard(GwentCard &target){
    //Position works as follows:
    //Note that position is passed in the form of a 4d size_t var.
    //First number is board / graveyard / hand / deck respectively, 0 1 2 3.
    //Second number is row number (only relevant for board).  Melee, ranged, siege is 0 1 2.
    //Third number is index in the vector / deque.
    //Fourth number is which player's side it is on, same as GwentPlayer.id (1 or 2).

    //Handle the case that it is on the board.
    if (target.zone == 0){
        //We now need to know the right row to remove it from.
        if (target.row == 0){
            //Melee row.
            //It can be on either player's side.
            if (target.side == 1){
                board.playerOneMelee.erase(board.playerOneMelee.begin()+target.index);
            }
            else{
                board.playerTwoMelee.erase(board.playerTwoMelee.begin()+target.index);
            }
        }
        else if (target.row == 1){
            //Ranged row.
            //It can be on either player's side.
            if (target.side == 1){
                board.playerOneRanged.erase(board.playerOneRanged.begin()+target.index);
            }
            else{
                board.playerTwoRanged.erase(board.playerTwoRanged.begin()+target.index);
            }
        }
        else if (target.row == 2){
            //Siege row.
            //It can be on either player's side.
            if (target.side == 1){
                board.playerOneSiege.erase(board.playerOneSiege.begin()+target.index);
            }
            else{
                board.playerTwoSiege.erase(board.playerTwoSiege.begin()+target.index);
            }
        }
        return;
    }
    //Account for the event that we must remove from the graveyard.
    else if (target.zone == 1){
        //It can be either player's graveyard.
        if (target.side == 1){
            board.playerOneGraveyard.erase(board.playerOneGraveyard.begin()+target.index);
        }
        else
            board.playerTwoGraveyard.erase(board.playerTwoGraveyard.begin()+target.index);
        return;
    }
    //We account for if we are removing from the deck.
    else if (target.zone == 3){
        //It can be either player's deck.
        if (target.side == 1)
            playerOne.deck.erase(playerOne.deck.begin()+target.index);
        else
            playerTwo.deck.erase(playerTwo.deck.begin()+target.index);
        return;
    }
    //The last possibility is it is in the player's hand that we are removing from.
    else{
        if (target.side == 1)
            playerOne.hand.erase(playerOne.hand.begin()+target.index);
        else
            playerTwo.hand.erase(playerTwo.hand.begin()+target.index);
        return;
    }
    qDebug() << "End of function reached: removeCard";
    return;
}

//Reset the unit to its base power.
void GwentGame::reset(GwentCard &target){
    target.currentPower = target.basePower;
}

//Resurrect the target from the graveyard.
void GwentGame::resurrect(GwentCard &target, int row, int index, int side){
    //Spawn a copy and remove the original from the grave.
    GwentCard copy = target;
    spawn(copy, row, index, side);
    //We now remove the original from the grave.
    removeCard(target);
}

//Reveal the target.
void GwentGame::reveal(GwentCard &target){
    target.revealed = true;
}

//Shuffle the player's deck.
void GwentGame::shuffleDeck(GwentPlayer &player){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle (player.deck.begin(), player.deck.end(), std::default_random_engine(seed));
}

//Spawn the target at the position.  Adds the card to the game and plays it.  Note that not all of position is needed (it can only be on the board, so need side, row number, and index, but we take the normal size for consistency.
void GwentGame::spawn(GwentCard target, int row, int index, int side){
    //Note that position is passed in the form of a 3d size_t var.
    //First number is board / graveyard / hand / deck respectively, 0 1 2 3, 4 for banished.
    //Second number is row number (only relevant for board).  Melee, ranged, siege is 0 1 2.
    //Third number is index in the vector / deque.
    //Fourth number is which player's side it is on, same as gwentPlayer.id (1 or 2).
    //Update the position of the target.
    target.row = row;
    target.index = index;
    target.side = side;

    //Figure out which board and side it is to be spawned on.
    if (side == 1){
        //Spawns on player 1's side.
        switch(row){
            case 0:
                //Melee.
                board.playerOneMelee.insert(board.playerOneMelee.begin()+index, target);
                break;
            case 1:
                //Ranged.
                board.playerOneRanged.insert(board.playerOneRanged.begin()+index, target);
                break;
            case 2:
                //Siege.
                board.playerOneSiege.insert(board.playerOneSiege.begin()+index, target);
                break;
            default:
                qDebug() << "Attempting to spawn on in correct row";
                return;
                break;
        }
    }
    else{
        //Spawns on player 2's side.
        switch(row){
            case 0:
                //Melee.
                board.playerTwoMelee.insert(board.playerTwoMelee.begin()+index, target);
                break;
            case 1:
                //Ranged.
                board.playerTwoRanged.insert(board.playerTwoRanged.begin()+index, target);
                break;
            case 2:
                //Siege.
                board.playerTwoSiege.insert(board.playerTwoSiege.begin()+index, target);
                break;
            default:
                qDebug() << "Attempting to spawn on in correct row";
                return;
                break;
        }
    }
}

//Summon the card from the deck.  Similair to spawn, but will remove the copy from the player's deck.
void GwentGame::summon(GwentCard &target, int row, int index, int side){
    //We can code this is spawning a copy and removing from the deck using the remove function.
    //The reason we make a copy is that in the spawn function the position member of the target card will be modified (as it should be).  We need this position for the remove function too.
    GwentCard copy = target;
    spawn(copy, row, index, side);
    //We now remove the card from the deck.
    removeCard(target);
}

//Transform the card into a different card.
void GwentGame::transform(GwentCard &target, const GwentCard transformedTo){
    int row = target.row;
    int side = target.side;
    int index = target.index;
    int zone = target.zone;
    target = transformedTo;
    target.row = row;
    target.side = side;
    target.index = index;
    target.zone = zone;
}

//Strengthen the target for the amount.  The board will not change with this effect, so we don't need to pass it.
void GwentGame::strengthen(GwentCard &target, const int amount){
    target.basePower += amount;
}

//Takes the target and weaken amount as an input.  We pass the target by reference.  If the target dies, we move it to graveyard / banish it.
void GwentGame::weaken(GwentCard &target, const int amount){
    //Handle the case that the target does not die.
    if (target.currentPower - amount > 0 && target.basePower - amount > 0){
        target.currentPower -= amount;
        target.basePower -= amount;
        return;
    }
    //Next we handle the case where the target dies but still has base str / is not banished.
    else if (target.currentPower - amount <= 0 && target.basePower - amount > 0){
        destroy(target);
        return;
    }
    //Otherwise the base str drops below 0 and it is banished.
    else{
        banish(target);
        return;
    }
}

