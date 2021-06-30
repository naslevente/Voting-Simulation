#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include "Ballot.h"
#include "Graph.h"
#include "GraphController.h"

#define MAXWEIGHT 500

#ifndef SIMULATION_H
#define SIMULATION_H

class Simulation {

    public:

        std::vector<int> igenNemCount;
        bool isOne;
        bool isSchulze;

        // necessary variables for analysis
        int numberOfManipulations;
        int numberOfSuccesses;
        int minoritySuccesses; // only for schulze

        Simulation(int numVoters, int numCandidates) {

            this->numCandidates = numCandidates;
            this->numVoters = numVoters;

            simulationBallots = std::vector<std::shared_ptr<Ballot>>(numVoters);

            // create the instance of graphController
            control = std::make_unique<GraphController>(this->numCandidates);
        }

        ~Simulation() = default;

        // !! negy kulonbozo eljaras !!

        void StartIgenNemSimulation() {

            // count ballots
            auto winningCandidates = IgenNemCount(igenNemCount);

            if(std::get<0>(winningCandidates) != -1 && std::get<1>(winningCandidates) != -1) {

                numberOfSuccesses += 1;
            }

            // manipulate current ballots and rerun the simulation
            Manipulation(std::get<0>(winningCandidates), std::get<1>(winningCandidates));

            // clear the count array
            igenNemCount.clear();
        }

        void StartSchulzeSimulation() {

            // count the ballots
            CountPreferances();

            // calculate strongest paths
            auto finalRank = FindWinningSchulzeCandidates();

            if(finalRank.at(0) != -1 && finalRank.at(1) != -1) {

                numberOfSuccesses += 1;
            }

            // manipulate the current ballots and rerun the simulation
            Manipulation(finalRank.at(0), finalRank.at(1));
        }

        void GenerateRandomBallots() {

            for(int i = 0; i < numVoters; ++i) {

                std::shared_ptr<Ballot> newBallot = std::make_shared<Ballot>(numCandidates, isOne, isSchulze);
                simulationBallots.at(i) = newBallot;
            }
        }

        // two candidates in the tuple are the winning candidate (-1 if there are none in the majority scenario)
        std::tuple<int, int> IgenNemCount(std::vector<int> &igenNemCount) {

            int firstWinningCandidate = 0;
            int secondWinningCandidate = 0;

            // count the votes
            for(int i = 0; i < numVoters; ++i) {

                for(int j = 0; j < numCandidates; ++j) {

                    if(simulationBallots.at(i)->ballot.at(j) != 0) {

                        igenNemCount.at(j) += 1;
                    }
                }
            }

            auto winningCandidates = DetermineWinners();
            return winningCandidates;
        }

        std::tuple<int, int> DetermineWinners() {

            int firstWinningCandidate = -1;
            int secondWinningCandidate = -1;

            if(isOne) {

                // find the winning candidates
                // TODO: make a case for ties
                for(int i = 0; i < igenNemCount.size(); ++i) {

                    if(igenNemCount.at(i) >= (int)(numVoters / 2)) {

                        if(igenNemCount.at(i) > firstWinningCandidate) {

                            firstWinningCandidate = igenNemCount.at(i);
                        }
                        else if(igenNemCount.at(i) > secondWinningCandidate) {

                            secondWinningCandidate = igenNemCount.at(i);
                        }
                    }
                }

                auto winningCandidates = std::make_tuple(firstWinningCandidate, secondWinningCandidate);
                return winningCandidates;
            }
            else {

                for(int i = 0; i < igenNemCount.size(); ++i) {

                    if(igenNemCount.at(i) > firstWinningCandidate) {

                        firstWinningCandidate = igenNemCount.at(i);
                    }
                    else if(igenNemCount.at(i) > secondWinningCandidate) {

                        secondWinningCandidate = igenNemCount.at(i);
                    }
                }

                auto winningCandidates = std::make_tuple(firstWinningCandidate, secondWinningCandidate);
                return winningCandidates;
            }
        }

        void CountPreferances() {

            // this method loops through the candidates and calculates the number of voters who
            // prefer a certain candidate over another
            for(int i = 0; i < numCandidates; ++i) {

                int currentCandidate = i;
                for(int j = i + 1; j < numCandidates; ++j) {

                    int iPreferance = 0;
                    int jPreferance = 0;
                    for(int k = 0; k < numVoters; ++k) {

                        // currently ignoring special case with same ranks
                        if(simulationBallots.at(k)->ballot.at(i) < simulationBallots.at(k)->ballot.at(j)) {

                            ++iPreferance;
                        }
                        else {

                            ++jPreferance;
                        }
                    }

                    control->AddToAdjMatrix(i, j, iPreferance);
                    control->AddToAdjMatrix(j, i, jPreferance);
                }
            }
        }

        // function will return a vector that will contain all the candidates in order of winning
        std::vector<int> FindWinningSchulzeCandidates() {

            control->CalculateStrongestPaths();

            std::vector<int> finalRank = std::vector<int>(numCandidates, -1);

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

                // refer to wikipedia table with strong paths
                // count the number of wins a certain row number has over its column associates
                // that number minus the number of candidates gives the ranking for that row candidate
                finalRank.at(4 - numGreater) = i;
            }

            return finalRank;
        }

        // same manipulations for each simulation
        void Manipulation(int winningCandidateOne, int winningCandidateTwo) {

            for(int i = 0; i < numVoters; ++i) {

                std::shared_ptr<Ballot> currentVoter = simulationBallots.at(i);
                for(int j = 0; j < numCandidates; ++j) {

                    // R(k - 1) iteracio szerint elokelobb helyen vegzett, de a szavazo sajat rangsoraban hatrebb sorolt szituacio
                    if((j == winningCandidateOne || j == winningCandidateTwo) &&
                        currentVoter->ballot.at(j) != 1 && currentVoter->ballot.at(j) != 2) {

                        currentVoter->ballot.at(j) = 0;
                        numberOfManipulations += 1;
                    }
                }
            }
        }

    private:

        int numCandidates;
        int numVoters;

        std::vector<std::shared_ptr<Ballot>> simulationBallots;
        std::unique_ptr<GraphController> control;
};

#endif // SIMULATION_H
