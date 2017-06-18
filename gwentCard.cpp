#include "gwentCard.h"

GwentCard::GwentCard(QString name, char type, int basePower, int baseArmor){
    this->name = name;
    this->type = type;
    this->basePower = basePower;
    this->baseArmor = baseArmor;
}


bool GwentCard::isWounded(){
    return (basePower > currentPower);
}

bool GwentCard::isBoosted(){
    return (basePower < currentPower);
}

void GwentCard::resetToBaseCopy(){
    return;
}
