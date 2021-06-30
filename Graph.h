#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <memory>
#include "WeightedLink.h"

#ifndef GRAPH_H
#define GRAPH_H

class Graph {

    public:

        std::vector<std::vector<int>> adjMatrix;
        std::vector<std::vector<int>> strongPaths;

        // vector to track those nodes that have been visited during dfs
        std::vector<int> tracker;

        // size of the matrix
        size_t vertices;

        Graph(size_t numVert) {

            vertices = numVert;

            // Define the size for the order and adjMatrix vector
            adjMatrix = std::vector<std::vector<int>>(vertices, std::vector<int>(vertices, -1));
            strongPaths = std::vector<std::vector<int>>(vertices, std::vector<int>(vertices, -1));
            tracker = std::vector<int>(numVert, 0);
        }
        ~Graph() {}

        std::vector<std::vector<int>> getStrongPaths() {

            return this->strongPaths;
        }

        void DisplayAdjMatrix() {

            for(int i = 0; i < adjMatrix.size(); ++i) {

                for(int j = 0; j < adjMatrix.at(0).size(); ++j) {

                    std::cout << adjMatrix.at(i).at(j) << " ";
                }

                std::cout << std::endl;
            }
        }

        void DisplayStrongPaths() {

            for(int i = 0; i < strongPaths.size(); ++i) {

                for(int j = 0; j < strongPaths.at(0).size(); ++j) {

                    std::cout << strongPaths.at(i).at(j) << " ";
                }

                std::cout << std::endl;
            }
        }

        void ResetTracker() {

            tracker = std::vector<int>(vertices, 0);
        }

        bool AddLink(int currentNode, int nextNode, int weight) {

            adjMatrix.at(currentNode).at(nextNode) = weight;

            return true;
        }
};

#endif // GRAPH_H
