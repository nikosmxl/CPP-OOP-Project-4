#include <iostream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include "roads.h"

//Vehicle
vehicle::vehicle(int index, int dest) : section_index(index), ready(false), destination(dest), node_index(index){

    std::cout << "A vehicle was constructed" << std::endl;
    
}

vehicle::~vehicle(void){
    std::cout << "A vehicle was destructed" << std::endl;
}

bool vehicle::is_ready() const{
    return ready;
}

int vehicle::get_destination() const {
    return destination;
}

void vehicle::get_ready(){
    ready = true;
}

void vehicle::get_not_ready(){
    ready = false;
}

void vehicle::set_index(int index){
    section_index = index;
    node_index = index;
}

//Toll
toll::toll(int index_given, int nsegs_given) : capacity(3*MAX_VEHS_INSERT_IN_TOLL), index(index_given){
    //Initializations
    nsegs = nsegs_given;
    num_of_vehicles = rand() % (MAX_VEHS_INSERT_IN_TOLL + 1);
    vehicles = new vehicle*[capacity];
    for(int i = 0; i < num_of_vehicles; i++){
        vehicles[i] = new vehicle(-1, (index + rand() % (nsegs - index)));
    }

    std::cout << "A toll was constructed" << std::endl;
}

toll::~toll(){
    for(int i = 0; i < num_of_vehicles; i++){
        delete vehicles[i];
    }
    delete[] vehicles;
}

void toll::my_realloc(void*** m,int oldsize,int newsize){
    void** temp = new void*[newsize];
    for(int i = 0; i < newsize && i < oldsize; i++){
        temp[i] = (*m)[i];
    }
    delete[] *m;
    *m = temp;
    return;
}

void toll::fix_matrix_reduce(){

    if (num_of_vehicles == 1){
        vehicles[0] = NULL;
        return;
    }
    vehicles[0] = vehicles[num_of_vehicles-1];
    vehicles[num_of_vehicles-1] = NULL;
    
}

void toll::operate(){
    
    int num_of_new_vehs = rand() % MAX_VEHS_INSERT_IN_TOLL;
    if (num_of_vehicles+num_of_new_vehs > capacity){
        int new_capacity = capacity*2;
        while( new_capacity < num_of_vehicles+num_of_new_vehs ){
            new_capacity *= 2;
        }
        my_realloc( ((void***)(&vehicles)) ,capacity,new_capacity);
        capacity = new_capacity;
    }
    for(int i = num_of_vehicles; i < num_of_vehicles+num_of_new_vehs; i++){
        vehicles[i] = new vehicle(-1, (index + rand() % (nsegs - index)));
    }
    num_of_vehicles += num_of_new_vehs;
}

int toll::give_num_of_vehs() const{
    return num_of_vehicles;
}

vehicle* toll::give_a_veh(int& placed, int k){
    
    if (num_of_vehicles == 0){
        return NULL;
    }

    if (placed >= k){
        return NULL;
    }
    
    vehicle* for_return = vehicles[0];
    
    fix_matrix_reduce();
    num_of_vehicles--;
    placed++;
    if (num_of_vehicles > 0 && num_of_vehicles <= capacity/4){
        int new_capacity = capacity/2;
        my_realloc( ((void***)(&vehicles)) ,capacity,new_capacity);
        capacity = new_capacity;
    }
    return for_return;
}

//Electronic toll
electronic_toll::electronic_toll(int index_given, int nsegs_given) : toll(index_given, nsegs_given){
    std::cout << "An electornic toll was constructed" << std::endl;
}

//Collector toll
collector_toll::collector_toll(int index_given, int nsegs_given) : toll(index_given, nsegs_given){
    std::cout << "A collector toll was constructed" << std::endl;
}

//Node
int node::give_num_of_vehs() const{
    int sum = 0;
    for(int i=0; i < num_of_electronics+num_of_collectors; i++){
        sum += tolls[i]->give_num_of_vehs();
    }
    return sum;
}

node::node(const char* name,int kk, int index, int nsegs): delays(false), k(kk){

    name_id = new char[strlen(name)+1];
    strcpy(name_id,name);

    num_of_collectors = rand() % max_tolls_category + 1;        // 1 - 5
    num_of_electronics = rand() % max_tolls_category + 1;       // 1 - 5
    current_toll_with_priority = 0;
    tolls = new toll*[num_of_electronics + num_of_collectors];
    for(int i = 0; i < num_of_electronics + num_of_collectors; i++){
        if( i < num_of_electronics ){
            tolls[i] = new electronic_toll(index, nsegs);
        }
        else{
            tolls[i] = new collector_toll(index, nsegs);
        }
    }
    std::cout << "A node was constructed" << std::endl;
}

node::~node(){
    for(int i = 0; i < num_of_collectors+num_of_electronics; i++){
        delete tolls[i];
    }

    delete[] tolls;
    delete[] name_id;
}

bool node::give_delays() const{
    return delays;
}

vehicle** node::give_vehs_to_the_section(int remaining_vehs_to_full,int& vehs_to_enter){
    if( remaining_vehs_to_full <= 0 ){
        return NULL;
    }

    vehicle** return_vehs = new vehicle*[remaining_vehs_to_full];

    int toll_not_giving_car = 0;
    vehs_to_enter = 0;
    int j = current_toll_with_priority;
    int countercollector = 0;           // How many cars did collector tolls give?
    int counterelectronic = 0;          // How many cars did electronic tolls give?

    while (vehs_to_enter < remaining_vehs_to_full && toll_not_giving_car < num_of_electronics+num_of_collectors){
        vehicle* v;
        if (j < num_of_electronics){    // The first num_of_electronics tolls in the array are electronic
            v = tolls[j]->give_a_veh(counterelectronic, 2*k);
        }
        else{
            v = tolls[j]->give_a_veh(countercollector, k);
        }

        if (v != NULL){                 // If toll has car to give
            toll_not_giving_car = 0;
            return_vehs[vehs_to_enter++] = v;
        }
        else{                           // If toll has NO car to give
            toll_not_giving_car++;
        }

        j = (j+1)%(num_of_collectors+num_of_electronics);
    }

    current_toll_with_priority = j;         // After last j's increase this is the starting toll next time

    if (give_num_of_vehs() != 0){            // There are cars waiting 
        delays = true;
    }
    else{                                   // No cars left
        delays = false;
    }

    if (counterelectronic == 2*k && countercollector == k){
        k++;
    }
    else if (k > 1){
        k--;
    }

    if (vehs_to_enter == 0 ){
        delete[] return_vehs;
        return NULL;
    }

    return return_vehs;
}

void node::operate(){
    for (int i = 0 ; i < num_of_collectors + num_of_electronics ; i++){
        tolls[i]->operate();
    }
}

//Section
section::section(int per, int K, int capacity, section* next,int index_given, int segsnum) : delays(false), index(index_given), capacity_veh(capacity){

    nsegs = segsnum;
    percent = per;

    std::string name = "Node" + std::to_string(index);
    const char* charname = name.c_str();    // "Node0" , "Node1" ...

    insertion = new node(charname, K, index, nsegs);
    capacity_veh = capacity;
    next_section = next;
    previous_section = NULL;
    num_of_vehicles = rand() % (capacity_veh + 1);
    vehicles = new vehicle*[capacity_veh];

    for(int i = 0; i < capacity_veh; i++){
        if (i < num_of_vehicles){
            vehicles[i] = new vehicle(index, (index + rand() % (nsegs - index)));
        }
        else{
            vehicles[i] = NULL;
        }
    }
    std::cout << "A section was constructed" << std::endl;
}

section::~section(){
    for(int i = 0; i < num_of_vehicles; i++){
        delete vehicles[i];
    }
    delete[] vehicles;
    delete insertion;
}

void section::set_previous_section(section* prev){
    previous_section = prev;
}

int section::get_no_of_vehicles() const {
    return num_of_vehicles;
}

void section::exit(){
    int a = 0;
    if( num_of_vehicles != 0 ){
        for(int i = 0; i < num_of_vehicles; i++){
            if( vehicles[i]->is_ready() == true && (vehicles[i]->get_destination() == index || next_section == NULL) ){
                delete vehicles[i];
                vehicles[i] = NULL;
                a++;
            }
        }
        num_of_vehicles -= a;
        shift( ((void**)(vehicles)) ,capacity_veh);
    }
}

void section::pass(){
    int n=0;
    delays = false;
    for(int i = 0; i < num_of_vehicles; i++){
        if( vehicles[i]->is_ready() == true && vehicles[i]->get_destination() != index ){
            if( next_section->capacity_veh > next_section->num_of_vehicles ){
                next_section->take_vehicle(vehicles[i]);
                vehicles[i] = NULL;
                n++;
            }
            else{
                delays = true;
                break;
            }
        }
    }

    num_of_vehicles -= n;
    shift( ((void**)(vehicles)) ,capacity_veh);  
}

void section::take_vehicle(vehicle* v){
    vehicles[num_of_vehicles] = v;
    vehicles[num_of_vehicles]->set_index(index);
    vehicles[num_of_vehicles]->get_not_ready();
    num_of_vehicles++;
}

void section::shift(void** m,int msize){ // τα παμε ολα οσα δεν ειναι ναλλ μπροστα

    for(int i = 0; i < msize; i++){
        if( m[i] == NULL ){
            for(int j = i+1; j < msize; j++){
                if( m[j] != NULL ){
                    m[i] = m[j];
                    m[j] = NULL;
                    break;
                }
            }
        }
    }
}

void section::enter(){
    if( previous_section != NULL ){
        previous_section->pass();
    }
    
    int remaining_vehs_to_full = capacity_veh - num_of_vehicles;
    int num_of_new_vehs;
    vehicle** vv = insertion->give_vehs_to_the_section(remaining_vehs_to_full, num_of_new_vehs);
    
    if( vv != NULL ){
        for(int i = num_of_vehicles; i < num_of_vehicles+num_of_new_vehs; i++){
            vehicles[i] = vv[i-num_of_vehicles];
            vehicles[i]->set_index(index);
        }
        num_of_vehicles += num_of_new_vehs;
        delete[] vv;
    }

    //Prints
    if (insertion->give_delays() == true ){
        std::cout << "Delays on entrance after node number: " << index + 1 << std::endl;      // Nodes' index are the same with their sections. + 1 because we start from 0.
    }

    if (delays == true){
        std::cout << "Delays after node number: " << index + 1 << std::endl;
    }

    if (delays == false && insertion->give_delays() == false){
        std::cout << "Hold security distances after node number: " << index + 1 << std::endl;
    }
}

void section::operate(){
    insertion->operate();
    int num_of_readys = percent*num_of_vehicles/100;

    for(int i = 0; i < num_of_readys ; i++){
        vehicles[i]->get_ready();
    }
}

//Highway
highway::highway(int N, int K, int percent, int* cap) : nsegs(N) {
    unsigned long currtime = time(NULL);
    srand( (unsigned int) currtime);
    num_of_vehicles = 0;

    sections = new section*[nsegs];
    section* afterend = NULL;

    for (int i = nsegs - 1 ; i >= 0 ; i--){
        sections[i] = new section(percent, K, cap[i], afterend, i, nsegs);
        afterend = sections[i];
        num_of_vehicles += sections[i]->get_no_of_vehicles();
    }

    section* previous_section = NULL;

    for (int i = 0 ; i < nsegs ; i++){
        sections[i]->set_previous_section(previous_section);
        previous_section = sections[i];
    }
    std::cout << "A highway was constructed" << std::endl;
}

highway::~highway(){
    for (int i = nsegs - 1 ; i >= 0 ; i--){
        delete sections[i];
    }
    delete[] sections;
}

void highway::operate(){
    num_of_vehicles = 0;

    for (int i = nsegs - 1 ; i >= 0 ; i--){
        sections[i]->operate();
    }

    for (int i = nsegs - 1 ; i >= 0 ; i--){
        sections[i]->exit();
        sections[i]->enter();
        num_of_vehicles += sections[i]->get_no_of_vehicles();
    }
}