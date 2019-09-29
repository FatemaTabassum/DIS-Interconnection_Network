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
#include <map>
#include <iterator>
#include <cstdlib>
#include <ctime>


#define SIZE 65540
#define LINKSSIZE 65540
#define START_NODE 0

using namespace std;

bool color[SIZE];
bool queueColor[SIZE];
int totalNumberOfLink;
int dimensionOfHyperCube = 0;
int numberOfNodesInHypercube = 0;
int pathCountForAllPathRouting = 0;
bool end_flag = false;

vector<int> edges[LINKSSIZE];
vector<int>pathVector;
queue <int> nodesQueue;
vector<bool> taken;


void reset() {
    taken.clear();
    for (int i = 0; i < SIZE; i++) {
        color[i] = false;
    }
    for (int i = 0; i < SIZE; i++) {
        queueColor[i] = false;
    }
    for (int i = 0; i < SIZE; i++) {
        taken.push_back(false);
        
    }
    for (int i = 0; i < LINKSSIZE; i++) {
        edges[i].clear();
    }
    pathVector.clear();
    end_flag = false;
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
    for (int i = dimensionOfHyperCube-1; i > -1; i--) {
        s += (1<<i & num)? "1": "0";
    }
    return s;
}

void printAllTheLinks(int dimensionOfHyperCube) {
    int numberOfNodes = 2 << (dimensionOfHyperCube-1);
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
                edges[rootNode].push_back(newNode);
                edges[newNode].push_back(rootNode);
                if(checkColorOfNode(newNode, queueColor) == false){
                    nodesQueue.push(newNode);
                    setColorOfNode(newNode, queueColor);
                }
            }
        }
    }
}

void dim_order_routing(int src, int dst) {
    if ((src < 0 || src > (numberOfNodesInHypercube -1)) || (dst < 0 || src > (numberOfNodesInHypercube -1))) {
        cout << "out of range" <<endl;
    }
    int current = src;
    int pathLength = 0;
    pathVector.push_back(src);
    for (int i= 0; i < 17; i++) {
        if (current == dst) {
            break;
        }
        if ((current & 0x01<<i) != (dst & 0x01 << i)) {
            current = current^(0x01 << i);
            pathVector.push_back(current);
            pathLength++;
        }
    }
    cout<<"Path from "<< src << " to "<< dst << ":" <<" ";
    for (int j = 0;j < pathVector.size(); j++) {
        cout << pathVector[j] << "(" << getBinaryRepresentation(pathVector[j], dimensionOfHyperCube) << ")";
        if (j == (pathVector.size() - 1)) {
            cout << endl;
        } else {
            cout<<"->";
        }
    }
    cout<< endl <<endl;
    return;
}

void allpath_routing(int src, int dst, int idx) {
    if (idx > dimensionOfHyperCube) {
        return;
    }
    if (src == dst) {
        pathCountForAllPathRouting++;
        cout<<"Path "<< pathCountForAllPathRouting + 1 << " : ";
        for (int j = 0; j < pathVector.size(); j++) {
            cout << pathVector[j] << "(" << getBinaryRepresentation(pathVector[j], dimensionOfHyperCube)  << ")";
            if ((pathVector.size() - 1) != j) {
                cout << "-->";
            }
        }
        cout<< endl;
        return;
    }
    for (int i = 0; i < dimensionOfHyperCube; i++) {
        if (taken[i] == false) {
            if (((src & 1 << i) == (dst & 1 << i))) {
                taken[i] = true;
                idx++;
                continue;
            }
            int current = src ^ (1 << i);
            pathVector.push_back(current);
            taken[i] = true;
            allpath_routing(current, dst, idx + 1);
            taken[i] = false;
            pathVector.pop_back();
        }
    }
    return;
}

void back_track(int idx) {
    if (end_flag == true) {
        return;
    }
    else if (idx > numberOfNodesInHypercube) {
        return;
    }
    else if (idx == numberOfNodesInHypercube) {
        for (int j = 0; j < pathVector.size(); j++) {
            cout << j << " " << pathVector[j] << endl;
        }
        cout<< endl;
        end_flag = true;
        return;
    }
        
    while (1) {
        int randN = rand() % numberOfNodesInHypercube;
        if (taken[randN] == false) {
            if (idx != randN) {
                pathVector.push_back(randN);
                taken[randN] = true;
                back_track(idx + 1);
                if (end_flag == true) {
                    return;
                }
                taken[randN] = false;
                pathVector.pop_back();
                break;
            }
        }
    }
    
}

void perform_random_permutation (){
    while (1) {
    cout << "Enter number of nodes" << endl;
    cin >> numberOfNodesInHypercube;
    if (numberOfNodesInHypercube == -1) {
        break;
    }
    reset();
    back_track(START_NODE);
    }
        
}

int main(int argc, const char * argv[]) {
    
    // a program to generate a random permutation (try './genperm.x 8')
    perform_random_permutation();
    return 0;
    
    int routingType;
    cout << "Enter number of nodes" << endl;
    cin >> numberOfNodesInHypercube;
    if (totalNumberOfLink < 0 && totalNumberOfLink > 16) {
        cout<< "Number of nodes must be less than 16" << endl;
    } else {
        if (numberOfNodesInHypercube != 0 && (numberOfNodesInHypercube & (numberOfNodesInHypercube - 1)) != 0 ) {
            cout<<"Number of nodes must be power of 2"<<endl;
        } else {
            for (int i = 0; i < 18 ; i++) {
                if ((numberOfNodesInHypercube & (1 << i)) > 0) {
                    dimensionOfHyperCube = i;
                    break;
                }
            }
            totalNumberOfLink = 0;
            reset();
            int node = 0;
            nodesQueue.push(node);
            bfsTraversal(numberOfNodesInHypercube, dimensionOfHyperCube);
            printAllTheLinks(dimensionOfHyperCube);
            cout << "For dimension order routing press 1, for all shortest path press 2" << endl;
            cin >> routingType;
            int src, dest;
            while (1) {
                cout<<"Enter src dest : ";
                cin>> src >> dest;
                pathCountForAllPathRouting = 0;
                if (src == -1 || dest == -1) {
                    break;
                }
                if (routingType == 1) {
                    dim_order_routing(src, dest);
                } else {
                    for (int i = 0; i < dimensionOfHyperCube; i++) {
                        taken.push_back(false);
                    }
                    pathVector.push_back(src);
                    allpath_routing(src, dest, 0);
                    cout << endl;
                    pathVector.clear();
                    taken.clear();
                }
            }
        }
    }
return 0;
}



/*
Programming 2: Extend the program in assignment 1 to compute path(s) for (a) dimension order single path routing (lowest dimension first), and (b) all shortest path routing, with the following prototypes:
int dim_order_routing(int src, int dst, int *path); /* return path length */
//int allpath_routing(int src, int dst, int allpath[MAX_PATH][MAXZ_PATH_LEN]); /* return number of paths */
//int allpath_routing(int src, int dst, int allpath[MAX_PATH][MAXZ_PATH_LEN]) {
//    return 0;
//}



