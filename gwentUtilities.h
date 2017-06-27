#include <QString>
#include <map>
#include <deque>
#include "gwentCard.h"
#include <fstream>
#include <sstream>
#include <string>
#include "globals.h"

std::map<QString,GwentCard> loadCards(){
    //hard code file name, retrive cards

    //Format: Priestess of Freya,Skellige,1,Siege,Bronze,L,0,Choose(1, Graveyard),Resurrect(Choice1)
    //parse the csv file
    QString name;
    QString faction;
    QString basePower;
    QString type;
    QString rank;
    QString loyalty;

    int cellNum = 0;
    std::ifstream file(CARD_LIBRARY_PATH, std::ios::binary);
    std::map<QString,GwentCard> cardLibrary;
    std::string line;
    std::vector<QString> effectsVec;

    //This gets the file
    while(std::getline(file, line))
    {
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        std::stringstream  lineStream(line);
        std::string        cell;
        while(std::getline(lineStream, cell, ',')){
            switch(cellNum){
                case 0:
                    name = QString::fromStdString(cell);
                    break;
                case 1:
                    faction = QString::fromStdString(cell);
                    break;
                case 2:
                    basePower = QString::fromStdString(cell);
                    break;
                case 3:
                    type = QString::fromStdString(cell);
                    break;
                case 4:
                    rank = QString::fromStdString(cell);
                    break;
                case 5:
                    loyalty = QString::fromStdString(cell);
                    break;
                default:
                    effectsVec.push_back(QString::fromStdString(cell));
                    break;
            }
            cellNum++;
        }

        //Make a card
        GwentCard gc(name, faction, basePower.toInt(), type, rank, loyalty, 0, effectsVec);
        cardLibrary[name] = gc;

        //Reset cell num for each iteration of the loop
        cellNum = 0;
    }
    return cardLibrary;
}

GwentCard lookUpCard(QString name, std::map<QString,GwentCard>* cardLibrary){
    return cardLibrary->find(name)->second;
}

std::deque<GwentCard> createDeck(std::string fileName, std::map<QString,GwentCard>* cardLibrary){
    QString name;
    int number;

    int cellNum = 0;
    std::ifstream file(fileName);
    std::deque<GwentCard> deck;
    std::string line;
    while(std::getline(file, line, '\n'))
    {
        std::stringstream  lineStream(line);
        std::string        cell;
        while(std::getline(lineStream, cell, ',')){
            switch(cellNum){
                case 0:
                    name = QString::fromStdString(cell);
                    break;
                case 1:
                    number = std::stoi(cell);
                    break;
                default:
                    break;
            }
            cellNum++;
        }

        //Make a card
        GwentCard gc = lookUpCard(name, cardLibrary);
        for (int i = 0; i < number; i++){
            deck.push_back(gc);
        }

        //Reset cell num for each iteration of the loop
        cellNum = 0;
    }
    return deck;
}

void playGame(){

}
