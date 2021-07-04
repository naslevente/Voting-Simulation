#ifndef SIMULATIONWORKER_H
#define SIMULATIONWORKER_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "Simulation.h"

class SimulationWorker : public QObject
{

    Q_OBJECT

public:
    SimulationWorker(int, int);
    ~SimulationWorker();

    // boolean value that works with the stop button
    bool isRunning = true;

public slots:
    // main method that initiates simulating
    void Simulate();

signals:
    // finished signal for when the stop button is pressed
    void finished();

    // iteration finished signal to update iteration label
    void iterationFinished(int numIterations);

private:

    // all the simulations that will be running
    Simulation *igenNemOne;
    Simulation *igenNemTwo;
    Simulation *schulzeOne;
    Simulation *schulzeTwo;

    // number of voters and number of candidates fields
    int numVoters;
    int numCandidates;
};

#endif // SIMULATIONWORKER_H
