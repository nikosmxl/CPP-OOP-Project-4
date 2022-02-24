#include <iostream>
#include "roads.h"
#include <cstdlib>

int section::nsegs;
int toll::nsegs;
int section::percent;

int main(int argc, char *argv[]){

    if (argc != 5){
        std::cout << "Give the right amount of data" << std::endl;
        return 1;
    }

    const unsigned int N = std::stoi(argv[1]);
    const unsigned int NSegs = std::stoi(argv[2]);
    const unsigned int K = std::stoi(argv[3]);
    const unsigned int Percent = std::stoi(argv[4]);

    if (!(N > 0 && NSegs > 0 && K > 0 && Percent > 0 && Percent <= 100)){
        std::cout << "Give the right data" << std::endl;
        return 2;
    }
    
    int* cap = new int[NSegs];
    for (int i = 0 ; i < NSegs ; i++){

        if (i > 2){
            std::cout << "Give the " << (i + 1) << "th section's capacity:" << std::endl;
        }
        else if (i == 0){
            std::cout << "Give the first section's capacity:" << std::endl;
        }
        else if (i == 1){
            std::cout << "Give the second section's capacity:" << std::endl;
        }
        else{
            std::cout << "Give the third section's capacity:" << std::endl;
        }
        
        do{
            
            std::cin >> cap[i];
            if (cap[i] <= 0){
                std::cout << "Try again with the right capacity:" << std::endl;
            }
        } while (cap[i] <= 0);
        
    }

    highway* attica = new highway(NSegs,K,Percent,cap);
    for(int i = 0 ; i < N ; i++){
        attica->operate();
    }

    delete[] cap;
    delete attica;
    return 0;
}