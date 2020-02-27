//
//  AFlowLevelSimulator.cpp
//  HyperCubeLink
//
//  Created by Fatema on 2/21/20.
//  Copyright © 2020 Liza. All rights reserved.
//

/************* Preprocessors and Macros ***********/

#include <stdio.h>
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
#include <iosfwd>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <stdlib.h>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <iomanip> // for std::setprecision()


#define MSG_NO 500
using namespace std;

/*************  Structs and enums **********/

struct event_desc {
    int msg_id;
    int src_id;
    int dest_id;
    float msg_size;
    float msg_sent;
    float start_time;
    float finishing_time;
    float flow_rate;
    bool finished;
    int max_link_usage_cnt;
    float total_time_from_the_beginning; // time spend from the very beginning of the first event
    float more_time_needed_to_finish;
    std::vector<int> path_vector;
} ;


/************* Constant Variables **************/

const float FLOW_RATE_ = 1;
int cnt_ = -1;
static ofstream myfile;
static vector<int>pathVector;
static int dimensionOfHyperCube = 0;
static int numberOfNodesInHypercube = 0;
int link_count[MSG_NO][MSG_NO];
int finish_list[500];


/************* Files Variables **************/

ifstream simFile;
ofstream simFileOutput;
static ofstream dimensionOrpathFile;



/************* Vectors  **************/

auto compare = [](event_desc lhs, event_desc rhs)
{
    return lhs.start_time >= rhs.start_time;
};

vector<event_desc>save_event_lists;

vector<event_desc>active_events;

vector<string>input_lines;

std::priority_queue<event_desc, std::vector<event_desc>, decltype(compare)> event_lists(compare);

std::priority_queue<event_desc, std::vector<event_desc>, decltype(compare)> event_lists_with_finishing_time(compare);



/************* Function Prototypes  **************/

void implement_simulation();
static void set_number_of_nodes_hypercube(int nodes);
int get_dimension_of_hypercube();
void print_finish_time();
void parse_file_data();
void open_file_for_simulation();
void start_simulation();
void update_finish_list_for_remaining_active_events( vector <event_desc> & active_events );
void calculate_current_link_count( vector <event_desc> & active_events );
void reset_link_count();
void update_Events_after_recieving_new_event (vector <event_desc> & active_events );
void update_Events_maximum_link_Usage_cnt( vector <event_desc> & active_events );
static vector<int> dim_order_routing_for_simulation(int src, int dst);
static string getBinaryRepresentation(int num, int dimensionOfHyperCube);




#pragma mark - Simulation

static int number_of_nodes = 64;
static int dimension_of_hypercube;


int main(int argc, const char * argv[]) {

    implement_simulation();
    return 0;
    
}



void simulation_reset() {
    
}



template <class Container> void split(const std::string& str, Container& cont)
{
    std::istringstream iss(str);
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(),
              std::back_inserter(cont));
}


static string getBinaryRepresentation(int num, int dimensionOfHyperCube) {
    string s = "";
    for (int i = dimensionOfHyperCube-1; i > -1; i--) {
        s += (1<<i & num)? "1": "0";
    }
    return s;
}


#pragma mark routing
static vector<int> dim_order_routing_for_simulation(int src, int dst) {
    if ((src < 0 || src > (numberOfNodesInHypercube - 1)) || (dst < 0 || dst > (numberOfNodesInHypercube - 1))) {
        cout << "out of range" <<endl;
    }
    int current = src;
    int pathLength = 0;
    pathVector.clear();
    pathVector.push_back(src);
    for (int i = 0; i < 17; i++) {
        if (current == dst) {
            break;
        }
        if ((current & 1<<i) != (dst & 1 << i)) {
            current = current^(0x01 << i);
            pathVector.push_back(current);
            pathLength++;
        }
    }
    myfile<<"Path from "<< src << " to "<< dst << ":" <<" ";
    
    for (int j = 0;j < pathVector.size(); j++) {
        myfile << pathVector[j] << "(" << getBinaryRepresentation(pathVector[j], dimensionOfHyperCube) << ")";
        if (j == (pathVector.size() - 1)) {
            myfile << endl;
        } else {
            myfile<<"->";
        }
    }
    myfile<< endl <<endl;
    return pathVector;
}


void update_Events_maximum_link_Usage_cnt( vector <event_desc> & active_events ) {
    
    
    for ( int i =0; i <  active_events.size(); i++) {
        active_events[i].max_link_usage_cnt = -1;
        event_desc event = active_events[i];
        for (int j = 0; j < event.path_vector.size() - 1; j++) {
            int node1 = event.path_vector[j];
            int node2 = event.path_vector[j+1];
            active_events[i].max_link_usage_cnt = max(active_events[i].max_link_usage_cnt, link_count[node1][node2]);
        }
    }
}

/************ update all the event after PUSHING new event ***********/

void update_Events_after_recieving_new_event (vector <event_desc> & active_events ) {
    for ( int i = 0; i <  active_events.size(); i++) {
        
        active_events[i].flow_rate = (float) FLOW_RATE_ / (active_events[i].max_link_usage_cnt) ;
        active_events[i].more_time_needed_to_finish = active_events[i].msg_size  / active_events[i].flow_rate;
    }
}


void reset_link_count() {
    for (int i = 0; i < MSG_NO; i++) {
        for (int j = 0; j < MSG_NO; j++) {
            link_count[i][j] = 0;
        }
    }
}


void calculate_current_link_count( vector <event_desc> & active_events ) {
    for ( int i = 0; i <  active_events.size(); i++) {
        event_desc event = active_events[i];
        for (int j = 0; j < event.path_vector.size() - 1; j++) {
            int node1 = event.path_vector[j];
            int node2 = event.path_vector[j+1];
            link_count[node1][node2] += 1;
        }
    }
    // just for print //
    for ( int i = 0; i <  active_events.size(); i++) {
        event_desc event = active_events[i];
        for (int j = 0; j < event.path_vector.size() - 1; j++) {
            int node1 = event.path_vector[j];
            int node2 = event.path_vector[j+1];
            if (node1 == 0 && node2 == 4 && link_count[node1][node2] > 0) {
                //printf("id_=%d, count=%d, src_id=%d, dest_id=%d\n",cnt_, link_count[node1][node2], event.src_id, event.dest_id);
            }
        }
        
    }
}


void update_finish_list_for_remaining_active_events( vector <event_desc> & active_events ) {
    // For all the remaining events after the simulations
    
    update_Events_after_recieving_new_event(active_events);
    for ( int i = 0; i <  active_events.size(); i++) {
        
        active_events[i].finishing_time = (float)active_events[i].total_time_from_the_beginning +
        active_events[i].more_time_needed_to_finish ;
        active_events[i].msg_sent += active_events[i].msg_size;
        active_events[i].msg_size = 0;
        active_events[i].finished = true;
        event_lists_with_finishing_time.push(active_events[i]);
    }
}

void find_out_all_same_start_time_events (vector <event_desc> & active_events, event_desc newEvent ) {
    while (!event_lists.empty()) {
        struct event_desc curr_event = event_lists.top();
        if (curr_event.start_time == newEvent.start_time) {
            event_lists.pop();
            active_events.push_back(curr_event);
        }
        else {
            break;
        }
    }
}


void start_simulation() {
    
    cout << "Initially total event lists is " << event_lists.size() << endl;
    event_desc newEvent, evnt;
    float event_time = 0.0;
    while (!event_lists.empty()) {
        cnt_++;
        newEvent = event_lists.top();
        event_lists.pop();
        
        if (newEvent.src_id == newEvent.dest_id) {
            newEvent.msg_size = 0;
            newEvent.finished = true;
            newEvent.finishing_time = newEvent.start_time;
            event_lists_with_finishing_time.push(newEvent);
            continue;
        } else {
            active_events.push_back(newEvent);
        }
        
        
        event_time = newEvent.start_time;
        
        for ( int i = 0; i <  active_events.size(); i++) {
            
            evnt = active_events[i];
            // more_time_needed_to_finish = the time needed at current flow rate (active_events[i].flow_rate) to finish
            float time_to_consider = event_time - evnt.total_time_from_the_beginning;
            if ( evnt.more_time_needed_to_finish <= time_to_consider ) {
                active_events[i].finishing_time = active_events[i].total_time_from_the_beginning +
                active_events[i].more_time_needed_to_finish;
                active_events[i].msg_sent += active_events[i].msg_size;
                active_events[i].msg_size = 0;
                active_events[i].finished = true;
                event_lists_with_finishing_time.push(active_events[i]);
                swap(active_events[i], active_events[active_events.size()-1]);
                active_events.pop_back();
            } else {
                
                active_events[i].total_time_from_the_beginning += time_to_consider;
                float current_sending_msg = time_to_consider * active_events[i].flow_rate;
                active_events[i].msg_sent += current_sending_msg;
                active_events[i].msg_size -= current_sending_msg;
            }
        }
        
        
        /***** finding out all the same start time events *****/
        find_out_all_same_start_time_events( active_events, newEvent );
        
        /******* Reset link count for all the active events  ******/
        reset_link_count();
        
        /****** Calculate Current Link Count ********/
        calculate_current_link_count(active_events);
        
        /******** Update the maximum link usage count *********/
        update_Events_maximum_link_Usage_cnt( active_events);
        
        /********** Now update active events flow rate and calculate the time to need to finish *******/
        update_Events_after_recieving_new_event (active_events );
    }
    update_finish_list_for_remaining_active_events( active_events );
    
}



void open_file_for_simulation() {
    
    myfile.open ("/Volumes/Liza Disk - Data/FSU/ResearchStudiesFirstYear/HyperCubeLink/output_perm.txt");
    myfile << "Writing this to a file.\n";
    simFile.open("/Volumes/Liza Disk - Data/FSU/ResearchStudiesFirstYear/HyperCubeLink/input.txt");
    simFileOutput.open("/Volumes/Liza Disk - Data/FSU/ResearchStudiesFirstYear/HyperCubeLink/my_output.txt");
    dimensionOrpathFile.open("/Volumes/Liza Disk - Data/FSU/ResearchStudiesFirstYear/HyperCubeLink/path.txt");
    
    string line;
    
    if (simFile.is_open()) {
        while (getline(simFile, line)) {
            if (line[0] != -1) {
                input_lines.push_back(line);
            }
        }
        simFile.close();
    }
}


void parse_file_data() {
    
    event_desc event;
    
    for ( int i = 0; i <  input_lines.size() ; i++) {
        string str = input_lines[i];
        std::vector<std::string> words;
        split(str, words);
        if (words[0] == "-1") {
            break;
        }
        event.msg_id = i;
        event.src_id = atoi(words[0].c_str());
        event.dest_id = atoi(words[1].c_str());
        event.msg_size = atof(words[2].c_str()); // message size is in bits now
        event.msg_sent = 0;
        event.start_time = atof(words[3].c_str());
        event.finishing_time = -1;
        event.flow_rate = FLOW_RATE_;
        event.finished = false;
        event.max_link_usage_cnt = -1;
        event.total_time_from_the_beginning = event.start_time;
        event.more_time_needed_to_finish = event.msg_size / event.flow_rate;
        simulation_reset();
        vector<int>path = dim_order_routing_for_simulation(event.src_id, event.dest_id);
        event.path_vector = path;
        event_lists.push(event);
        save_event_lists.push_back(event);
    }
}


void print_finish_time() {
    
    cout << "After simulation total event size is " << event_lists_with_finishing_time.size() << endl;
    cout << "Print finish time " << endl << endl;
    
    while (!event_lists_with_finishing_time.empty()) {
        event_desc event = event_lists_with_finishing_time.top();
        event_lists_with_finishing_time.pop();
        cout << event.src_id << " "<< event.dest_id << " " << event.msg_size << " " << event.start_time << " " ;
        cout << event.finishing_time << endl;
        simFileOutput << event.src_id << " "<< event.dest_id << " " << event.msg_size << " " << event.start_time << " " << event.finishing_time << endl;
    }
}

int get_dimension_of_hypercube(){
    for (int i = 0; i < 18 ; i++) {
        if ((numberOfNodesInHypercube & (1 << i)) > 0) {
            dimensionOfHyperCube = i;
            break;
        }
    }
    return dimensionOfHyperCube;
}


static void set_number_of_nodes_hypercube(int nodes) {
    numberOfNodesInHypercube = nodes;
}


void implement_simulation() {
    
    open_file_for_simulation();
    set_number_of_nodes_hypercube(number_of_nodes);
    dimensionOfHyperCube = get_dimension_of_hypercube();
    parse_file_data();
    for(int i = 0; i < save_event_lists.size(); i++) {
        dimensionOrpathFile << i << ":  ";
        for (int j = 0; j < save_event_lists[i].path_vector.size(); j++) {
            dimensionOrpathFile << save_event_lists[i].path_vector[j] << ", ";
        }
        dimensionOrpathFile << endl;
    }
    start_simulation();
    print_finish_time();
}

