#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gwentUtilities.h"
#include <map>
#include <deque>
#include <QString>
#include <gwentCard.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);

    std::map<QString,GwentCard> cardLibrary = loadCards();
    createDeck("D:/GwentSimulator/gwentSimulator/deck_1.csv", &cardLibrary);
    lookUpCard("Adrenaline Rush", &cardLibrary);
}

MainWindow::~MainWindow(){
    delete ui;
}
