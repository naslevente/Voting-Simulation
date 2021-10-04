#include "igennemsimulation.h"

igennemsimulation::igennemsimulation() {}

igennemsimulation::BeginSimulation() {

    // count ballots
    CountVotes();

    // update final candidate tuple with winning candidates
    DetermineWinners();

    // determine if iteration was success
    if(std::get<0>(finalCandidates) != -1 && std::get<1>(finalCandidates) != -1) {

        if(doesNeedMajority && IsMajority()) numberOfSuccesses += 1;
        else if(!doesNeedMajority) numberOfSuccesses += 1;
    }

    // manipulate current ballots and rerun the simulation
    Manipulation(std::get<0>(finalCandidates), std::get<1>(finalCandidates));

    // reset necessary fields
    voteCount = std::vector<int>(numCandidates, 0);
    finalCandidates = std::make_tuple<int, int>(-1, -1);

    ++numberOfIterations;
}

void igennemsimulation::DetermineWinners() {

    /*
    int firstWinningCandidate = -1;
    int secondWinningCandidate = -1;

    // find the winning candidates by looping through the number of voters
    // that voted for a certain candidate and determining candidate with most votes
    for(int i = 0; i < voteCount.size(); ++i) {

        if(voteCount.at(i) >= (int)(numVoters / 2) || !isOne) {

            if(firstWinningCandidate == -1) {

                firstWinningCandidate = i;
            }
            else if(secondWinningCandidate == -1) {

                secondWinningCandidate = i;
            }
            else {

                // holtverseny eseten egy random szam alapitja meg a nyertest
                int random = rand() % 2;

                if(voteCount.at(i) > voteCount.at(firstWinningCandidate)) {

                    secondWinningCandidate = firstWinningCandidate;
                    firstWinningCandidate = i;
                }
                else if(voteCount.at(i) > voteCount.at(secondWinningCandidate)) {

                    secondWinningCandidate = i;
                }
                else if(voteCount.at(i) == voteCount.at(firstWinningCandidate)) {

                    if(random) {

                        secondWinningCandidate = firstWinningCandidate;
                        firstWinningCandidate = i;
                    }
                    else {

                        secondWinningCandidate = i;
                    }
                }
                else if(voteCount.at(i) == voteCount.at(secondWinningCandidate)) {

                    if(random) {

                        secondWinningCandidate = i;
                    }
                }
            }
        }
    }

    auto winningCandidates = std::make_tuple(firstWinningCandidate, secondWinningCandidate);
    this->finalCandidates = &winningCandidates;
    */

    for(int i = 0; i < voteCount.size(); ++i) {

        UpdateWinTuple(this->finalCandidates, voteCount, i);
    }
    /*
    else {

        for(int i = 0; i < voteCount.size(); ++i) {

            if(voteCount.at(i) > firstWinningCandidate) {

                firstWinningCandidate = voteCount.at(i);
            }
            else if(voteCount.at(i) > secondWinningCandidate) {

                secondWinningCandidate = voteCount.at(i);
            }
        }

        auto winningCandidates = std::make_tuple(firstWinningCandidate, secondWinningCandidate);
        return winningCandidates;
    }
    */
}
