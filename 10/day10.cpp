#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>

#define DEBUG false

using namespace std;

vector<string> buildInput(string filename){
    ifstream file;
    file.open(filename);
    if(file.fail()){
        cout << "File not found: " << filename << endl;
        exit(1);
    }
    string line;
    vector<string> out;
    while(!file.eof()){
        getline(file, line);
        if(line.size() > 0){
            out.push_back(line);
        }
    }
    file.close();
    return out;
}

pair<vector<string>, unsigned long> part1(vector<string> & input){
    vector<char> opening = {'(','[','{','<'};
    vector<char> closing = {')',']','}','>'};
    map<char, char> matching = {{'(',')'},{'[',']'},{'{','}'},{'<','>'}};
    map<char, int> scoring = {{')',3},{']',57},{'}',1197},{'>',25137}};
    vector<string> noncorruptedLines;
    unsigned long score = 0;

    for(string line : input){
        vector<char> stack;
        bool corrupted = false;
        for(char c : line){
            if(find(opening.begin(), opening.end(), c) != opening.end()){
                stack.push_back(c);
            }
            else{
                char b = stack.back();
                if(c == matching.at(b)){
                    stack.pop_back();
                }
                else{
                    // read incorrect character
                    score += scoring.at(c);
                    if(DEBUG){
                        cout << "'Expected: " << matching.at(b) << ", but found: " << c << " instead." << endl;
                        // remove corrupted line from input
                    }
                    corrupted = true;
                    break;
                }
            }
        }
        if(!corrupted){
            noncorruptedLines.push_back(line);
        }

    }
    return make_pair(noncorruptedLines, score);
}

int part2(vector<string> & input){
    vector<char> opening = {'(','[','{','<'};
    vector<char> closing = {')',']','}','>'};
    map<char, char> matching = {{'(',')'},{'[',']'},{'{','}'},{'<','>'}};
    map<char, int> scoring = {{')',1},{']',2},{'}',3},{'>',4}};
    vector<unsigned long long> scores;

    // only input lines left are those that are incomplete.
    for(string line : input){
        vector<char> stack;
        for(char c : line){
            if(find(opening.begin(), opening.end(), c) != opening.end()){
                stack.push_back(c);
            }
            else{
                char b = stack.back();
                if(c == matching.at(b)){
                    stack.pop_back();
                }
            }
        }
        unsigned long long score = 0;
        for(auto it = stack.rbegin(); it != stack.rend(); it++){
            score = score * 5;
            score += scoring.at(matching.at(*it));
        }
        scores.push_back(score);

    }
    sort(scores.begin(), scores.end());
    return scores.at(((scores.size()-1)/2));
}

int main(){
    cout << "Day 10" << endl;
    vector<string> input = buildInput("input");
    pair<vector<string>, unsigned long> p1Result = part1(input);
    cout << "Part 1: " << p1Result.second << endl;
    cout << "Part 2: " << part2(p1Result.first) << endl;
    return 0;
}