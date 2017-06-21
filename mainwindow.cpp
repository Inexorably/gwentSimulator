
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gwentUtilities.h"
#include "globals.h"
#include <map>
#include <deque>
#include <QString>
#include <gwentCard.h>
#include "gwentAI.h"

#include "gwentGame.h"
#include <QDebug>

/********************************************/
//This is a global for debugging.
//Not here currently.
/********************************************/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
}

MainWindow::~MainWindow(){
    delete ui;
}

//Called in display game, writes the cards to a text browser, good for hand and stuff.  Does not write total power.
void MainWindow::displayRow(std::vector<GwentCard> cards, QTextBrowser &text){
    QString temp;
    for (size_t i = 0; i < cards.size(); i++){
        temp += cards[i].name;
        if (i+1 != cards.size())
            temp += ", ";

    }
    text.setText(temp);
}

//This overload provides label support and should be used for writing to rows where sum power should be displayed.
void MainWindow::displayRow(std::vector<GwentCard> cards, QTextBrowser &text, QLabel &label){
    int sumPower = 0;
    QString temp;
    for (size_t i = 0; i < cards.size(); i++){
        temp += "[" + cards[i].name + " " + QString::number(cards[i].currentPower) + "/" + QString::number(cards[i].basePower);
        if (cards[i].currentArmor != 0){
            temp += " " + QString::number(cards[i].currentArmor) + " Armor";
        }
        if (!cards[i].tags.empty()){
            for (size_t j = 0; j < cards[i].tags.size(); j++){
                temp += " " + cards[i].tags[j];
                if (j + 1 != cards[i].tags.size()){
                    temp += ",";
                }
            }
        }
        temp += "]";
        if (i+1 != cards.size())
            temp += ", ";
        sumPower += cards[i].currentPower;
    }
    text.setText(temp);
    label.setText(QString::number(sumPower));
}

//Displays the game state to our skilly ui.  Calling it game would conflict with global GwentGame test variable.
void MainWindow::displayGame(const GwentGame gameToDisplay){
    displayRow(gameToDisplay.playerOne.hand, *(ui->p1hand));
    displayRow(gameToDisplay.playerTwo.hand, *(ui->p2hand));
    displayRow(gameToDisplay.board.playerOneMelee, *(ui->p1melee), *(ui->p1meleePoints));
    displayRow(gameToDisplay.board.playerOneRanged, *(ui->p1ranged), *(ui->p1rangedPoints));
    displayRow(gameToDisplay.board.playerOneSiege, *(ui->p1siege), *(ui->p1siegePoints));
    displayRow(gameToDisplay.board.playerTwoMelee, *(ui->p2melee), *(ui->p2meleePoints));
    displayRow(gameToDisplay.board.playerTwoRanged, *(ui->p2ranged), *(ui->p2rangedPoints));
    displayRow(gameToDisplay.board.playerTwoSiege, *(ui->p2siege), *(ui->p2siegePoints));
}

//In this button, we will create a game state and display it with the display function such that
void MainWindow::on_pushButton_clicked(){
    //Create a test game.
    GwentGame game;
    //first thing we will need to do is load the card library
    std::map<QString,GwentCard> cardLibrary = loadCards();

    //Give some players temporary decks.
    //You've never dueled like this before, Ben Brode Jr.
    //Give some players temporary decks.
    game.playerOne.deck = createDeck(DECK_PATH, &cardLibrary);
    game.playerTwo.deck = createDeck(DECK_PATH, &cardLibrary);
    ui->debugBrowser->append("Test players initialized.");

    //The players have now been created.  We now cycle through the deck / hand post mulligan (not implemented) and assign the positions to the cards.  The turn player is also decided randomly.
    game.startGame();
    ui->debugBrowser->append("Game initalized.");

    //The game is set up correctly now.  Now we should play the game.  For debugging purposes, this is just using the different functions and making sure they display properly.
    //TEST STUFF GOES HERE NORMALLY.
    std::vector<size_t> testPosition;
    testPosition.push_back(0);
    testPosition.push_back(1);
    testPosition.push_back(0);
    testPosition.push_back(1);
    play(game.playerOne.hand[0], game.playerOne, game.board, testPosition);
    boost(game.board.playerOneRanged[0], 10);
    addTag(game.board.playerOneRanged[0], "Memes");
    addArmor(game.board.playerOneRanged[0], 10);
    //banish(game.board.playerOneRanged[0], game.playerOne, game.board);
    for (int i = 0; i < 4; i++){
        qDebug() << QString::number(game.board.playerOneGraveyard.size()) << " ";
    }
    //Display to the ui.
    displayGame(game);
    ui->debugBrowser->append("Execution complete.\n");
}
/* Old button - redoing interface, while still text based should be easier to understand.
void MainWindow::on_debugPushButton_clicked(){
    GwentCard bair("Savage Bair", "Skellige", 6, "m", "Gold", "L", 0);
    for (int i = 0; i < 13; i++){
        game.playerOne.deck.push_back(bair);
        game.playerTwo.deck.push_back(bair);
    }
    for (int i = 0; i < 12; i++){
        game.playerOne.hand.push_back(bair);
        game.playerTwo.hand.push_back(bair);
    }

    ui->debugBrowser->append("Debug players initialized.\n");
}

void MainWindow::on_pushButton_clicked(){

    if (game.turnPlayerId == 1 && !game.playerOne.hand.empty()){
        game.board.playerOneMelee.push_back(game.playerOne.hand.back());
        game.playerOne.hand.pop_back();
        game.turnPlayerId = 2;
    }
    else if (!game.playerTwo.hand.empty()){
        game.board.playerTwoMelee.push_back(game.playerTwo.hand.back());
        game.playerTwo.hand.pop_back();
        game.turnPlayerId = 1;
    }

    //Display the board and hands.
    ui->debugBrowser->append("Player One Hand: \n");
    for (int i = 0; i < game.playerOne.hand.size(); i++){
        QString temp = game.playerOne.hand[i].name + ", ";
        ui->debugBrowser->append(temp);
    }
    ui->debugBrowser->append("Player Two Hand: \n");
    for (int i = 0; i < game.playerTwo.hand.size(); i++){
        QString temp = game.playerTwo.hand[i].name + ", ";
        ui->debugBrowser->append(temp);
    }
    ui->debugBrowser->append("Board: \n");
    ui->debugBrowser->append("Player One Melee: \n");
    std::vector<GwentCard> currentRow = game.board.playerOneMelee;
    for (int i = 0; i < currentRow.size(); i++){
        QString temp = currentRow[i].name + ", ";
        ui->debugBrowser->append(temp);
    }
    ui->debugBrowser->append("Player One Ranged: \n");
    currentRow = game.board.playerOneRanged;
    for (int i = 0; i < currentRow.size(); i++){
        QString temp = currentRow[i].name + ", ";
        ui->debugBrowser->append(temp);
    }
    ui->debugBrowser->append("Player One Siege: \n");
    currentRow = game.board.playerOneSiege;
    for (int i = 0; i < currentRow.size(); i++){
        QString temp = currentRow[i].name + ", ";
        ui->debugBrowser->append(temp);
    }
    ui->debugBrowser->append("Player Two Melee: \n");
    currentRow = game.board.playerTwoMelee;
    for (int i = 0; i < currentRow.size(); i++){
        QString temp = currentRow[i].name + ", ";
        ui->debugBrowser->append(temp);
    }
    ui->debugBrowser->append("Player Two Ranged: \n");
    currentRow = game.board.playerTwoRanged;
    for (int i = 0; i < currentRow.size(); i++){
        QString temp = currentRow[i].name + ", ";
        ui->debugBrowser->append(temp);
    }
    ui->debugBrowser->append("Player Two Siege: \n");
    currentRow = game.board.playerTwoSiege;
    for (int i = 0; i < currentRow.size(); i++){
        QString temp = currentRow[i].name + ", ";
        ui->debugBrowser->append(temp);
    }

    QString temp = "Turn " + QString::number(game.turnNumber) + " ended.\n";
    ui->debugBrowser->append(temp);
    game.turnNumber++;
}
*/


//Here.
void MainWindow::on_debugButton_clicked(){
    //END GOAL:
    //Initialize a gwent AI, and play the game out

    //Create a test game.
    GwentGame game;
    //first thing we will need to do is load the card library
    std::map<QString,GwentCard> cardLibrary = loadCards();
    //Give some players temporary decks.
    //You've never dueled like this before, Ben Brode Jr.
    //Give some players temporary decks.
    game.playerOne.deck = createDeck(DECK_PATH, &cardLibrary);
    game.playerTwo.deck = createDeck(DECK_PATH, &cardLibrary);

    GwentAI gwentAI(game);
    gwentAI.gg.startGame();

    //SIMULATE INPUT FROM AHK OR SOME PROGRAM
    //we lost coin flip. time to make a play
    gwentAI.processPlay();
    //we played a bear. Now its the opponents turn. This will usually update
    //based on ahk observations, but lets do it manually
    gwentAI.gg.changeTurnPlayer();
    gwentAI.gg.setPlayerPassed(2);
    gwentAI.gg.changeTurnPlayer();
    //player 2 was scared of the bear. lets pass
    gwentAI.processPlay();

    //Define turns:
    //1. P1 plays savage bear
    //2. P2 Plays a savage bear
    //3. P1 ends
    //4. P2 ends

    //gwentAI.processPlay();

}
