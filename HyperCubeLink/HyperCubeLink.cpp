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
#define FLOW_RATE 1000

using namespace std;

struct node_desc {
    int current;
    int previous;
};

struct link_node_description {
    int src;
    int dest;
    int count_usage = 0;
};

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
vector <vector<int>> shortest_path_vectors;
int all_path_count[LINKSSIZE][35];

void reset() {
    if (taken.size() > 0) {
        taken.clear();
    }
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
}

void reset_for_bfsTraversal_to_find_path () {
    if (taken.size() > 0) {
        taken.clear();
    }
    for (int i = 0; i < SIZE; i++) {
        color[i] = false;
    }
    for (int i = 0; i < SIZE; i++) {
        queueColor[i] = false;
    }
    for (int i = 0; i < SIZE; i++) {
        taken.push_back(false);
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
    cout << endl << endl;
}

#pragma mark Print_All_Link_In_HyperLink

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
        else {
            setColorOfNode(rootNode, color);
        }
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

#pragma mark routing

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

#pragma mark Throughput

//void back_track(int idx) {
//    if (end_flag == true) {
//        return;
//    }
//    else if (idx > numberOfNodesInHypercube) {
//        return;
//    }
//    else if (idx == numberOfNodesInHypercube) {
//        for (int j = 0; j < pathVector.size(); j++) {
//            cout << j << " " << pathVector[j] << endl;
//        }
//        cout<< endl;
//        end_flag = true;
//        return;
//    }
//
//    while (1) {
//        int randN = rand() % numberOfNodesInHypercube;
//        if (taken[randN] == false) {
//            if (idx != randN) {
//                pathVector.push_back(randN);
//                taken[randN] = true;
//                back_track(idx + 1);
//                if (end_flag == true) {
//                    return;
//                }
//                taken[randN] = false;
//                pathVector.pop_back();
//                break;
//            }
//        }
//    }
//
//}
//
//
//void backtrack_reset () {
//    end_flag = false;
//    taken.clear();
//    for (int i = 0; i < SIZE; i++) {
//        taken.push_back(false);
//    }
//}

int linear_search(int search_node, vector<node_desc> each_edge_desc_vector) {
    for (int i = 0; i < each_edge_desc_vector.size(); i++) {
        if (each_edge_desc_vector[i].current == search_node) {
            return i;
        }
    }
    return -1;
}

vector<int> find_path(vector<node_desc> each_edge_desc_vector) {
    vector<int> a_full_path;
    node_desc now = each_edge_desc_vector.back();
    while (now.previous != -1) {
        a_full_path.push_back(now.current);
        int index = linear_search(now.previous, each_edge_desc_vector);
        now = each_edge_desc_vector[index];
    }
    a_full_path.push_back(now.current);
    reverse(a_full_path.begin(), a_full_path.begin() + a_full_path.size());
    return a_full_path;
}

void bfsTraversal_to_find_path(int src, int dest, int numberOfNodesInHypercube, int dimensionOfHyperCube) {
    vector<node_desc> each_edge_desc_vector;
    queue <node_desc> node_desc_queue;
    node_desc src_node;
    src_node.current = src;
    src_node.previous = -1;
    node_desc_queue.push(src_node);
    while (1) {
        if (node_desc_queue.empty() == true) {
            break;
        }
        node_desc rootNode = node_desc_queue.front();
        node_desc_queue.pop();
        if (rootNode.current == dest) {
            each_edge_desc_vector.push_back(rootNode);
            vector<int> a_full_path = find_path(each_edge_desc_vector);
            shortest_path_vectors.push_back(a_full_path);
            break;
        }
        else if (checkColorOfNode(rootNode.current, color) == true) {
            continue;
        }
        else {
            each_edge_desc_vector.push_back(rootNode);
            setColorOfNode(rootNode.current, color);
        }
        for (int i = 0; i < edges[rootNode.current].size(); i++) {
            int now = edges[rootNode.current][i];
            if (checkColorOfNode(now, color) == false && checkColorOfNode(now, queueColor) == false) {
                node_desc new_node_desc;
                new_node_desc.current = now;
                new_node_desc.previous = rootNode.current;
                node_desc_queue.push(new_node_desc);
                setColorOfNode(new_node_desc.current, queueColor);
            }
        }
    }
    return;
}

void save_all_paths() {
    reset();
    int node = 0;
    nodesQueue.push(node);
    for (int i = 0; i < 18 ; i++) {
        if ((numberOfNodesInHypercube & (1 << i)) > 0) {
            dimensionOfHyperCube = i;
            break;
        }
    }
    bfsTraversal(numberOfNodesInHypercube, dimensionOfHyperCube);
    printAllTheLinks(dimensionOfHyperCube);
}

void compute_throughput() {
    save_all_paths();
    
    for (int i = 0; i < LINKSSIZE; i++) {
        for (int j = 0; j < 32; j++) {
            all_path_count[i][j] = 0;
        }
    }
    for (int i = 0; i < shortest_path_vectors.size(); i++) {
        shortest_path_vectors[i].clear();
    }
    shortest_path_vectors.clear();
    for (int i = 0; i < pathVector.size(); i++) {
        int src = i;
        int dest = pathVector[i];
        reset_for_bfsTraversal_to_find_path();
        bfsTraversal_to_find_path(src, dest, numberOfNodesInHypercube, dimensionOfHyperCube);
    }
    
    for (int i = 0; i < shortest_path_vectors.size() ; i++) {
        vector<int> v = (vector<int> )shortest_path_vectors[i];
        for ( int j = 0; j < v.size() - 1; j++) {
            all_path_count[v[j]][v[j+1]]++;
        }
    }
    
    int total_src_count = pathVector.size();

    for (int i = 0; i < total_src_count ; i++) {
        for ( int j = 0; j < shortest_path_vectors[i].size(); j++) {
            cout<< shortest_path_vectors[i][j] << "("<< getBinaryRepresentation(shortest_path_vectors[i][j],dimensionOfHyperCube) << ")" ;
            if (j != (shortest_path_vectors[i].size() -1)) {
                cout << "-->";
            }
        }
        cout<<endl;
    }
    /// Aggregate Throughput ///
    
    int aggregate_througput = 0;
    vector<int>path_vector_corresponding_thorughput;
    for (int i = 0; i < pathVector.size(); i++) {
        path_vector_corresponding_thorughput.push_back(FLOW_RATE);
    }
    
    for (int i = 0; i < total_src_count ; i++) {
        for ( int j = 0; j < shortest_path_vectors[i].size() - 1; j++) {
            int  first_node = shortest_path_vectors[i][j];
            int second_node = shortest_path_vectors[i][j+1];
            path_vector_corresponding_thorughput[i] = min(path_vector_corresponding_thorughput[i], FLOW_RATE/all_path_count[first_node][second_node]);
        }
        aggregate_througput += path_vector_corresponding_thorughput[i];
    }
    cout << endl << "Aggregate throughput: " << aggregate_througput << "Mbps" << endl << endl;
    
    for (int i = 0; i < pathVector.size() ; i++) {
        int src = i;
        int dest = pathVector[i];
        cout << "flow[" << i << "]" << ": " << src << "->" << dest << ", " << "rate" << " = " << path_vector_corresponding_thorughput[i] << "Mbps" <<endl;
    }
    cout << endl << endl;
    
}

int get_a_random_num_from (vector<int> & v) {
    int n = v.size();
    int index = rand() % n;
    int num = v[index];
    swap(v[index], v[n - 1]);
    v.pop_back();
    return num;
}

void perform_random_permutation () {
    
    bool wrong_permutation = false;
    vector<int> numbers_to_permute_randomly;
    srand(time(NULL));
    while (1) {
        for (int i = 0; i < numberOfNodesInHypercube; i++) {
            numbers_to_permute_randomly.push_back(i);
        }
        while (numbers_to_permute_randomly.size() > 0) {
            int num = get_a_random_num_from(numbers_to_permute_randomly);
            pathVector.push_back(num);
        }
        for (int i = 0; i < pathVector.size(); i++) {
            if (i == pathVector[i]) {
                wrong_permutation = true;
                pathVector.clear();
                break;
            }
        }
        if (wrong_permutation == false) {
            break;
        }
        wrong_permutation = false;
    }
}


void perform_random_permutation_and_Thorughput () {
    while (1) {
        cout << "Enter number of nodes" << endl;
        cin >> numberOfNodesInHypercube;
        if (numberOfNodesInHypercube == -1) {
            break;
        }
        else if (numberOfNodesInHypercube != 0 && (numberOfNodesInHypercube & (numberOfNodesInHypercube - 1)) != 0 ) {
            cout<<"Number of nodes must be power of 2"<<endl;
            continue;
        }
        perform_random_permutation();
        compute_throughput();
        pathVector.clear();
    }
}

int main(int argc, const char * argv[]) {
    
    // a program to generate a random permutation (try './genperm.x 8')
    perform_random_permutation_and_Thorughput();
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



