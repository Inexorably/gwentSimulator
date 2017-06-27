#ifndef GWENTCARD_H
#define GWENTCARD_H

#include <vector>
#include <QString>


class GwentCard{
public:
    GwentCard();
    GwentCard(QString name, QString faction, int basePower, QString type, QString rank, QString loyalty, int baseArmor, std::vector<QString> effects);
    //Note that position is passed in the form of a 3d size_t var.
    //First number is board / graveyard / hand / deck respectively, 0 1 2 3, 4 for banished.
    //Second number is row number (only relevant for board).  Melee, ranged, siege is 0 1 2.
    //Third number is index in the vector / deque.
    //Fourth number is which player's side it is on, same as gwentPlayer.id (1 or 2).
    int zone;
    int row;
    int index;
    int side;

    //A card can have different tags assigned to it.  Some are inherent such as cursed and regressing, and others are added such as adreneline rush.
    //These can be stored in a vector because the amount of tags can and will vary on cards.
    //These tags can be stored as strings.
    std::vector<QString> tags;

    //These tags can be stored as strings.
    std::vector<QString> effects;

    //Basic variables for information of card.  Base power of -1 for spells.
    //Note that name is not constant because cards can be transformed (such as raging berserker).
    QString name;
    QString type;
    QString rank;
    QString loyalty;
    QString faction;

    int basePower;
    int baseArmor;

    //Current stats for gameplay
    int currentArmor;
    int currentPower;

    //Boolean for revealed.  If true it is revealed, if false not revealed.  Initializes to false.
    bool revealed;

    //True if spying, else false.  Initializes to false.  Changes to false when no longer on opponent's side.
    bool spying;
    bool locked;

    //The timer.  The first parameter is how many turns to tick the ratio is (such as 2 for vran).  The second is for not looping (Octivist, 0 or false), or looping (Vran, 1 or true).
    int timer[2];

    bool isWounded();
    bool isBoosted();

    //Resets a card to its base version using the string name.
    void resetToBaseCopy();
};

#endif // GWENTCARD_H
