#ifndef GWENTAI_H
#define GWENTAI_H

#include "gwentGame.h"
#include "gwentPlayer.h"

class GwentAI
{
public:
    GwentAI();
    ~GwentAI();
    GwentGame initGame(GwentPlayer p1, GwentPlayer p2) {

    }

private:
    GwentGame* gg;
};

#endif // GWENTAI_H
