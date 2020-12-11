#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>


//mode 1 returns accumulator, mode 2 returns 1 if correct execution and -1 if incorrect
std::pair<int, std::map<int,int>> run (std::vector<std::string> instructions, int mode){
    unsigned int p = 0;
    int accumulator = 0;
    std::map<int,int> executed;
    while (p < instructions.size()){
        if (executed.find(p) != executed.end()){
            if (mode == 1){
                return std::pair<int, std::map<int,int>>(accumulator, executed);
            } else {
                return std::pair<int, std::map<int,int>>(-1, executed);
            }
        }
        executed[p] = 1;
        std::string instruction = instructions[p];
        std::string operation = instruction.substr(0,3);
        std::string sign = instruction.substr(4,1);
        int argument = stoi(instruction.substr(5));
        if (operation == "nop"){
            p++;
        } else if (operation == "jmp"){
            sign == "+" ? p += argument : p -= argument;
        } else if (operation == "acc"){
            sign == "+" ? accumulator += argument : accumulator -= argument;
            p++;
        }
    }
    if (mode == 1){
        return std::pair<int, std::map<int,int>>(accumulator, executed);
    } else {
        return std::pair<int, std::map<int,int>>(1, executed);
    }
}

int main(){
    std::string in;
    std::ifstream fs("input8.txt");
    std::vector<std::string> instructions;
    while (std::getline(fs, in)){
        instructions.push_back(in);
    }
    std::pair<int, std::map<int, int>> first_run = run(instructions, 1);
    std::cout << "Day 1: " << first_run.first << "\n";
    std::map<int, int> executed = first_run.second;

    for (auto const& q : executed) {
        std::vector<std::string> instuctions_copy = instructions;
        std::string change = instructions[q.first];
        std::string change_op = change.substr(0,3);
        if (change_op == "nop"){
            instuctions_copy[q.first] = "jmp" + change.substr(3);
        } else if (change_op == "jmp"){
            instuctions_copy[q.first] = "nop" + change.substr(3);
        }
        if (run(instuctions_copy, 2).first == 1){
            std::cout << "Day 2: " << run(instuctions_copy, 1).first << "\n";
            break;
        }
    }
}