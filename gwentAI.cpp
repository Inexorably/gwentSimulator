#include "gwentAI.h"

GwentAI::GwentAI(){

}

GwentAI::initGame(GwentPlayer p1, GwentPlayer p2){
    //Note: probably going to need to overload game constructor, since AI wont always know p2 but for now this is fine.
    gg = new GwentGame(p1, p2);
}

GwentAI::~GwentAI()
{
    delete gg;
}
