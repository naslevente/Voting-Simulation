#include "simulationworker.h"
#include "basesimulation.h"
#include "igennemsimulation.h"
#include "schulzesimmulation.h"

SimulationWorker::SimulationWorker(int numVoters, int numCandidates)
{

    // set the numVoters and numCandidates fields
    this->numVoters = numVoters;
    this->numCandidates = numCandidates;

    // heap allocate each simulation type
    igenNemOne = new igennemsimulation(this->numVoters, this->numCandidates);
    igenNemTwo = new igennemsimulation(this->numVoters, this->numCandidates);
    schulzeOne = new schulzesimmulation(this->numVoters, this->numCandidates);
    schulzeTwo = new schulzesimmulation(this->numVoters, this->numCandidates);

    // !! generate random batch of ballots for each simulation !!

    // doesNeedMajority boolean field will determine if a simulation requires majority vote for the candidates to win
    // isSchulze boolean field will distinguish between schulze simulation and igen-nem simulation (these distinctions are used
    // when generating random ballots

    igenNemOne->isGettingFullBallot = false;
    igenNemOne->doesNeedMajority = false;
    igenNemOne->GenerateRandomBallots();
    igenNemOne->voteCount = std::vector<int>(this->numCandidates);

    igenNemTwo->isGettingFullBallot = false;
    igenNemTwo->doesNeedMajority = true;
    igenNemTwo->GenerateRandomBallots();
    igenNemTwo->voteCount = std::vector<int>(this->numCandidates);

    schulzeOne->isGettingFullBallot = true;
    schulzeOne->doesNeedMajority = false;
    schulzeOne->GenerateRandomBallots();
    schulzeOne->voteCount = std::vector<int>(this->numCandidates);

    schulzeTwo->isGettingFullBallot = false;
    schulzeTwo->doesNeedMajority = true;
    schulzeTwo->GenerateRandomBallots();
    schulzeTwo->voteCount = std::vector<int>(this->numCandidates);
}

SimulationWorker::~SimulationWorker() {

    // delete heap allocated objects
    delete igenNemOne;
    delete igenNemTwo;
    delete schulzeOne;
    delete schulzeTwo;
}

std::vector<std::vector<double>> SimulationWorker::getProbabilities() {

    std::vector<std::vector<double>> probabilities = std::vector<std::vector<double>>();

    // get all individual probabilites from each simulation
    probabilities.push_back(igenNemOne->getProbabilities());
    probabilities.push_back(igenNemTwo->getProbabilities());
    probabilities.push_back(schulzeOne->getProbabilities());
    probabilities.push_back(schulzeTwo->getProbabilities());

    return probabilities;
}

void SimulationWorker::Simulate() {

    int simulationsFinished = 0;

    std::cout << "all simulations started" << std::endl;

    // while the stop button hasn't been pressed
    while(isRunning && simulationsFinished != 4) {

        // start new iteration of each simulation if there were changes to the ballots
        // otherwise increment simulationFinished (once all four simulations see no change, emit finished)
        if(igenNemOne->manipulationCount != 0 && (!igenNemOne->isDone)) igenNemOne->StartIgenNemSimulation();
        else if(igenNemOne->manipulationCount != -1) ++simulationsFinished, igenNemOne->manipulationCount = -1, igenNemOne->isDone = true;

        if(igenNemTwo->manipulationCount != 0 && (!igenNemTwo->isDone)) igenNemTwo->StartIgenNemSimulation();
        else if(igenNemTwo->manipulationCount != -1) ++simulationsFinished, igenNemTwo->manipulationCount = -1, igenNemTwo->isDone = true;

        if(schulzeOne->manipulationCount != 0 && (!schulzeOne->isDone)) schulzeOne->StartSchulzeSimulation();
        else if(schulzeOne->manipulationCount != -1) ++simulationsFinished, schulzeOne->manipulationCount = -1, schulzeOne->isDone = true;

        if(schulzeTwo->manipulationCount != 0 && (!schulzeTwo->isDone)) schulzeTwo->StartSchulzeSimulation();
        else if(schulzeTwo->manipulationCount != -1) ++simulationsFinished, schulzeTwo->manipulationCount = -1, schulzeTwo->isDone = true;

        iterations += 1;
        //emit iterationFinished(iterations);

        /*
        std::cout << "winning candidates: " << std::get<0>(igenNemOne->finalCandidates) << " " << std::get<1>(igenNemOne->finalCandidates) <<
                     " and the number of manipulations: " << igenNemOne->manipulationCount << std::endl;
        std::cout << "winning candidates: " << std::get<0>(igenNemTwo->finalCandidates) << " " << std::get<1>(igenNemTwo->finalCandidates) <<
                     " and the number of manipulations: " << igenNemTwo->manipulationCount << std::endl;
        std::cout << "winning candidates: " << std::get<0>(schulzeOne->finalCandidates) << " " << std::get<1>(schulzeOne->finalCandidates) <<
                     " and the number of manipulations: " << schulzeOne->manipulationCount << std::endl;
        std::cout << "winning candidates: " << std::get<0>(schulzeTwo->finalCandidates) << " " << std::get<1>(schulzeTwo->finalCandidates) <<
                     " and the number of manipulations: " << schulzeTwo->manipulationCount << std::endl;
        */

        std::cout << "iteration finished"  << std::endl;
    }

    std::cout << "all simulations finished" << std::endl;

    // emit finished if stop button pressed or if all simulations have converged ballots
    emit iterationFinished(iterations);
    emit finished();
}
