#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

vector<int> readInput(){
    ifstream file;
    file.open("input");
    string line;
    vector<int> out;
    if(!file.fail()){
        while(!file.eof()){
            getline(file, line);
            if(line.size() > 0){
                out.push_back(stoi(line));
            }
            
        }
    }
    else{
        cout << "cannot open input file" << endl;
        exit(1);
    }
    
    file.close();
    return out;
}

int part1DepthIncrementCount(vector<int> input){
    int out = 0;
    for(int i = 0; i < input.size()-1; i++){
        if(input[i] < input[i+1]){
            out++;
        }
    }
    return out;
}
int part2SlidingWindow(vector<int> input){
    int out = 0;
    int last = 0;
    int windowSize = 3;
    int i ;
    for(i = 0; i < input.size() - windowSize + 1; i++){
        // sum window
        int sum = 0;
        for(int j = i; j < i + windowSize; j++){
            sum += input[j];
        }
        if(sum > last && last != 0){
            out +=1;
        }
        last = sum;
    }
    return out;
}

int main(){
    cout << "Day 1" << endl;
    vector<int> input = readInput();
    int part1 = part1DepthIncrementCount(input);
    cout << "Part 1: " << part1 << endl;
    int part2 = part2SlidingWindow(input);
    cout << "Part 2: " << part2 << endl;
    return 0;
}