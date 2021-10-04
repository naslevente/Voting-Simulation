#include "basesimulation.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

BaseSimulation::BaseSimulation(int numVoters, int numCandidates) {

    srand(time);

    this->numCandidates = numCandidates;
    this->numVoters = numVoters;

    finalCandidates = std::make_tuple<int, int>(-1, -1);

    simulationBallots = std::vector<std::shared_ptr<Ballot>>(numVoters);
}

// create 1d array of probailities and attach to 2d array of all probabilities
void BaseSimulation::GetProbabilities() {

    std::vector<double> probabilities = std::vector<double>();
    probabilities.push_back((long double)((this->numberOfManipulations / (long double)(numberOfIterations)) * 100));
    probabilities.push_back((long double)((this->numberOfSuccesses / (long double)(numberOfIterations)) * 100));
    probabilities.push_back((long double)((this->minoritySuccesses / (long double)(numberOfIterations)) * 100));

    return probabilities;
}

void BaseSimulation::CountVotes() {

    // count the votes
    for(int i = 0; i < numVoters; ++i) {

        for(int j = 0; j < numCandidates; ++j) {

            if(simulationBallots.at(i)->ballot.at(j) != 0) {

                voteCount.at(j) += 1;
            }
        }
    }
}

void BaseSimulation::Manipulation(int winningCandidateOne, int winningCandidateTwo) {

    int manipulations = 0;
    for(int i = 0; i < numVoters; ++i) {

        std::shared_ptr<Ballot> currentVoter = simulationBallots.at(i);
        bool hasManipulated = false;
        for(int j = 0; j < numCandidates; ++j) {

            // R(k - 1) iteracio szerint elokelobb helyen vegzett, de a szavazo sajat rangsoraban hatrebb sorolt szituacio
            if((j == winningCandidateOne || j == winningCandidateTwo) &&
                currentVoter->ballot.at(j) != 1 && currentVoter->ballot.at(j) != 2 && currentVoter->ballot.at(j) != 0) {

                currentVoter->ballot.at(j) = 0;

                if(!hasManipulated) {

                    manipulations += 1;
                    hasManipulated = true;
                }
            }
        }
    }

    // add up probability from each simulation and average after each iteration
    manipulationCount = manipulations;
    numberOfManipulations += (long double)(manipulations / (long double)(numVoters));
}

// accepting tuple input as reference
template<typename T, typename Y, typename U>
void BaseSimulation::UpdateWinTuple(T &finalCandidates, Y comparison, U currentCandidate) {

    int firstWinningCandidate = std::get<0>(finalCandidates);
    int secondWinningCandidate = std::get<1>(finalCandidates);

    if(firstWinningCandidate == -1 || comparison.at(currentCandidate) > comparison.at(firstWinningCandidate)) {

        secondWinningCandidate = firstWinningCandidate;
        firstWinningCandidate = currentCandidate;
    }
    else if(secondWinningCandidate == -1 || comparison.at(currentCandidate) > comparison.at(secondWinningCandidate)) {

        secondWinningCandidate = currentCandidate;
    }
    else if(comparison.at(currentCandidate) == comparison.at(secondWinningCandidate)) {

        int random = rand() % 2;
        if(random) {

            secondWinningCandidate = currentCandidate;
        }
    }

    std::get<0>(finalCandidates) = firstWinningCandidate;
    std::get<1>(finalCandidates) = secondWinningCandidate;
}

bool BaseSimulation::IsMajority() {

    // check if final candidates meet majority count requirements
    int firstCandidate = std::get<0>(finalCandidates);
    int secondCandidate = std::get<1>(finalCandidates);
    if(voteCount.get(firstCandidate) >= (int)(numVoters / 2) && voteCount.get(secondCandidate) >= (int)(numVoters / 2))
        return true;
    else
        return false;
}
