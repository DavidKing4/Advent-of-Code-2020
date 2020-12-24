#include <iostream>
#include <fstream>
#include <map>
using namespace std;


int main(){
    string in;
    string instr;
    string subinst;
    ifstream fs ("input24.txt");
    map<pair<int,int>,int> floor;
    while (getline(fs,in)) {
        int x = 0;
        int y = 0;
        for (uint16_t i = 0; i < in.length(); i++) {
            instr = in.substr(i, 1);
            if (instr == "e") {
                x++;
            } else if (instr == "w") {
                x--;
            } else if (instr == "n") {
                i++;
                y++;
                subinst = in.substr(i, 1);
                if (subinst == "e") {
                    x++;
                }
            } else if (instr == "s") {
                i++;
                y--;
                subinst = in.substr(i, 1);
                if (subinst == "w") {
                    x--;
                }
            }
        }
        if (floor.find(pair<int,int>(x,y)) == floor.end()){
            floor[pair<int,int>(x,y)] = 1;
        } else {
            floor[pair<int,int>(x,y)] = floor[pair<int,int>(x,y)] == 1 ? 0 : 1;
        }
    }
    int black = 0;
    for (auto p : floor) {
        black += p.second;
    }
    cout << "Part 1: " << black << "\n";

    pair<int,int> d[6] =    {
                            pair<int,int>(1,0),
                            pair<int,int>(-1,0),
                            pair<int,int>(0,1),
                            pair<int,int>(1,1),
                            pair<int,int>(-1,-1),
                            pair<int,int>(0,-1)
                            };

    for (int i = 0; i < 100; i++) {
        map<pair<int,int>,int> floor_next;
        for (auto const q : floor) {
            int x = q.first.first;
            int y = q.first.second;
            black = 0;
            for (int j = 0; j < 6; j++) {
                pair<int,int> x1y1 (x + d[j].first, y + d[j].second);
                if (floor.find(x1y1) != floor.end()) {
                    if ((*floor.find(x1y1)).second == 1) {
                        black++;
                    }
                }
            }
            if (q.second == 1) {
                if (black > 0 && black < 3){
                    floor_next[pair<int,int>(x, y)] = 1;
                }
            } else {
                if (black == 2){
                    floor_next[pair<int,int>(x, y)] = 1;
                }
            }

            for (int k = 0; k < 6; k++) {
                int x = q.first.first + d[k].first;
                int y = q.first.second + d[k].second;
                black = 0;
                for (int j = 0; j < 6; j++) {
                    // cout << k << "\t" << j << "\n";
                    pair<int,int> x1y1 (x + d[j].first, y + d[j].second);
                    if (floor.find(x1y1) != floor.end()) {
                        if ((*floor.find(x1y1)).second == 1) {
                            black++;
                        }
                    }
                }
                if ((*floor.find(pair<int,int>(x, y))).second == 1) {
                    if (black > 0 && black < 3){
                        floor_next[pair<int,int>(x, y)] = 1;
                    }
                } else {
                    if (black == 2){
                        floor_next[pair<int,int>(x, y)] = 1;
                    }
                }
            }
        }
        black = 0;
        for (auto p : floor_next) {
            black += p.second;
        }
        cout << "Day " << i + 1 << "\t" << black << "\n";
        floor = floor_next;
    }
    cout << "Part 2: " << black << "\n";
}