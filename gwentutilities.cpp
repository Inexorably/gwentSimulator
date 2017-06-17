#include "gwentGame.h"

#include <QString>

void loadCards(){
    //hard code file name, retrive cards

	
	//Format: "Adrenaline Rush",Teste,"8326"
    //parse the csv file
    std::vector<GwentCard>;
    cardLibrary.push_back(GwentCard(file[1], file[2], file[3]));

    ifstream file ( "cards.csv" ); 

	std::string name;
	std::string type;
	std::string basePower;
	while(file.good()){

	    getline(file, ID, ',');
	    cout << "ID: " << ID << " " ; 

	    getline(file, nome, ',') ;
	    cout << "User: " << nome << " " ;

	    getline(file, idade, ',') ;
	    cout << "Idade: " << idade << " "  ; 

	    getline(file, genero, ' ') ; 
	    cout << "Sexo: " <<  genero<< " "  ;

	}
	
}


void createDeck(){

}

void playGame(){

}
