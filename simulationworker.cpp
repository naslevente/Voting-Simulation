#include "simulationworker.h"

SimulationWorker::SimulationWorker(int numVoters, int numCandidates)
{

    // set the numVoters and numCandidates fields
    this->numVoters = numVoters;
    this->numCandidates = numCandidates;

    // heap allocate each simulation type
    igenNemOne = new Simulation(this->numVoters, this->numCandidates);
    igenNemTwo = new Simulation(this->numVoters, this->numCandidates);
    schulzeOne = new Simulation(this->numVoters, this->numCandidates);
    schulzeTwo = new Simulation(this->numVoters, this->numCandidates);

    // !! generate random batch of ballots for each simulation !!

    // isOne boolean field will distinguish a simulation from its counterpart (igen-nem elso eljaras v.s. igen-nem masodik eljaras
    // isSchulze boolean field will distinguish between schulze simulation and igen-nem simulation (these distinctions are used
    // when generating random ballots

    igenNemOne->GenerateRandomBallots();
    igenNemOne->igenNemCount = std::vector<int>(this->numCandidates);
    igenNemOne->isOne = true;
    igenNemOne->isSchulze = false;

    igenNemTwo->GenerateRandomBallots();
    igenNemTwo->igenNemCount = std::vector<int>(this->numCandidates);
    igenNemTwo->isOne = false;
    igenNemTwo->isSchulze = false;

    schulzeOne->GenerateRandomBallots();
    schulzeOne->isOne = true;
    schulzeOne->isSchulze = true;

    schulzeTwo->GenerateRandomBallots();
    schulzeTwo->isOne = false;
    schulzeTwo->isSchulze = true;
}

SimulationWorker::~SimulationWorker() {

    // delete heap allocated objects
    delete igenNemOne;
    delete igenNemTwo;
    delete schulzeOne;
    delete schulzeTwo;
}

void SimulationWorker::Simulate() {

    // iteration counter for iteration label
    int iterations = 0;

    // while the stop button hasn't been pressed
    while(isRunning) {

        // start new iteration of each simulation
        igenNemOne->StartIgenNemSimulation();
        igenNemTwo->StartIgenNemSimulation();
        schulzeOne->StartSchulzeSimulation();
        schulzeTwo->StartSchulzeSimulation();

        iterations += 1;
        emit iterationFinished(iterations);
    }

    // emit finished if stop button pressed
    emit finished();
}
