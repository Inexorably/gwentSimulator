#include <QString>
#include <vector>
#include "gwentCard.h"
#include <fstream>
#include <sstream>

void loadCards(){
    //hard code file name, retrive cards

	
    //Format: Adrenaline Rush,Neutral,0,Event,Bronze,
    //parse the csv file
    std::vector<GwentCard> cardLibrary;
    //cardLibrary.push_back();

    QString name;
    QString faction;
    QString basePower;
    QString type;
    QString rank;
    QString loyalty;

    int cellNum = 0;
    std::ifstream file("D:/GwentSimulator/gwentSimulator/cards.csv");

    std::string line;
    while(std::getline(file,line))
    {
        std::stringstream  lineStream(line);
        std::string        cell;
        while(std::getline(lineStream,cell,',')){
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
            }
            cellNum++;
        }

        //Make a card
        GwentCard gc(name, faction, basePower.toInt(), type, rank, loyalty, 0);
        cardLibrary.push_back(gc);

        //Reset cell num for each iteration of the loop
        cellNum = 0;
    }
}


void createDeck(){

}

void playGame(){

}
