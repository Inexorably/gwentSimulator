#include "gwentAI.h"

GwentAI::GwentAI(GwentGame game, std::map<QString,GwentCard>* cardLib){
    gg = game;
    cardLibrary = cardLib;
}

// processPlay()
// Description: the main "brains" of our AI. should examine the board state, and determine what play to make.
// Relies on the following functions:
//      GwentCard* GwentAI::chooseCard(int minVal, ourId);
//      int GwentAI::evaluatePlayValue(GwentCard card);
//      void GwentAI::makeDecision();

void GwentAI::processPlay(){
    //Get player turn
    int ourId = gg.getTurnPlayerId(); // 1 - P1 turn
                                          // 2 - P2 turn

    int opponentId;
    bool opponentPassed;

    if (ourId == 1){
        opponentId = 2;
        opponentPassed = gg.playerTwoPassed;
    }
    else if (ourId == 2){
        opponentId = 1;
        opponentPassed = gg.playerOnePassed;
    }
    else {
        //Theres a problem.
        return;
    }

    //Look at point totals, and determine the best play.
    int ourPoints = gg.getPlayerPointTotal(ourId);
    int opponentPoints = gg.getPlayerPointTotal(opponentId);
    int differential = ourPoints - opponentPoints;

    //Retard AI decisions:
    // Other player passed
    if (opponentPassed){
        //1. We are behind or tied
        if (differential <= 0){
            // Play a card that will put us in the lead, but not by too much, or play the maximum value card
            GwentCard* cardToPlay = chooseCard(differential+1, ourId);
            makeDecision(cardToPlay, ourId);
        }
        //2. We are ahead
        //   - Pass
        else {
            makeDecision(nullptr, ourId);
        }
    }
    // Other player did not pass
    else {
        //1. We are behind or tied
        //   - Play a card that will put us in the lead, but not by too much, or play the maximum value card
        if (differential <= 0){
            GwentCard* cardToPlay = chooseCard(differential+1, ourId);
            makeDecision(cardToPlay, ourId);
        }
        //2. We are ahead by 25 points
        //   - Pass
        else if (differential >= 25){
             makeDecision(nullptr, ourId);
        }
        //3. We are ahead
        //   - Play min value play
        else { //(differential > 0 && differential < 25)
            GwentCard* cardToPlay = chooseCard(1, ourId);
            makeDecision(cardToPlay, ourId);
        }
    }
}

GwentCard* GwentAI::chooseCard(int minVal, int ourId){
    GwentCard* cardChoice = nullptr;
    GwentCard* maxValueCard = nullptr;
    int cardChoiceVal = 99999; //start this arbitrarily high
    int maxVal = 0;

    //Get a pointer to our hand
    std::vector<GwentCard>* ourHand;
    if (ourId == 1){
        ourHand = &gg.playerOne.hand;
    }
    else{
        ourHand = &gg.playerTwo.hand;
    }

    int handSize = ourHand->size();
    //Search hand for the optimal play
    for (int i = 0; i < handSize; i++){
        //evaluate card's value
        int cardValue = evaluatePlayValue(ourHand->at(i));

        //track the maximum value card
        if (cardValue > maxVal){
            maxValueCard = &ourHand->at(i);
            maxVal = cardValue;
        }

        if (cardValue >= minVal && cardValue < cardChoiceVal){
            cardChoice = &ourHand->at(i);
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
//TODO: makeDecision should also take card position as arguments. Right now the decision function does not determine where to place.
void GwentAI::makeDecision(GwentCard* card, int side){

    if (card == nullptr){
        GwentCard passCard;
        gg.executeTurn(true, passCard, -1, -1, -1);
        return; //TODO: call the pass function
    }

    //call the play function on card
    GwentCard cardToPlay = *card;
    //TODO: change position arguments
    gg.executeTurn(false, cardToPlay, 0, 0, side);
    return;
}
