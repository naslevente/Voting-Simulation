#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <algorithm>
#include <tuple>

#include <vector>

#ifndef BALLOT_H
#define BALLOT_H

class Ballot {

    public:

        std::vector<int> ballot;

        Ballot(int numCandidates, bool isOne, bool isSchulze) {

            int startingRank = 1;
            int step = 1;

            ballot = std::vector<int>(numCandidates, -1);
            for(int i = 0; i < numCandidates; ++i) {

                int randomIndex = rand() % numCandidates;
                while(ballot.at(randomIndex) != -1) {

                    randomIndex = rand() % numCandidates;
                }

                int nextRandNumber = rand() % 2;
                if(nextRandNumber || (isOne && isSchulze)) { // elso schulze eljarasban mindegyik kandidatus kap egy szavazatot

                    ballot.at(randomIndex) = startingRank;
                    startingRank += step;
                }
                else {

                    ballot.at(randomIndex) = 0;
                }
            }

            for(int i = 0; i < ballot.size(); ++i) {

                std::cout << ballot.at(i) << " ";
            }

            std::cout << std::endl;
        }

        int getRank(int candidate) {

            return ballot.at(candidate);
        }
};

#endif // BALLOT_H
