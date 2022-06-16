#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

#define DEBUG true

using namespace std;


struct DumboMap {
    vector<vector<int>> dmap;

    DumboMap(vector<string> stringmap){    
        for(string s : stringmap){
            vector<int> row;
            for(char c : s){
                row.push_back(c - '0');
            }
            dmap.push_back(row);
        }
    }
    int at(int x, int y){
        if(x >= 0 && x < getWidth() && y >= 0 && y < getHeight()){
            return dmap.at(y).at(x);
        }
        return -1;
    }
    void set(int x, int y, int value){
        if(x >= 0 && x < getWidth() && y >= 0 && y < getHeight()){
            dmap[y][x] = value;
        }
    }
    vector<vector<int>>::size_type getHeight(){
        return dmap.size();
    }
    vector<int>::size_type getWidth(){
        if(dmap.size() > 0){
            return dmap.at(0).size();    
        }
        else {
            return 0;
        }
    }
    void print(){
        for(auto y = 0; y < getHeight(); y++){
            for(auto x = 0; x < getWidth(); x++){
                cout << at(x,y);
            }
            cout << endl;
        }
    }
    bool someReadyToFlash(vector<pair<int, int>> & alreadyFlashed){
        for(auto y = 0; y < getHeight(); y++){
            for(auto x = 0; x < getWidth(); x++){
                // some are ready to flash and haven't already flashed
                if(at(x, y) > 9 && find(alreadyFlashed.begin(), alreadyFlashed.end(), make_pair(x,y))== alreadyFlashed.end()){
                    return true;
                    
                }
            }
        }
        return false;
    }
    int step(){
        // increment all levels by 1
        for(auto y = 0; y < getHeight(); y++){
            for(auto x = 0; x < getWidth(); x++){
                set(x,y,at(x,y)+1);
            }
        }
        // for all levels > 9, increment surrounding levels by 1
        vector<pair<int, int>> flashed;
        while(someReadyToFlash(flashed)){
            for(auto y = 0; y < getHeight(); y++){
                for(auto x = 0; x < getWidth(); x++){
                    // ready to flash and hasn't already flashed
                    if(at(x,y) > 9 && find(flashed.begin(), flashed.end(), make_pair(x,y)) == flashed.end()){
                        flashed.push_back(make_pair(x, y));
                        // U D L R
                        set(x-1,y,at(x-1,y)+1);
                        set(x+1,y,at(x+1,y)+1);
                        set(x,y-1,at(x,y-1)+1);
                        set(x,y+1,at(x,y+1)+1);

                        // diagonal
                        set(x-1,y-1,at(x-1,y-1)+1);
                        set(x+1,y+1,at(x+1,y+1)+1);
                        set(x+1,y-1,at(x+1,y-1)+1);
                        set(x-1,y+1,at(x-1,y+1)+1);
                    }
                }
            }
        }
        
        // set all levels > 9 to 0
        for(auto y = 0; y < getHeight(); y++){
            for(auto x = 0; x < getWidth(); x++){
                if(at(x,y) > 9){
                    set(x,y,0);
                }
            }
        }
        return flashed.size(); //return number that flashed
    }
    
};

DumboMap buildInput(string filename){
    ifstream file;
    file.open(filename);
    if(file.fail()){
        cout << "Cannot open file: " << filename << endl;
        exit(1);
    }
    vector<string> smap;
    while(!file.eof()){
        string line;
        getline(file, line);
        if(line.size() > 0){
            smap.push_back(line);
        }
    }
    file.close();
    return DumboMap(smap);
}

int part1(DumboMap & input){
    input.print();
    cout << endl;
    int flashedTotal = 0;
    int steps = 100;
    for(int i = 1; i < steps+1; i++){
        
        flashedTotal += input.step();
        if(DEBUG){
            cout << "After " << i << " steps." << endl;
            input.print();
        }
        
        
    }
    return flashedTotal;
}
int part2(DumboMap & input){
    int stepCount = 0;
    int flashCount = 0;
    while(flashCount != 100){
        flashCount = input.step();
        stepCount +=1;
    }
    return stepCount;
}


int main(){
    cout << "Day 11" << endl;
    DumboMap input = buildInput("input");
    int p1 = part1(input);
    cout << "Part 1: " << p1 << endl;
    input = buildInput("input");
    int p2 = part2(input);
    cout << "Part 2: " << p2 << endl;
    return 0;
}