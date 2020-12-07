#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <string.h>
#include <vector>


int main(){
    std::string in;
    std::ifstream fs ("input6.txt", std::ifstream::in);
    int questions1[26] = {};
    int questions2[26] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    int total1 = 0;
    int total2 = 0;

    while (std::getline(fs, in)){
        int person[26] = {};
        if (in.length() == 0){
            total1 += std::accumulate(std::begin(questions1),std::end(questions1), 0);
            total2 += std::accumulate(std::begin(questions2),std::end(questions2), 0);
            for (int i = 0; i < 26; i++){
                questions1[i] = 0;
                questions2[i] = 1;
            }
        } else {
            for (uint16_t i = 0; i < in.length(); i++){
                questions1[in[i] - 97] = 1;
                person[in[i] - 97] = 1;
            }
            for (int i = 0; i < 26; i++){
                if (person[i] == 0){
                    questions2[i] = 0;
                }
            }
        }
    }
    total1 += std::accumulate(std::begin(questions1),std::end(questions1), 0);
    std::cout << "Part 1:\n" << total1 << "\n";
    std::cout << "Part 2:\n" << total2 << "\n";
}