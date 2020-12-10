#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

std::map<std::string, bool> counted;

int can_contain(int count, std::string bag, std::map<std::string, std::vector<std::string>> parents){
    if (parents.find(bag) == parents.end()) return count;
    std::vector<std::string> contained_in = parents[bag];
    for (uint16_t i = 0; i < contained_in.size(); i++){
        if (counted.find(contained_in[i]) == counted.end()){
            counted.insert(std::pair<std::string, bool>(contained_in[i],1));
            count = can_contain(count + 1, contained_in[i], parents);
        }
    }
    return count;
}

unsigned long int contains_amount(unsigned long int count, std::string bag, std::map<std::string, std::map<std::string,int>> children){
    if (children.find(bag) == children.end()) return 0;
    std::map<std::string,int> direct_children = children[bag];
    for (auto const& child : direct_children){
        count += child.second * (contains_amount(0, child.first, children) + 1);
    }
    return count;
}

int main(){
    std::string in;
    std::ifstream fs ("input7.txt");
    std::map<std::string, std::map<std::string,int>> children;
    std::map<std::string, std::vector<std::string>> parents;

    while (std::getline(fs,in)){
        if (in.find("no other") != std::string::npos) continue;
        int contain = in.find(" bags contain ");
        std::string outer = in.substr(0, contain);
        std::string inner = in.substr(contain + 14);
        std::map<std::string, int> contents;

        while (inner.find(" bag") != std::string::npos){
            int bag_pos = inner.find(" bag");
            std::string bag = inner.substr(0, bag_pos);
            contents.insert(std::pair<std::string, int>(bag.substr(2), bag[0] - 48));
            if (inner.find(", ") != std::string::npos){
                inner = inner.substr(inner.find(", ") + 2);
            } else {
                inner = "";
            }
            if (parents.find(bag.substr(2)) == parents.end()){
                std::vector<std::string> contained_in {outer};
                parents.insert(std::pair<std::string, std::vector<std::string>>(bag.substr(2), contained_in));
            } else {
                parents[bag.substr(2)].push_back(outer);
            }
        }
        children.insert(std::pair<std::string, std::map<std::string, int>>(outer, contents));
    }

    std::cout << "Part 1:\t" << can_contain(0, "shiny gold", parents) << "\n";
    std::cout << "Part 2:\t" << contains_amount(0, "shiny gold", children) << "\n";
}