//
//  HyperCubeLink.cpp
//  HyperCubeLink
//
//  Created by Fatema on 9/7/19.
//  Copyright © 2019 Liza. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <algorithm>
#include <queue>

#define SIZE 100000

using namespace std;

bool color[SIZE];
bool queueColor[SIZE];

int totalNumberOfLink;
queue <int> nodesQueue;

void reset() {
    for (int i = 0; i < SIZE; i++) {
        color[i] = false;
    }
    for (int i = 0; i < SIZE; i++) {
        queueColor[i] = false;
    }
}

void setColorOfNode(int nodeNumber, bool colorArray[]) {
    colorArray[nodeNumber] = true;
}

bool checkColorOfNode(int nodeNum, bool colorArray[]) {
    if (colorArray[nodeNum] == false) {
        return false;
    }
    return true;
}

int getNodeWithHammingDistance(int nodeNum, int bitNum) {
    return nodeNum ^ (1<<bitNum);
}

void printLink(int node1, int node2) {
    cout << "Link:  " << node1 << " -->  " << node2 << endl;
}

void bfsTraversal( int numberOfNodesInHypercube, int dimensionOfHypercube) {
    while (1) {
        if (nodesQueue.empty() == true) {
            break;
        }
        int rootNode = nodesQueue.front();
        nodesQueue.pop();
        if (checkColorOfNode(rootNode, color) == true) {
            continue;
        }
        setColorOfNode(rootNode, color);
        for (int i = 0; i < dimensionOfHypercube; i++) {
            int newNode = getNodeWithHammingDistance(rootNode, i);
            if (checkColorOfNode(newNode, color) == false) {
                //totalNumberOfLink++;
                printLink(rootNode, newNode);
                if(checkColorOfNode(newNode, queueColor) == false){
                    nodesQueue.push(newNode);
                    setColorOfNode(newNode, queueColor);
                }
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    
    int dimensionOfHyperCube, numberOfNodesInHypercube = 0;
    
    while (1) {
        cout << "Enter the dimension of HyperCube. To end the program enter -1" << endl;
        cin >> dimensionOfHyperCube;
        if (dimensionOfHyperCube == -1){
            break;
        }
        totalNumberOfLink = 0;
        reset();
        numberOfNodesInHypercube = 2 << (dimensionOfHyperCube-1);
        int node = 0;
        nodesQueue.push(node);
        bfsTraversal(numberOfNodesInHypercube, dimensionOfHyperCube);
    }
    return 0;
}
