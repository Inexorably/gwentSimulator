#include "gwentAI.h"

GwentAI::GwentAI(GwentGame game){
    gg = game;
}

GwentAI::GwentAI(){
    return;
}

// processPlay()
// Description: the main "brains" of our AI. should examine the board state, and determine what play to make.
// Relies on the following functions:
//      GwentCard* GwentAI::chooseCard(int minVal);
//      int GwentAI::evaluatePlayValue(GwentCard card);
//      void GwentAI::makeDecision();

void GwentAI::processPlay(){
    //Get player turn
    bool didWePass = gg.playerOnePassed;
    int whoseTurn = gg.getTurnPlayerId(); // 1 - P1 turn
                                          // 2 - P2 turn

    if (whoseTurn == 2 || didWePass){
        return;     //its not our turn/we passed already, why did you even call this function?
    }

    //Now we are sure it is our turn. Look at point totals, and determine the best play.
    int myPoints = gg.getPlayerPointTotal(1);
    int opponentPoints = gg.getPlayerPointTotal(2);
    int differential = myPoints - opponentPoints;

    //Retard AI decisions:
    // Other player passed
    if (gg.playerTwoPassed){
        //1. We are behind or tied
        if (differential <= 0){
            // Play a card that will put us in the lead, but not by too much, or play the maximum value card
            GwentCard* cardToPlay = chooseCard(differential+1);
            makeDecision(cardToPlay);
        }
        //2. We are ahead
        //   - Pass
        else {
            makeDecision(nullptr);
        }
    }
    // Other player did not pass
    else {
        //1. We are behind or tied
        //   - Play a card that will put us in the lead, but not by too much, or play the maximum value card
        if (differential <= 0){
            GwentCard* cardToPlay = chooseCard(differential+1);
            makeDecision(cardToPlay);
        }
        //2. We are ahead by 25 points
        //   - Pass
        else if (differential >= 25){
             makeDecision(nullptr);
        }
        //3. We are ahead
        //   - Play min value play
        else { //(differential > 0 && differential < 25)
            GwentCard* cardToPlay = chooseCard(1);
            makeDecision(cardToPlay);
        }
    }
}

GwentCard* GwentAI::chooseCard(int minVal){
    GwentCard* cardChoice = nullptr;
    GwentCard* maxValueCard = nullptr;
    int cardChoiceVal = 99999; //start this arbitrarily high
    int maxVal = 0;

    int handSize = gg.playerOne.hand.size();
    //Search hand for the optimal play
    for (int i = 0; i < handSize; i++){
        //evaluate card's value
        int cardValue = evaluatePlayValue(gg.playerOne.hand[i]);

        //track the maximum value card
        if (cardValue > maxVal){
            maxValueCard = &gg.playerOne.hand[i];
            maxVal = cardValue;
        }

        if (cardValue >= minVal && cardValue < cardChoiceVal){
            cardChoice = &gg.playerOne.hand[i];
            cardChoiceVal = cardValue;
        }
    }

    //check to see if we found a card over the minVal threshold. if not, return the max value card
    if (cardChoice == nullptr){
        return maxValueCard;
    }

    return cardChoice;
}

//TODO: this function should eventually take into account board hazards and card effects ie. witchers = 17pts if 2 in deck
int GwentAI::evaluatePlayValue(GwentCard card){
    return card.currentPower; //return currentPower LUL
}

// We can either pass, or play a card.
// Input: a pointer to a card we want to play. If its nullptr, we want to pass
void GwentAI::makeDecision(GwentCard* card){
    if (card == nullptr){
        return; //TODO: call the pass function
    }

    //TODO: call the play function on card
    return;
}
