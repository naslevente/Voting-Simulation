#include "schulzesimmulation.h"

schulzesimmulation::schulzesimmulation() {

    // initialize simulation ballot vector
    simulationBallots = std::vector<std::shared_ptr<Ballot>>(this->numVoters);

    // create graph controller object that will run schulze simulation
    control = new GraphController(this->numCandidates);
}

schulzesimmulation::~schulzesimmulation() {

    delete control;
}

void schulzesimmulation::BeginSimulation() {

    // count ballots
    CountPreferances(); // set up preferances for graph algo
    CountVotes(); // necessary for second schulze simulation where majority matters

    // update finalCandidates tuple with the winning candidates
    DetermineWinners();

    // determine if iteration was success
    bool isSuccess = false;
    if(std::get<0>(finalCandidates) != -1 && std::get<1>(finalCandidates) != -1) {

        if(doesNeedMajority && IsMajority()) {

            numberOfSuccesses += 1;
            isSuccess = true;
        }
        else if(!doesNeedMajority) {

            numberOfSuccesses += 1;
            isSuccess = true;
        }
    }

    // manipulate the current ballots based off winning candidates
    Manipulation(firstWinCan, secondWinCan);

// !! calculate minorty probability
    for(int i = 0; i < minorityCount.size(); ++i) {

        if(minorityCount.at(i) < (int)(numVoters / 2)) {

            minorityCount.at(i) = -1;
        }
    }

    // check is first or second candidate is a minority
    if(isSuccess && (minorityCount.at(firstWinCan) >= int(numVoters / 2) || minorityCount.at(secondWinCan) >= int(numVoters / 2))) {

        minoritySuccesses += 1;
    }
// !!

    // reset necessary fields
    voteCount = std::vector<int>(numCandidates, 0);
    minorityCount = std::vector<int>(numCandidates, -1);
    finalCandidates = std::make_tuple<int, int>(-1, -1);

    ++numberOfIterations;
}

void schulzesimmulation::DetermineWinners() {

    control->CalculateStrongestPaths();

    std::vector<int> candidateValue = std::vector<int>(numCandidates, -1);

    std::vector<std::vector<int>> strongPaths = control->graph->getStrongPaths();
    for(int i = 0; i < strongPaths.size(); ++i) {

        int numGreater = 0;
        for(int j = 0; j < strongPaths.at(0).size(); ++j) {

            if(i != j) {

                if(strongPaths.at(i).at(j) > strongPaths.at(j).at(i)) {

                    numGreater += 1;
                }
            }
        }

        // winning candidate is determined by number of strong paths greater than
        // reverse path
        candidateValue.at(i) = numGreater; // this will be sent to UpdateWinTuple
    }

    for(int i = 0; i < numCandidates; ++i) {

        UpdateWinTuple(this->finalCandidates, candidateValue, i);
    }
}

void schulzesimmulation::CountPreferances() {

    // initialize vector that will count minority
    minorityCount = std::vector<int>(numCandidates, -1);

    // this method loops through the candidates and calculates the number of voters who
    // prefer a certain candidate over another
    for(int i = 0; i < numCandidates; ++i) {

        // count minority votes for each candidate
        int elfogadhatatlanSzam = 0;
        for(int j = 0; j < numVoters; ++j) {

            if(simulationBallots.at(j)->ballot.at(i) == 0) {

                elfogadhatatlanSzam += 1;
            }
        }

        // update minority count vector
        minorityCount.at(i) = elfogadhatatlanSzam;

        for(int j = i + 1; j < numCandidates; ++j) {

            int iPreferance = 0;
            int jPreferance = 0;
            for(int k = 0; k < numVoters; ++k) {

                // TODO: consider a voter using same rank on two different candidates
                if(simulationBallots.at(k)->ballot.at(i) < simulationBallots.at(k)->ballot.at(j)) {

                    if(simulationBallots.at(k)->ballot.at(i) == 0) {

                        ++jPreferance;
                    }
                    else {

                        ++iPreferance;
                    }
                }
                else if(simulationBallots.at(k)->ballot.at(j) < simulationBallots.at(k)->ballot.at(i)){

                    if(simulationBallots.at(k)->ballot.at(j) == 0) {

                        ++iPreferance;
                    }
                    else {

                        ++jPreferance;
                    }
                }
            }

            control->AddToAdjMatrix(i, j, iPreferance);
            control->AddToAdjMatrix(j, i, jPreferance);
        }
    }
}
