#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "gwentGame.h"
#include "gwentCard.h"

#include <QTextBrowser>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

        //This allows us to display a game to our skilly window.
        //Calling it game might interfere with global test variable game.
        void displayGame(const GwentGame gameToDisplay);

        //Called in display game, writes the cards to a text browser, good for hand and stuff.  Does not write total power.
        void displayRow(std::vector<GwentCard> cards, QTextBrowser &text);
        //This overload provides label support and should be used for writing to rows where sum power should be displayed.
        void displayRow(std::vector<GwentCard> cards, QTextBrowser &text, QLabel &label);

    private slots:
    /*
        void on_debugPushButton_clicked();

        void on_pushButton_clicked();
    */
        void on_pushButton_clicked();

        void on_debugButton_clicked();

        void on_playTurn_clicked();

private:
        Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
