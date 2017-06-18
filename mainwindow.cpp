#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gwentGame.h"

/********************************************/
//This is a global for debugging.
GwentGame game;
/********************************************/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
}

MainWindow::~MainWindow(){
    delete ui;
}

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

void MainWindow::on_debugButtonMatt_clicked(){
    //Debug shit can go here.
}
