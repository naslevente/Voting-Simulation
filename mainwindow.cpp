#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtCore>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    isStop = false;
}

MainWindow::~MainWindow()
{
    delete ui;

    // delete simulation pointers
    delete igenNemOne;
    delete igenNemTwo;
    delete schulzeOne;
    delete schulzeTwo;
}

void MainWindow::on_start_button_clicked()
{

    QTextEdit* candidatesInput = MainWindow::centralWidget()->findChild<QTextEdit*>("candidates_input");
    QTextEdit* votersInput = MainWindow::centralWidget()->findChild<QTextEdit*>("voters_input");

    int numCandidates = candidatesInput->toPlainText().toInt();
    int numVoters = votersInput->toPlainText().toInt();

    // create the simulations
    igenNemOne = new Simulation(numVoters, numCandidates);
    igenNemTwo = new Simulation(numVoters, numCandidates);
    schulzeOne = new Simulation(numVoters, numCandidates);
    schulzeTwo = new Simulation(numVoters, numCandidates);

    // !! generate random batch of ballots for each simulation !!

    // isOne boolean field will distinguish a simulation from its counterpart (igen-nem elso eljaras v.s. igen-nem masodik eljaras
    // isSchulze boolean field will distinguish between schulze simulation and igen-nem simulation (these distinctions are used
    // when generating random ballots

    igenNemOne->GenerateRandomBallots();
    igenNemOne->igenNemCount = std::vector<int>(numCandidates);
    igenNemOne->isOne = true;
    igenNemOne->isSchulze = false;

    igenNemTwo->GenerateRandomBallots();
    igenNemTwo->igenNemCount = std::vector<int>(numCandidates);
    igenNemTwo->isOne = false;
    igenNemTwo->isSchulze = false;

    schulzeOne->GenerateRandomBallots();
    schulzeOne->isOne = true;
    schulzeOne->isSchulze = true;

    schulzeTwo->GenerateRandomBallots();
    schulzeTwo->isOne = false;
    schulzeTwo->isSchulze = true;

    while(true) {

        // begin simulations until stop button is pressed
        igenNemOne->StartIgenNemSimulation();
        igenNemTwo->StartIgenNemSimulation();

        schulzeOne->StartSchulzeSimulation();
        schulzeTwo->StartSchulzeSimulation();

        // if stop button is pressed, the while loop breaks
        QApplication::processEvents();
        if(isStop) {

            break;
        }
    }
}

void MainWindow::on_stop_button_clicked()
{

    this->isStop = true;
}
