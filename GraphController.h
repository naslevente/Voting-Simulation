#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <memory>
#include "WeightedLink.h"
#include "Graph.h"
#include "Ballot.h"

#ifndef GRAPHCONTROLLER_H
#define GRAPHCONTROLLER_H

class GraphController {

    public:

        std::shared_ptr<Graph> graph;
        size_t graphSize;

        GraphController(int graphSize) {

            this->graphSize = graphSize;
            graph = std::make_shared<Graph>(graphSize);
        }

        void AddToAdjMatrix(int firstNode, int secondNode, int count) {

            // once preferances are counted between firstNode and secondNode,
            // add to adjMatrix
            if(!graph->AddLink(firstNode, secondNode, count)) {

                std::cout << "Error: unable to add edge to graph" << std::endl;
            }
        }

        void CalculateStrongestPaths() {

            for(int i = 0; i < graphSize; ++i) {

                for(int j = 0; j < graphSize; ++j) {

                    if(i != j) {

                        if(graph->adjMatrix.at(i).at(j) > graph->adjMatrix.at(j).at(i)) {

                            graph->strongPaths.at(i).at(j) = graph->adjMatrix.at(i).at(j);
                        }
                        else {

                            graph->strongPaths.at(i).at(j) = 0;
                        }
                    }
                }
            }

            for(int i = 0; i < graphSize; ++i) {

                for(int j = 0; j < graphSize; ++j) {

                    if(i != j) {

                        for(int k = 0; k < graphSize; ++k) {

                            if(i != k && j != k) {

                                graph->strongPaths.at(j).at(k) = std::max(graph->strongPaths.at(j).at(k),
                                    std::min(graph->strongPaths.at(j).at(i), graph->strongPaths.at(i).at(k)));
                            }
                        }
                    }
                }
            }
        }
};

#endif // GRAPHCONTROLLER_H
