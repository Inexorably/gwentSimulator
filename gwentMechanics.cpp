#include "gwentMechanics.h"
#include <QDebug>

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
    //First number is board / graveyard / hand / deck respectively, 0 1 2 3.  4 for banished.
    //Second number is row number (only relevant for board).  Melee, ranged, siege is 0 1 2.
    //Third number is index in the vector / deque.
    //Fourth number is which player's side it is on, same as GwentPlayer.id (1 or 2).
    std::vector<size_t> position = target.position;

    //When a card is banished / destroyed / discard, its power is reset.
    target.currentPower = target.basePower;

    GwentCard original = target;

    //To which player's banished zone depends upon where it was banished.
    if (target.position[3] == 1){
        target.position[0] = 4;
        target.position[2] = (int)board.playerOneBanished.size();
        target.position[3] = 1;
        board.playerOneBanished.push_back(target);
    }
    else{
        target.position[0] = 4;
        target.position[2] = (int)board.playerTwoBanished.size();
        target.position[3] = 2;
        board.playerTwoBanished.push_back(target);
    }
    //We now remove the card from where it was originally.
    removeCard(original, player, board);
}

//Consume functions.  If the target is in the hand / field it is sent to grave, else if in grave it is banished.  Note: should activate a "consumed trigger", as other mechanics shall with their triggers.
//Takes card consuming as an input too such that it can buff the consumed card.  Only grave hag will need to be hard coded for this.
void consume(GwentCard &cardConsuming, GwentCard &target, GwentPlayer &player, GwentBoard &board){
    std::vector<size_t> position = target.position;

    //Boost the card that consumed the target by the current power of the target.
    boost(cardConsuming, target.currentPower);

    //When a card is banished / destroyed / discard, its power is reset.
    target.currentPower = target.basePower;

    GwentCard original = target;

    //To which player's graveyard/banished zone depends upon where it was consumed from.
    if (target.position[3] == 1){
        //In the case the the target is consumed from the hand / deck / board, it is sent to the graveyard.
        if (position[0] == 0 || position[0] == 2 || position[0] == 3){
            //NOTE: RECORD THE POSITION CORRECTLY AS DONE IN THE BANISH FUNCTION.
            board.playerOneGraveyard.push_back(target);
            removeCard(original, player, board);
        }
        //In the case it is consumed from grave, it is banished.
        else if (position[0] == 1){
            board.playerOneBanished.push_back(target);
            removeCard(original, player, board);
        }
    }
    else{
        //In the case the the target is consumed from the hand / deck / board, it is sent to the graveyard.
        if (position[0] == 0 || position[0] == 2 || position[0] == 3){
            board.playerTwoGraveyard.push_back(target);
            removeCard(original, player, board);
        }
        //In the case it is consumed from grave, it is banished.
        else if (position[0] == 1){
            board.playerTwoBanished.push_back(target);
            removeCard(original, player, board);
        }
    }
}

//Takes the target and damage amount as an input.  We pass the target by reference.  If the target dies, we move it to graveyard / banish it.
//We also pass the player as an argument as some cards can damage cards in hand (such as Nilfguard revealing the highest card in one's hand and setting it to one strength).
void damage(GwentCard &target, int amount, GwentBoard &board, GwentPlayer &player){
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
        destroy(target, player, board);
    }
    return;
}

//Demote the card to silver if gold.  If bronze stays bronze.
void demote(GwentCard &target){
    if (target.rank == 'g')
        target.rank = 's';
    else if (target.rank == 's')
        target.rank = 'b';
}

//Destroy the target.
void destroy(GwentCard &target, GwentPlayer &player, GwentBoard &board){
    //This is similiar to the banish mechanic, but can only go from field to grave.
    std::vector<size_t> position = target.position;

    //NOTE: HANDLE POSITION UPDATING PROPERLY AS DONE WITH GWENTCARD ORIGINAL IN BANISH FUNCTION.

    //When a card is banished / destroyed / discard, its power is reset.
    target.currentPower = target.basePower;

    //To which player's banished zone depends upon where it was banished.
    if (target.position[3] == 1){
        board.playerOneGraveyard.push_back(target);
    }
    else{
        board.playerTwoGraveyard.push_back(target);
    }
    //We now remove the card from where it was originally.
    removeCard(target, player, board);
}

//Discard the card to graveyard from hand.  Takes the index in hand and uses that information to send to the graveyard.
//Takes the player as a passed by reference input such that we can remove and send the card to the graveyard.
//We could store the card position in the card variable.
//Thus, functions such as this require us to pass the position.
void discard(GwentCard &target, GwentPlayer &player, GwentBoard &board){
    //Similiar to banish, but can only go from hand / deck to grave.
    std::vector<size_t> position = target.position;

    //NOTE: HANDLE POSITION UPDATING PROPERLY AS DONE WITH GWENTCARD ORIGINAL IN BANISH FUNCTION.

    //When a card is banished / destroyed / discard, its power is reset.
    target.currentPower = target.basePower;

    //To which player's banished zone depends upon where it was banished.
    if (target.position[3] == 1){
        board.playerOneGraveyard.push_back(target);
    }
    else{
        board.playerTwoGraveyard.push_back(target);
    }
    //We now remove the card from where it was originally.
    removeCard(target, player, board);
}

//While draw is a member function of GwentPlayer, it is also a mechanic from cards such as spies.
void draw(GwentPlayer &player){
    player.draw();
}

//Heal the target.  Can take just the card as input as will not change the board.
void heal(GwentCard &target){
    if (target.isWounded())
        target.currentPower = target.basePower;
}

//Returns a vector of pointers to the highestCards on the row.
std::vector<GwentCard*> highestOnRow(std::vector<GwentCard> row){
    GwentCard temp("Temp", 'm', 0, 0);
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
std::vector<GwentCard*> highest(GwentBoard board){
    GwentCard temp("Temp", 'm', 0, 0);
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
void lock(GwentCard &target){
    target.locked = !target.locked;
}

//Find the lowest card(s) on the row and returns pointers to them.
std::vector<GwentCard*> lowestOnRow(std::vector<GwentCard> row){
    GwentCard temp("Temp", 'm', 99999, 99999);
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
std::vector<GwentCard*> lowest(GwentBoard board){
    GwentCard temp("Temp", 'm', 0, 0);
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
void mulligan(const size_t index, GwentPlayer &player){
    //Remember to implement blacklist vector (likely member of player).
    //TODO: NOTE: will require a blacklist vector of type GwentCard to be temp held in the player.  Can clear after mulligan phase ends in each round.
    return;
}

//Promote: convert the target to gold until the end of the game.
void promote(GwentCard &target){
    target.rank = 'g';
}

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
        return;
    }
    //Account for the event that we must remove from the graveyard.
    else if (position[0] == 1){
        //It can be either player's graveyard.
        if (position[3] == 1){
            board.playerOneGraveyard.erase(board.playerOneGraveyard.begin()+position[2]);
        }
        else
            board.playerTwoGraveyard.erase(board.playerTwoGraveyard.begin()+position[2]);
        return;
    }
    //We account for if we are removing from the deck.
    else if (position[0] == 3){
        //It can be either player's deck.
        //We should be returned the correct player.
        player.deck.erase(player.deck.begin()+position[2]);
        return;
    }
    //The last possibility is it is in the player's hand that we are removing from.
    else{
        player.hand.erase(player.hand.begin()+position[2]);
        return;
    }
    qDebug() << "End of function reached: removeCard";
    return;
}

//Reset the unit to its base power.
void reset(GwentCard &target){
    target.currentPower = target.basePower;
}

//Resurrect the target from the graveyard.
void resurrect(GwentCard &target, GwentPlayer &player, GwentBoard &board, std::vector<size_t> position){
    //Spawn a copy and remove the original from the grave.
    GwentCard copy = target;
    spawn(copy, position, board);
    //We now remove the original from the grave.
    removeCard(target, player, board);
}

//Reveal the target.
void reveal(GwentCard &target){
    target.revealed = true;
}

//Spawn the target at the position.  Adds the card to the game and plays it.  Note that not all of position is needed (it can only be on the board, so need side, row number, and index, but we take the normal size for consistency.
void spawn(GwentCard &target, std::vector<size_t> position, GwentBoard &board){
    //Note that position is passed in the form of a 3d size_t var.
    //First number is board / graveyard / hand / deck respectively, 0 1 2 3, 4 for banished.
    //Second number is row number (only relevant for board).  Melee, ranged, siege is 0 1 2.
    //Third number is index in the vector / deque.
    //Fourth number is which player's side it is on, same as gwentPlayer.id (1 or 2).
    if (position[0] != 0){
        qDebug() << "Attempting to spawn unit at non board area, unit is: ";
        qDebug() << target.name;
        return;
    }
    //Update the position of the target.
    target.position = position;

    //Figure out which board and side it is to be spawned on.
    if (position[3] == 1){
        //Spawns on player 1's side.
        switch(position[1]){
            case 0:
                //Melee.
                board.playerOneMelee.insert(board.playerOneMelee.begin()+position[2], target);
                break;
            case 1:
                //Ranged.
                board.playerOneRanged.insert(board.playerOneRanged.begin()+position[2], target);
                break;
            case 2:
                //Siege.
                board.playerOneSiege.insert(board.playerOneSiege.begin()+position[2], target);
                break;
            default:
                qDebug() << "Attempting to spawn on in correct row";
                return;
                break;
        }
    }
    else{
        //Spawns on player 2's side.
        switch(position[1]){
            case 0:
                //Melee.
                board.playerTwoMelee.insert(board.playerTwoMelee.begin()+position[2], target);
                break;
            case 1:
                //Ranged.
                board.playerTwoRanged.insert(board.playerTwoRanged.begin()+position[2], target);
                break;
            case 2:
                //Siege.
                board.playerTwoSiege.insert(board.playerTwoSiege.begin()+position[2], target);
                break;
            default:
                qDebug() << "Attempting to spawn on in correct row";
                return;
                break;
        }
    }
}

//Summon the card from the deck.  Similair to spawn, but will remove the copy from the player's deck.
void summon(GwentCard &target, std::vector<size_t> position, GwentPlayer &player, GwentBoard &board){
    //We can code this is spawning a copy and removing from the deck using the remove function.
    //The reason we make a copy is that in the spawn function the position member of the target card will be modified (as it should be).  We need this position for the remove function too.
    GwentCard copy = target;
    spawn(copy, position, board);
    //We now remove the card from the deck.
    removeCard(target, player, board);
}

//Transform the card into a different card.
void transform(GwentCard &target, const GwentCard transformedTo){
    std::vector<size_t> position = target.position;
    target = transformedTo;
    target.position = position;
}

//Strengthen the target for the amount.  The board will not change with this effect, so we don't need to pass it.
void strengthen(GwentCard &target, const int amount){
    target.basePower += amount;
}

//Takes the target and weaken amount as an input.  We pass the target by reference.  If the target dies, we move it to graveyard / banish it.
void weaken(GwentCard &target, GwentPlayer &player, const int amount, GwentBoard &board){
    //Handle the case that the target does not die.
    if (target.currentPower - amount > 0 && target.basePower - amount > 0){
        target.currentPower -= amount;
        target.basePower -= amount;
        return;
    }
    //Next we handle the case where the target dies but still has base str / is not banished.
    else if (target.currentPower - amount <= 0 && target.basePower - amount > 0){
        destroy(target, player, board);
        return;
    }
    //Otherwise the base str drops below 0 and it is banished.
    else{
        banish(target, player, board);
        return;
    }
}



