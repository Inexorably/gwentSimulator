#include "gwentCard.h"

GwentCard::GwentCard(QString name, QString faction, int basePower, QString type, QString rank, QString loyalty, int baseArmor){
    this->name = name;
    this->faction = faction;
    this->basePower = basePower;
    this->currentPower = basePower;
    this->type = type;
    this->rank = rank;
    this->loyalty = loyalty;
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

