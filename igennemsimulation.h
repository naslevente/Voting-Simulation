#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "basesimulation.h"
#include "Ballot.h"
#include "Graph.h"
#include "GraphController.h"

#pragma once
#ifndef IGENNEMSIMULATION_H
#define IGENNEMSIMULATION_H


class igennemsimulation : BaseSimulation {
public:

    igennemsimulation();
    ~igennemsimulation() = default;

    void BeginSimulation();
    void DetermineWinners();
};

#endif // IGENNEMSIMULATION_H
