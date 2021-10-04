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

    // get the start button associated with this mainwindow
    startButton = MainWindow::centralWidget()->findChild<QPushButton*>("start_button");
    finishButton = MainWindow::centralWidget()->findChild<QPushButton*>("finish_button");
    candidatesInput = MainWindow::centralWidget()->findChild<QTextEdit*>("candidates_input");
    votersInput = MainWindow::centralWidget()->findChild<QTextEdit*>("voters_input");
    iterationLabel = MainWindow::centralWidget()->findChild<QLabel*>("iteration_label");
    tableWidget = MainWindow::centralWidget()->findChild<QTableWidget*>("tableWidget");

    // set initial fields
    numberOfIterations = 0;
    isConvereged = false;

    // make call to SetupApplication()
    SetupApplication();

    /*
    // !! necessary only for qthread implementation

    // initialize the main thread and set the SIGNAL and SLOT parameters
    mainThread = new QThread();

    // cleanup of mainthread after prob numbers begin to converge
    connect(this, SIGNAL(CleanupMainThread()), this->mainThread, SLOT(quit()));
    connect(this, SIGNAL(CleanupMainThread()), this->mainThread, SLOT(deleteLater()));

    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(StartProcess()));
    connect(finishButton, SIGNAL(clicked(bool)), this->mainThread, SLOT(deleteLater())); // must click finish to release main thread from memory
    */

    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(StartSimulationIterations()));
}

MainWindow::~MainWindow() {
    delete ui;

    // delete simulation worker
    delete worker;

    // delete ui elements
    delete startButton;
    delete candidatesInput;
    delete votersInput;
    delete iterationLabel;
    delete tableWidget;
}

void MainWindow::SetupApplication() {

    // setup table widget
    for(int i = 0; i < 4; ++i) {

        for(int j = 0; j < 3; ++j) {

            QTableWidgetItem *item = tableWidget->item(i, j);
            if(!item) {

                item = new QTableWidgetItem();
                tableWidget->setItem(i, j, item);
            }
            item->setText("0");
        }
    }
}

void MainWindow::StartSimulationIterations() {

    // set the iteration label
    iterationLabel->setText("Iteration counter: 0");

    // increment iteration counte
    ++numberOfIterations;

    // create a new iteration of simulations
    StartProcess();
}

void MainWindow::ContinueSimulationIterations() {

    std::cout << "value of isConverged: " << isConvereged << std::endl;

    if(!isConvereged) {

        // increment iteration counter
        ++numberOfIterations;

        std::cout << "Start process being called" << std::endl;

        // create new main thread
        //this->mainThread = new QThread();

        // start next simulation iteration
        StartProcess();
    }
}

void MainWindow::on_stop_button_clicked() {

    UpdateProbabilityTable();
}

void MainWindow::UpdateProbabilityTable() {

    // end the while loop that was creating iteraions of each simulation
    worker->isRunning = false;

    // get probabilities and update table
    std::vector<std::vector<double>> probabilities = worker->getProbabilities();
    UpdateTableWidget(probabilities);
}

void MainWindow::on_finish_button_clicked() {

    std::cout << "ended the simulations" << std::endl;
    this->close();
}

void MainWindow::UpdateTableWidget(std::vector<std::vector<double>> probabilities) {

    // loop through each table widget item and update each cell
    for(int i = 0; i < 4; ++i) {

        for(int j = 0; j < 3; ++j) {

            QString currentValue = tableWidget->item(i, j)->text();
            double newValue = double(((currentValue.split(" ")[0].toDouble() * double(numberOfIterations - 1)) +
                    probabilities.at(i).at(j)) / numberOfIterations);

            // update isConverged bool value
            //if(newValue == currentValue.split(" ")[0].toDouble() && numberOfIterations != 0) {
            if(numberOfIterations == 10000) {

                isConvereged = true;
            }

            // update the table widget
            tableWidget->item(i, j)->setText(QString::number(newValue) + " %");
            //tableWidget->item(i, j)->setText(QString::number(probabilities.at(i).at(j)) + " %");
        }
    }
}

/*
// !! thread implementation
void MainWindow::StartProcess() {

    // extract voter and candidate input from text edits
    this->numCandidates = candidatesInput->toPlainText().toInt();
    this->numVoters = votersInput->toPlainText().toInt();

    // create simulation worker
    worker = new SimulationWorker(this->numVoters, this->numCandidates);
    worker->moveToThread(this->mainThread);

    // apply the necessary connections for functionality and cleanup

    // !! start, process, and continue the work
    connect(this->mainThread, SIGNAL(started()), worker, SLOT(Simulate()));
    connect(worker, SIGNAL(iterationFinished(int)), this, SLOT(UpdateIterationLabel(int)));
    connect(worker, SIGNAL(finished()), this, SLOT(UpdateProbabilityTable()));
    //connect(worker, SIGNAL(finished()), this, SLOT(ContinueSimulationIterations()));

    // !! cleanup
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(worker, SIGNAL(finished()), this->mainThread, SLOT(quit()));
    connect(this->mainThread, SIGNAL(finished()), this, SLOT(ContinueSimulationIterations()));
    //connect(worker, SIGNAL(finished()), this->mainThread, SLOT(deleteLater()));

    std::cout << "starting main thread" << std::endl;

    // start thread
    this->mainThread->start();
}
*/

void MainWindow::StartProcess() {

    // extract voter and candidate input from text edits
    this->numCandidates = candidatesInput->toPlainText().toInt();
    this->numVoters = votersInput->toPlainText().toInt();

    // create simulation worker
    worker = new SimulationWorker(this->numVoters, this->numCandidates);

    // create necessary connections
    connect(worker, SIGNAL(iterationFinished(int)), this, SLOT(UpdateIterationLabel(int)));
    connect(worker, SIGNAL(finished()), this, SLOT(UpdateProbabilityTable()));
    //connect(worker, SIGNAL(finished()), this, SLOT(ContinueSimulationIterations()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(worker, SIGNAL(destroyed(QObject*)), this, SLOT(ContinueSimulationIterations()));
    connect(this, SIGNAL(BeginSimulation()), worker, SLOT(Simulate()));

    emit BeginSimulation();
}

// update iteration label each time iteration is complete and thread sends iterationFinished signal
void MainWindow::UpdateIterationLabel(int numIterations) {

    //++numberOfIterations;

    // convert int to qstring to update iteration label
    std::cout << "updating iteration label: " << numberOfIterations << std::endl;
    QString iterations;
    iterations.setNum((numberOfIterations));
    //iterations.setNum(numIterations);
    //this->iterationLabel->setText("Iteration counter: " + iterations);
    this->iterationLabel->setText("Iteration counter: " + numberOfIterations);
}
