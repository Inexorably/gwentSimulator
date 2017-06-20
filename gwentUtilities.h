#include <QString>
#include <map>
#include <deque>
#include "gwentCard.h"
#include <fstream>
#include <sstream>
#include <string>

std::map<QString,GwentCard> loadCards(){
    //hard code file name, retrive cards
	
    //Format: Adrenaline Rush,Neutral,0,Event,Bronze,
    //parse the csv file
    QString name;
    QString faction;
    QString basePower;
    QString type;
    QString rank;
    QString loyalty;

    int cellNum = 0;
    std::ifstream file("D:/GwentSimulator/gwentSimulator/cards.csv");
    std::map<QString,GwentCard> cardLibrary;
    std::string line;
    while(std::getline(file, line, '\r'))
    {
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
                    break;
            }
            cellNum++;
        }

        //Make a card
        GwentCard gc(name, faction, basePower.toInt(), type, rank, loyalty, 0);
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
    while(std::getline(file, line, '\r'))
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
        for (int i = 0; i < number; i++){
            GwentCard gc = lookUpCard(name, cardLibrary);
            deck.push_back(gc);
        }

        //Reset cell num for each iteration of the loop
        cellNum = 0;
    }
    return deck;
}

void playGame(){

}

