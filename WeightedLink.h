#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <memory>

#ifndef WEIGHTEDLINK_H
#define WEIGHTEDLINK_H

class WeightedLink {

    public:

        int weight;
        int vert;
        std::shared_ptr<WeightedLink> next;

        WeightedLink(int vertNum) : vert(vertNum) {}
};

#endif // WEIGHTEDLINK_H
