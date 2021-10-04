#include "basesimulation.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Ballot.h"
#include "GraphController.h"

#pragma once
#ifndef SCHULZESIMMULATION_H
#define SCHULZESIMMULATION_H


class schulzesimmulation : BaseSimulation {

public:

    schulzesimmulation();
    ~schulzesimmulation();

    // overwrite base class functions
    void BeginSimulation();
    void DetermineWinners();

    // personal functions
    void CountPreferances();

private:

    std::vector<std::shared_ptr<ballots>> simulationBallots;

    GraphController *control;
};

#endif // SCHULZESIMMULATION_H
