#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "gwentGame.h"

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

    private slots:
    /*
        void on_debugPushButton_clicked();

        void on_pushButton_clicked();
    */
        void on_pushButton_clicked();

    private:
        Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
