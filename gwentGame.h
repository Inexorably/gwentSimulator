#ifndef GWENTGAME_H
#define GWENTGAME_H

#include "gwentPlayer.h"
#include "gwentBoard.h"

//For draw and stuff.
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <algorithm>    // std::shuffle

#include "gwentCard.h"

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

    //Members/Functions for passing turn
    void playerPass();
    bool playerOnePassed;
    bool playerTwoPassed;

    //This will occur at the beginning of the game, and draw cards + intialize things like position member for cards and such.
    void startGame();
    void setPlayerPassed(int playerId);

    //Called in startGame.  Will set the positions for all cards in the game correctly.  Can be expensive.
    void updateCardPositions();

    //Players have 2 options for advancing game, they can pass or they can play a card. nullptr = pass
    void executeTurn(bool passTurn, GwentCard &card, int row, int index, int side);

    //The round number (1, 2, 3).
    int roundNumber;

    //Ends the round.  Moves all cards to grave except for resilient.  Will trigger death effects, creates an event for round end.  If the game is not over, calls startRound.
    //Returns true if game complete, false if game is still going on
    bool endRound();
    bool gameComplete;

    //-------GwentMechanics-------\\

    //Adds a tag to the target card (such as resilient, regressing, etc).
    void addTag(GwentCard &target, const QString tag);

    //Adds armor to the target.
    void addArmor(GwentCard &target, const int amount);

    //Boost the target for the amount.  The board will not change with this effect, so we don't need to pass it.
    void boost(GwentCard &target, const int amount);

    //Banish the card at the position.  Takes the target and the player who the card is on the side of as inputs.  Note that the banished zone is a member of GwentBoard as it is public info.
    void banish(GwentCard &target);

    //Consume functions.  If the target is in the hand / field it is sent to grave, else if in grave it is banished.  Note: should activate a "consumed trigger", as other mechanics shall with their triggers.
    void consume(GwentCard &cardConsuming, GwentCard &target);

    //Takes the target and damage amount as an input.  We pass the target by reference.  If the target dies, we move it to graveyard / banish it.
    //We also pass the player as an argument as some cards can damage cards in hand (such as Nilfguard revealing the highest card in one's hand and setting it to one strength).
    void damage(GwentCard &target, int amount);

    //Demote the card to silver if gold.  If bronze stays bronze.
    void demote(GwentCard &target);

    //Destroy the target.
    void destroy(GwentCard &target);

    //Discard the card to graveyard from hand.  Takes the index in hand and uses that information to send to the graveyard.
    //Takes the player as a passed by reference input such that we can remove and send the card to the graveyard.
    //We could store the card position in the card variable.
    //Thus, functions such as this require us to pass the position.
    void discard(GwentCard &target);

    //While draw is a member function of GwentPlayer, it is also a mechanic from cards such as spies.
    void draw(GwentPlayer &player);

    //Heal the target.  Can take just the card as input as will not change the board.
    void heal(GwentCard &target);

    //Find the highest card(s) on the row and returns pointers to them.
    std::vector<GwentCard*> highestOnRow(std::vector<GwentCard> row);

    //Returns the highest card on the board.
    std::vector<GwentCard*> highest();

    //Locks the target, does not affect board.
    void lock(GwentCard &target);

    //Find the lowest card(s) on the row and returns pointers to them.
    std::vector<GwentCard*> lowestOnRow(std::vector<GwentCard> row);

    //Finds the lowest card(s) on the board.
    std::vector<GwentCard*> lowest();

    //Mulligan the card at the target position (in hand).
    void mulligan(const size_t index, GwentPlayer &player);

    //Play a card from hand.
    void play(GwentCard &target, int row, int index, int side);

    //Promote: convert the target to gold until the end of the round.
    void promote(GwentCard &target);

    //Remove a card (such as from the board / hand / deck) using its position member.
    void removeCard(GwentCard &target);

    //Reset the unit to its base power.
    void reset(GwentCard &target);

    //Resurrect the target from the graveyard.
    void resurrect(GwentCard &target, int row, int index, int side);

    //Reveal the target.
    void reveal(GwentCard &target);

    //Shuffle the player's deck.
    void shuffleDeck(GwentPlayer &player);

    //Spawn the target at the position.  Adds the card to the game and plays it.  Note that not all of position is needed (it can only be on the board, so need side, row number, and index, but we take the normal size for consistency.
    void spawn(GwentCard target, int row, int index, int side);

    //Summon the card from the deck.  Similair to spawn, but will remove the copy from the player's deck.
    void summon(GwentCard &target, int row, int index, int side);

    //Transform the card into a different card.
    void transform(GwentCard &target, const GwentCard transformedTo);

    //Strengthen the target for the amount.  The board will not change with this effect, so we don't need to pass it.
    void strengthen(GwentCard &target, const int amount);

    //Takes the target and weaken amount as an input.  We pass the target by reference.  If the target dies, we move it to graveyard / banish it.
    void weaken(GwentCard &target, const int amount);

};


//Initiating autism.

//Define global strings such as const char GOLD = 'g' here.  Tags also go here.
const QString TAG_RESILIENT = "Resilient";

//List of keywords that are tags and will not have an explicit function:
/*http://gwentify.com/guides/gwent-keyword-list-gwent-mechanics/
 * Ally, Ambush, Bond, Brave, Crewmen X, Deathwish, Deploy, Disloyal, Effort, Enemy, Fresh crew, Orders
 * Retaliation, Regressing, Shield (like Quen), Trio, Turn End, Turn Start, Veteran, Weaken
 *
 *
 *
 *
*/

int randomInt(int min, int max);


#endif // GWENTGAME_H
