#include "gwentMechanics.h"

//Function implementations.

//Adds a tag to the target card (such as resilient, regressing, etc).
void addTag(GwentCard &target, const QString tag){
    target.tags.push_back(tag);
}

//Adds armor to the target.
void addArmor(GwentCard &target, const int amount){
    target.currentArmor += amount;
}

//Boost the target for the amount.  The board will not change with this effect, so we don't need to pass it.
void boost(GwentCard &target, const int amount){
    target.currentPower += amount;
}

//Banish the card at the position.  Takes the target and the player who the card is on the side of as inputs.  Note that the banished zone is a member of GwentBoard as it is public info.
void banish(GwentCard &target, GwentPlayer &player, GwentBoard &board){
    //Position works as follows:
    //Note that position is passed in the form of a 4d size_t var.
    //First number is board / graveyard / hand / deck respectively, 0 1 2 3.
    //Second number is row number (only relevant for board).  Melee, ranged, siege is 0 1 2.
    //Third number is index in the vector / deque.
    //Fourth number is which player's side it is on, same as GwentPlayer.id (1 or 2).
    std::vector<size_t> position = target.position;

    //To which player's banished zone depends upon where it was banished.
    if (target.position[3] == 1){
        board.playerOneBanished.push_back(target);
    }
    else{
        board.playerTwoBanished.push_back(target);
    }
    //We now remove the card from where it was originally.
    removeCard(target, player, board);
}

//Consume functions.  If the target is in the hand / field it is sent to grave, else if in grave it is banished.  Note: should activate a "consumed trigger", as other mechanics shall with their triggers.
void consume(GwentCard &target, GwentPlayer &player, GwentBoard &board){

}

//Takes the target and damage amount as an input.  We pass the target by reference.  If the target dies, we move it to graveyard / banish it.
//We also pass the player as an argument as some cards can damage cards in hand (such as Nilfguard revealing the highest card in one's hand and setting it to one strength).
void damage(GwentCard &target, const int amount, GwentBoard &board, GwentPlayer &player);

//Demote the card to silver if gold.  If bronze stays bronze.
void demote(GwentCard &target);

//Destroy the target.
void destroy(GwentCard &target, GwentPlayer &player, GwentBoard &board);

//Discard the card to graveyard from hand.  Takes the index in hand and uses that information to send to the graveyard.
//Takes the player as a passed by reference input such that we can remove and send the card to the graveyard.
//We could store the card position in the card variable.
//Thus, functions such as this require us to pass the position.
void discard(GwentCard &target, GwentPlayer &player, GwentBoard &board);

//While draw is a member function of GwentPlayer, it is also a mechanic from cards such as spies.
void draw(GwentPlayer &player);

//Heal the target.  Can take just the card as input as will not change the board.
void heal(GwentCard &target);

//Returns the highest card on the board.
GwentCard highest(const GwentBoard board);

//Locks the target, does not affect board.
void lock(GwentCard &target);

//Finds the lowest card on the board.
GwentCard lowest(const GwentBoard board);

//Mulligan the card at the target position (in hand).
void mulligan(const size_t index, GwentPlayer &player);

//Promote: convert the target to gold until the end of the round.
void promote(GwentCard &target);

//Remove a card (such as from the board / hand / deck) using its position member.
void removeCard(GwentCard &target, GwentPlayer &player, GwentBoard &board){
    //Position works as follows:
    //Note that position is passed in the form of a 4d size_t var.
    //First number is board / graveyard / hand / deck respectively, 0 1 2 3.
    //Second number is row number (only relevant for board).  Melee, ranged, siege is 0 1 2.
    //Third number is index in the vector / deque.
    //Fourth number is which player's side it is on, same as GwentPlayer.id (1 or 2).
    std::vector<size_t> position = target.position;

    //Handle the case that it is on the board.
    if (position[0] == 0){
        //We now need to know the right row to remove it from.
        if (position[1] == 0){
            //Melee row.
            //It can be on either player's side.
            if (position[3] == 1){
                board.playerOneMelee.erase(board.playerOneMelee.begin()+position[2]);
            }
            else{
                board.playerTwoMelee.erase(board.playerTwoMelee.begin()+position[2]);
            }
        }
        else if (position[1] == 1){
            //Ranged row.
            //It can be on either player's side.
            if (position[3] == 1){
                board.playerOneRanged.erase(board.playerOneRanged.begin()+position[2]);
            }
            else{
                board.playerTwoRanged.erase(board.playerTwoRanged.begin()+position[2]);
            }
        }
        else if (position[1] == 2){
            //Siege row.
            //It can be on either player's side.
            if (position[3] == 1){
                board.playerOneSiege.erase(board.playerOneSiege.begin()+position[2]);
            }
            else{
                board.playerTwoSiege.erase(board.playerTwoSiege.begin()+position[2]);
            }
        }
    }
    //Account for the event that we must remove from the graveyard.  Note that graveyard is a member of board.playerOneSide.graveyard (ex).
    else if (position[0] == 1)
        return;
    //LEAVING OFF HERE
    return;
}

//Reset the unit to its base power.
void reset(GwentCard &target);

//Resurrect the target from the graveyard.
void resurrect(GwentCard &target, GwentPlayer &player, GwentBoard &board);

//Reveal the target.
void reveal(GwentCard &target);

//Spawn the target at the position.  Adds the card to the game and plays it.  Note that not all of position is needed (it can only be on the board, so need side, row number, and index, but we take the normal size for consistency.
void spawn(GwentCard &target, const size_t position[4], GwentBoard &board);

//Summon the card from the deck.  Similair to spawn, but will remove the copy from the player's deck.
void summon(GwentCard &target, const size_t position[4], GwentPlayer &player, GwentBoard &board);

//Transform the card into a different card.
void transform(GwentCard &target, const GwentCard transformedTo);

//Strengthen the target for the amount.  The board will not change with this effect, so we don't need to pass it.
void strengthen(GwentCard &target, const int amount);

//Takes the target and weaken amount as an input.  We pass the target by reference.  If the target dies, we move it to graveyard / banish it.
void weaken(GwentCard &target, GwentPlayer &player, const int amount, GwentBoard &board);


