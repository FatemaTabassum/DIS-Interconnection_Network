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
#include <vector>
#include <string>


#define SIZE 100000
#define LINKSSIZE 65540

using namespace std;

bool color[SIZE];
bool queueColor[SIZE];
int totalNumberOfLink;

vector<int> edges[LINKSSIZE];
queue <int> nodesQueue;


void reset() {
    for (int i = 0; i < SIZE; i++) {
        color[i] = false;
    }
    for (int i = 0; i < SIZE; i++) {
        queueColor[i] = false;
    }
    for (int i = 0; i < LINKSSIZE; i++) {
        edges[i].clear();
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

string getBinaryRepresentation(int num, int dimensionOfHyperCube) {
    string s = "";
    //printf("num = %d\n", num);
    for (int i = dimensionOfHyperCube-1; i > -1; i--) {
        //printf("i = %d dimension = %d 1<<i = %d    1<<i & num = %d \n", i, dimension, 1<<i, 1<<i & num);
        s += (1<<i & num)? "1": "0";
    }
    return s;
}

void printAllTheLinks(int dimensionOfHyperCube) {
    int numberOfNodes = 2 << (dimensionOfHyperCube-1);
    printf(" totoal nodes count %d\n",numberOfNodes);
    for (int i = 0; i < numberOfNodes ; i++) {
        cout<< i << "("<< getBinaryRepresentation(i,dimensionOfHyperCube) << ")" << ":" << " ";
        for ( int j = 0; j < edges[i].size(); j++) {
            cout<< edges[i][j] << "("<< getBinaryRepresentation(edges[i][j],dimensionOfHyperCube) << ")" << " ";
        }
        cout<<endl;
    }
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
                edges[rootNode].push_back(newNode);
                edges[newNode].push_back(rootNode);
                //printf("%d ---> %d   %d ---> %d\n", rootNode, edges[rootNode].back(), newNode, edges[newNode].back());
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
        printAllTheLinks(dimensionOfHyperCube);
    }
    return 0;
}
