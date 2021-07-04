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
    candidatesInput = MainWindow::centralWidget()->findChild<QTextEdit*>("candidates_input");
    votersInput = MainWindow::centralWidget()->findChild<QTextEdit*>("voters_input");
    iterationLabel = MainWindow::centralWidget()->findChild<QLabel*>("iteration_label");
    tableWidget = MainWindow::centralWidget()->findChild<QTableWidget*>("tableWidget");

    // make call to SetupApplication()
    SetupApplication();

    // initialize the main thread and set the SIGNAL and SLOT parameters
    mainThread = new QThread();
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(StartProcess()));
}

MainWindow::~MainWindow()
{
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

    // set the iteration label
    iterationLabel->setText("Iteration counter: 0");
}

/*
void MainWindow::UpdateProbabilityTable(int iterations) {

    // update fields for igen-nem 1 simulation
    tableWidget->item(0, 0)->setText(QString::number(((igenNemOne->numberOfManipulations / iterations) * 100)) + "%");
    tableWidget->item(0, 1)->setText(QString::number(((igenNemOne->numberOfSuccesses / iterations) * 100)) + "%");

    // update fields for igen-nem 2 simulation
    tableWidget->item(1, 0)->setText(QString::number(((igenNemTwo->numberOfManipulations / iterations) * 100)) + "%");
    tableWidget->item(1, 1)->setText(QString::number(((igenNemTwo->numberOfSuccesses / iterations) * 100)) + "%");

    // update fields for schulze 1 simulation
    tableWidget->item(2, 0)->setText(QString::number(((schulzeOne->numberOfManipulations / iterations) * 100)) + "%");
    tableWidget->item(2, 1)->setText(QString::number(((schulzeOne->numberOfSuccesses / iterations) * 100)) + "%");
    tableWidget->item(2, 2)->setText(QString::number(((schulzeOne->minoritySuccesses / iterations) * 100)) + "%");

    // update fields for schulze 2 simulation
    tableWidget->item(3, 0)->setText(QString::number(((schulzeTwo->numberOfManipulations / iterations) * 100)) + "%");
    tableWidget->item(3, 1)->setText(QString::number(((schulzeTwo->numberOfSuccesses / iterations) * 100)) + "%");
    tableWidget->item(3, 2)->setText(QString::number(((schulzeTwo->minoritySuccesses / iterations) * 100)) + "%");
}
*/

void MainWindow::on_stop_button_clicked()
{

    worker->isRunning = false;

    // reset mainThread and get ready for new simulation run
    mainThread = new QThread();
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(StartProcess()));
}

void MainWindow::StartProcess() {

    // set the iteration label
    iterationLabel->setText("Iteration counter: 0");

    // extract voter and candidate input from text edits
    this->numCandidates = candidatesInput->toPlainText().toInt();
    this->numVoters = votersInput->toPlainText().toInt();

    // create simulation worker
    worker = new SimulationWorker(this->numVoters, this->numCandidates);
    worker->moveToThread(this->mainThread);

    // apply the necessary connections for functionality and cleanup

    // !! start and process the work
    connect(mainThread, SIGNAL(started()), worker, SLOT(Simulate()));
    connect(worker, SIGNAL(iterationFinished(int)), this, SLOT(UpdateIterationLabel(int)));

    // !! cleanup
    connect(worker, SIGNAL(finished()), this->mainThread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(mainThread, SIGNAL(finished()), mainThread, SLOT(deleteLater()));

    // start thread
    mainThread->start();
}

// update iteration label each time iteration is complete and thread sends iterationFinished signal
void MainWindow::UpdateIterationLabel(int numIterations) {

    // convert int to qstring to update iteration label
    QString iterations;
    iterations.setNum(numIterations);
    this->iterationLabel->setText("Iteration counter: " + iterations);
}
