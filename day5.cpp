#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::bitset<10>> passes_to_bin(std::vector<std::string> passes){
    std::vector<std::bitset<10>> passes_bin;
    for (std::vector<std::string>::const_iterator pass = passes.begin(); pass != passes.end(); ++pass){
        std::bitset<10> seat_num;
        for (int i = 0; i < 7; i++){
            if ((*pass)[i] == 'F'){
                seat_num[9 - i] = 0;
            } else if ((*pass)[i] == 'B'){
                seat_num[9 - i] = 1;
            } else {
                throw;
            }
        }
        for (int i = 7; i < 10; i++){
            if ((*pass)[i] == 'L'){
                seat_num[9 - i] = 0;
            } else if ((*pass)[i] == 'R'){
                seat_num[9 - i] = 1;
            } else {
                throw;
            }
        }
        passes_bin.push_back(seat_num);
    }
    return(passes_bin);
}

bool comp (std::bitset<10> b1, std::bitset<10> b2){
    for (int i = 9; i >= 0; i--){
        if (b1[i] > b2[i]){
            return(false);
        } else if (b1[i] < b2[i]){
            return(true);
        }
    }
    return(false);
}

bool equiv(std::bitset<10> b1, std::bitset<10> b2){ return(b1==b2); }
bool operator> (std::bitset<10> b1, std::bitset<10> b2){ return(comp(b1,b2)); }
bool operator< (std::bitset<10> b1, std::bitset<10> b2){ return(comp(b2,b1)); }

int main(){
    std::string in;
    std::vector<std::string> passes;
    std::ifstream fs ("input5.txt", std::ifstream::in);

    while (std::getline(fs, in)){
        passes.push_back(in);
    }

    std::vector<std::bitset<10>> passes_bin = passes_to_bin(passes);
    std::cout << "Part 1:\n";
    std::cout << (*std::max_element(passes_bin.begin(), passes_bin.end(), comp)).to_ulong();
    std::cout << "\n";

    std::cout << "Part 2:\n";
    std::sort(passes_bin.begin(), passes_bin.end(), comp);
    for (std::vector<std::bitset<10>>::iterator seat = passes_bin.begin(); (seat + 1) != passes_bin.end(); ++seat){
        if ((*seat)[0] == (*(seat + 1))[0]){
            std::cout << ((*seat).to_ulong() + 1) << '\n';
        }
    }
}