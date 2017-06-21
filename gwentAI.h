#ifndef GWENTAI_H
#define GWENTAI_H

#include "gwentGame.h"
#include "gwentPlayer.h"
#include "gwentCard.h"

// GwentAI
// Description: GwentAI's main member is a GwentGame object gg. The game instance inside of AI however
//              will only be knowledgeable of one player's hand and deck, so P2's hand/deck will generally
//              be empty (until they play a card). The game object is still going to be the same as the regular
//              class, but a lot of AI functions will be wrappers for underlying gwentMechanics functions.

class GwentAI
{
public:
    GwentAI(GwentGame game);

    //the main decision entry point
    void processPlay();

    //helpers of process play
    GwentCard* chooseCard(int minVal);
    int evaluatePlayValue(GwentCard card);

    //processplay will call this when it decides.
    void makeDecision();

private:
    GwentGame gg;
};

#endif // GWENTAI_H
