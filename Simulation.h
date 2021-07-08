#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <vector>
#include <QTableWidget>
#include "Ballot.h"
#include "Graph.h"
#include "GraphController.h"

#define MAXWEIGHT 500

#ifndef SIMULATION_H
#define SIMULATION_H

class Simulation {

    public:

        std::vector<int> igenNemCount;
        std::vector<int> minorityCount;
        bool isOne;
        bool isSchulze;

        // necessary variables for analysis
        long double numberOfManipulations;
        int numberOfSuccesses;
        int minoritySuccesses; // only for schulze

        Simulation(int numVoters, int numCandidates) {

            srand(time(0));

            this->numCandidates = numCandidates;
            this->numVoters = numVoters;

            numberOfManipulations = 0;
            numberOfSuccesses = 0;
            minoritySuccesses = 0;

            simulationBallots = std::vector<std::shared_ptr<Ballot>>(numVoters);

            // create the instance of graphController
            control = std::make_unique<GraphController>(this->numCandidates);
        }

        ~Simulation() = default;

        // create 1d array of probailities and attach to 2d array of all probabilities
        std::vector<double> getProbabilities(int iterations) {

            std::vector<double> probabilities = std::vector<double>();
            probabilities.push_back((long double)((this->numberOfManipulations / iterations) * 100));
            probabilities.push_back((long double)((this->numberOfSuccesses / (long double)(iterations)) * 100));
            probabilities.push_back((long double)((this->minoritySuccesses / (long double)(iterations)) * 100));

            return probabilities;
        }

        // getters and setters
        int getNumberOfManipulations() {

            return this->numberOfManipulations;
        }

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
            igenNemCount = std::vector<int>(numCandidates, 0);
        }

        void StartSchulzeSimulation() {

            // count the ballots
            CountPreferances();

            // calculate strongest paths
            auto finalRank = FindWinningSchulzeCandidates();

            bool isSuccess = false;
            if(finalRank.at(0) != -1 && finalRank.at(1) != -1) {

                numberOfSuccesses += 1;
                isSuccess = true;
            }

            // manipulate the current ballots and rerun the simulation
            Manipulation(finalRank.at(0), finalRank.at(1));

            // calculate minorty probability
            int max = 0;
            for(int i = 0; i < minorityCount.size(); ++i) {

                max = std::max(minorityCount.at(i), max);
            }

            // check if first or second place winner was a minority count leader
            if(isSuccess && (minorityCount.at(finalRank.at(0)) == max || minorityCount.at(finalRank.at(1)) == max) && max != 0) {

                minoritySuccesses += 1;
            }

            // reset minority count vector
            minorityCount = std::vector<int>(numCandidates, -1);
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

                // find the winning candidates by looping through the number of voters
                // that voted for a certain candidate and determining candidate with most votes
                for(int i = 0; i < igenNemCount.size(); ++i) {

                    if(igenNemCount.at(i) >= (int)(numVoters / 2) || !isOne) {

                        if(firstWinningCandidate == -1) {

                            firstWinningCandidate = i;
                        }
                        else if(secondWinningCandidate == -1) {

                            secondWinningCandidate = i;
                        }
                        else {

                            // change random number generator seed and generate either 0 or 1
                            srand(time(0));
                            int random = rand() % 2;

                            if(igenNemCount.at(i) > igenNemCount.at(firstWinningCandidate)) {

                                secondWinningCandidate = firstWinningCandidate;
                                firstWinningCandidate = i;
                            }
                            else if(igenNemCount.at(i) > igenNemCount.at(secondWinningCandidate)) {

                                secondWinningCandidate = i;
                            }
                            else if(igenNemCount.at(i) == igenNemCount.at(firstWinningCandidate)) {

                                if(random) {

                                    secondWinningCandidate = firstWinningCandidate;
                                    firstWinningCandidate = i;
                                }
                                else {

                                    secondWinningCandidate = i;
                                }
                            }
                            else if(igenNemCount.at(i) == igenNemCount.at(secondWinningCandidate)) {

                                if(random) {

                                    secondWinningCandidate = i;
                                }
                            }
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

            // initialize vector that will count minority
            minorityCount = std::vector<int>(numCandidates, -1);

            // this method loops through the candidates and calculates the number of voters who
            // prefer a certain candidate over another
            for(int i = 0; i < numCandidates; ++i) {

                // count minority votes for each candidate
                int elfogadSzam = 0;
                for(int j = 0; j < numVoters; ++j) {

                    if(simulationBallots.at(j)->ballot.at(i) == 0) {

                        elfogadSzam += 1;
                    }
                }

                // update minority count vector
                minorityCount.at(i) = elfogadSzam;

                for(int j = i + 1; j < numCandidates; ++j) {

                    int iPreferance = 0;
                    int jPreferance = 0;
                    for(int k = 0; k < numVoters; ++k) {

                        // TODO: consider a voter using same rank on two different candidates
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
                finalRank.at((numCandidates - 1) - numGreater) = i;
            }

            return finalRank;
        }

        // same manipulations for each simulation
        void Manipulation(int winningCandidateOne, int winningCandidateTwo) {

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
            numberOfManipulations += (long double)(manipulations / (long double)(numVoters));
        }

    private:

        int numCandidates;
        int numVoters;

        std::vector<std::shared_ptr<Ballot>> simulationBallots;
        std::unique_ptr<GraphController> control;
};

#endif // SIMULATION_H
