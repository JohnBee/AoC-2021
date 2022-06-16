#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <math.h>

using namespace std;

vector<int> regexParseInput(string line){
    regex delim("[0-9]+");
    vector<string> sr(sregex_token_iterator(line.begin(), line.end(), delim, 0),sregex_token_iterator());
    vector<int> out;
    for(auto s : sr){
        out.push_back(stoi(s));
    }
    return out;
}


vector<int> buildInput(){
    vector<int> out;
    ifstream file;
    file.open("input");
    string line;
    if(file.fail()){
        cout << "Cannot open input file.";
        exit(1);
    }
    getline(file, line);
    if(line.size() == 0){
        cout << "Input file empty";
        exit(1);
    }

    file.close();
    return regexParseInput(line);
}
int part1(vector<int> input){
    // find local minimal
    // brute force
    unsigned long long smallestFuel = ULLONG_MAX;
    int smallestPos = 0;
    int min = INT_MAX;
    int max = 0;
    for(int i : input){
        if(i < min){
            min = i;
        }
        if(i > max){
            max = i;
        }
    }
    for(int pos = min; pos < max; pos++){
        unsigned long long fuel = 0;
        for(int i : input){
            fuel += abs(i-pos);
        }
        if(fuel < smallestFuel){
            smallestFuel = fuel;
            smallestPos = pos;
        }
    }
    cout << "Pos: " << smallestPos << endl;
    return smallestFuel;
}
int part2(vector<int> input){
    // find local minimal
    // brute force
    unsigned long long smallestFuel = ULLONG_MAX;
    int smallestPos = 0;
    int min = INT_MAX;
    int max = 0;
    for(int i : input){
        if(i < min){
            min = i;
        }
        if(i > max){
            max = i;
        }
    }
    for(int pos = min; pos < max; pos++){
        unsigned long long fuel = 0;
        for(int i : input){
            unsigned long long x = abs(i-pos);
            fuel += (x*x + x)/2;
        }
        if(fuel < smallestFuel){
            smallestFuel = fuel;
            smallestPos = pos;
        }
    }
    cout << "Pos: " << smallestPos << endl;
    return smallestFuel;
}

int main(){
    cout << "Day 7" << endl;
    vector<int> input = buildInput();
    cout << "Part 1: " << part1(input) << endl;
    cout << "Part 2: " << part2(input) << endl;
    return 0;   
}

