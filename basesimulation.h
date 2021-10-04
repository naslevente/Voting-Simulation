#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Ballot.h"

#pragma once
#ifndef BASESIMULATION_H
#define BASESIMULATION_H


class BaseSimulation
{
public:

    BaseSimulation(int, int);
    ~BaseSimulation() = default;

    // subclasses will overwrite their own versions
    virtual BeginSimulation();
    virtual DetermineWinners();

    // inherited functions
    void CountVotes();
    void Manipulation(int, int);
    void GetProbabilities();
    bool IsMajority();

    template<typename T, typename Y, typename U>
    void UpdateWinTuple(T&, Y, U);

protected:

    int numVoters;
    int numCandidates;

    // necessary boolean fields
    bool doesNeedMajority;
    bool isGettingFullBallot;

    // necessary variables for analysis
    long double numberOfManipulations = 0;
    int numberOfSuccesses = 0;
    int minoritySuccesses = 0; // only for schulze
    int manipulationCount = -1;
    int numberOfIterations = 0;

    // necessary vectors
    std::vector<int> voteCount;
    std::vector<int> minorityCount;
    std::vector<std::shared_ptr<Ballot>> simulationBallots;

    std::tuple<int, int> finalCandidates;

};

#endif // BASESIMULATION_H
