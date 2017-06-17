#include "gwentMechanics.h"

//Function implementations.

//Adds a tag to the target card (such as resilient, regressing, etc).
void addTag(gwentCard &target, const QString tag){
    target.tags.push_back(tag);
}

//Adds armor to the target.
void addArmor(gwentCard &target, const int amount){
    target.currentArmor += amount;
}

//Boost the target for the amount.  The board will not change with this effect, so we don't need to pass it.
void boost(gwentCard &target, const int amount){
    target.currentPower += amount;
}

//Banish the card at the position.  Takes the target and the player who the card is on the side of as inputs.  Note that the banished zone is a member of gwentBoard as it is public info.
void banish(gwentCard &target, gwentPlayer &player, gwentBoard &board){
    //Position works as follows:
    //Note that position is passed in the form of a 4d size_t var.
    //First number is board / graveyard / hand / deck respectively, 0 1 2 3.
    //Second number is row number (only relevant for board).  Melee, ranged, siege is 0 1 2.
    //Third number is index in the vector / deque.
    //Fourth number is which player's side it is on, same as gwentPlayer.id (1 or 2).
    size_t position[4] = target.position;

    //To which player's banished zone depends upon where it was banished.
    if (target.position[3] == 1){
        board.playerOneSide.banished.push_back(target);
    }
    else{
        board.playerTwoSide.banished.push_back(target);
    }
    //We now remove the card from where it was originally.
    removeCard(gwentCard &target, gwentPlayer &player, gwentBoard &board);
}

//Consume functions.  If the target is in the hand / field it is sent to grave, else if in grave it is banished.  Note: should activate a "consumed trigger", as other mechanics shall with their triggers.
void consume(gwentCard &target, gwentPlayer &player, gwentBoard &board);

//Takes the target and damage amount as an input.  We pass the target by reference.  If the target dies, we move it to graveyard / banish it.
//We also pass the player as an argument as some cards can damage cards in hand (such as Nilfguard revealing the highest card in one's hand and setting it to one strength).
void damage(gwentCard &target, const int amount, gwentBoard &board, gwentPlayer &player);

//Demote the card to silver if gold.  If bronze stays bronze.
void demote(gwentCard &target);

//Destroy the target.
void destroy(gwentCard &target, gwentPlayer &player, gwentBoard &board);

//Discard the card to graveyard from hand.  Takes the index in hand and uses that information to send to the graveyard.
//Takes the player as a passed by reference input such that we can remove and send the card to the graveyard.
//We could store the card position in the card variable.
//Thus, functions such as this require us to pass the position.
void discard(gwentCard &target, gwentPlayer &player, gwentBoard &board);

//While draw is a member function of gwentPlayer, it is also a mechanic from cards such as spies.
void draw(gwentPlayer &player);

//Heal the target.  Can take just the card as input as will not change the board.
void heal(gwentCard &target);

//Returns the highest card on the board.
gwentCard highest(const gwentBoard board);

//Locks the target, does not affect board.
void lock(gwentCard &target);

//Finds the lowest card on the board.
gwentCard lowest(const gwentBoard board);

//Mulligan the card at the target position (in hand).
void mulligan(const size_t index, gwentPlayer &player);

//Promote: convert the target to gold until the end of the round.
void promote(gwentCard &target);

//Remove a card (such as from the board / hand / deck) using its position member.
void removeCard(gwentCard &target, gwentPlayer &player, gwentBoard &board){
    //Position works as follows:
    //Note that position is passed in the form of a 4d size_t var.
    //First number is board / graveyard / hand / deck respectively, 0 1 2 3.
    //Second number is row number (only relevant for board).  Melee, ranged, siege is 0 1 2.
    //Third number is index in the vector / deque.
    //Fourth number is which player's side it is on, same as gwentPlayer.id (1 or 2).
    size_t position[4] = target.position;

    //Handle the case that it is on the board.
    if (position[0] == 0){
        //We now need to know the right row to remove it from.
        if (position[1] == 0){
            //Melee row.
            //It can be on either player's side.
            if (position[3] == 1){
                board.playerOneSide.melee.erase(board.playerOneSide.melee.begin()+position[2]);
            }
            else{
                board.playerTwoSide.melee.erase(board.playerTwoSide.melee.begin()+position[2]);
            }
        }
        else if (position[1] == 1){
            //Ranged row.
            //It can be on either player's side.
            if (position[3] == 1){
                board.playerOneSide.ranged.erase(board.playerOneSide.ranged.begin()+position[2]);
            }
            else{
                board.playerTwoSide.ranged.erase(board.playerTwoSide.ranged.begin()+position[2]);
            }
        }
        else if (position[1] == 2){
            //Siege row.
            //It can be on either player's side.
            if (position[3] == 1){
                board.playerOneSide.siege.erase(board.playerOneSide.siege.begin()+position[2]);
            }
            else{
                board.playerTwoSide.siege.erase(board.playerTwoSide.siege.begin()+position[2]);
            }
        }
    }
    //Account for the event that we must remove from the graveyard.  Note that graveyard is a member of board.playerOneSide.graveyard (ex).
    else if (position[0] == 1)
        Leaving off here
}

//Reset the unit to its base power.
void reset(gwentCard &target);

//Resurrect the target from the graveyard.
void resurrect(gwentCard &target, gwentPlayer &player, gwentBoard &board);

//Reveal the target.
void reveal(gwentCard &target);

//Spawn the target at the position.  Adds the card to the game and plays it.  Note that not all of position is needed (it can only be on the board, so need side, row number, and index, but we take the normal size for consistency.
void spawn(gwentCard &target, const size_t position[4], gwentBoard &board);

//Summon the card from the deck.  Similair to spawn, but will remove the copy from the player's deck.
void summon(gwentCard &target, const size_t position[4], gwentPlayer &player, gwentBoard &board);

//Transform the card into a different card.
void transform(gwentCard &target, const gwentCard transformedTo);

//Strengthen the target for the amount.  The board will not change with this effect, so we don't need to pass it.
void strengthen(gwentCard &target, const int amount);

//Takes the target and weaken amount as an input.  We pass the target by reference.  If the target dies, we move it to graveyard / banish it.
void weaken(gwentCard &target, gwentPlayer &player, const int amount, gwentBoard &board);


