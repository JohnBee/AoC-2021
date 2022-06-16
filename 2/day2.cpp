#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <fstream>

using namespace std;
typedef pair<string, int> instruction;

struct Submarine {
    int depth = 0;
    int right = 0;
    int aim = 0;
};

vector<string> splitString(string s, string regex_delim){
    regex s_regex(regex_delim);
    vector<string> out(sregex_token_iterator(s.begin(), s.end(), s_regex, -1), sregex_token_iterator());
    return out;
}

vector<instruction> loadInput(){
    vector<instruction> out;
    ifstream file;
    file.open("input");
    if(file.fail()){
        cout << "cannot open input file." << endl;
        exit(1);
    }
    string line;
    while(!file.eof()){
        getline(file, line);
        if(line.length() > 0){
            vector<string> splitLine = splitString(line, " ");
            instruction instr = make_pair(splitLine[0], stoi(splitLine[1]));
            out.push_back(instr);
        }
        
    }
    file.close();
    return out;
}

int part1(){
    vector<instruction> input = loadInput();
    Submarine sub;
    for(auto i : input){
        if(i.first == "up"){
            sub.depth -= i.second;
            continue;
        }
        if(i.first == "down"){
            sub.depth += i.second;
            continue;
        }
        if(i.first == "forward"){
            sub.right += i.second;
            continue;
        }
    }
    return sub.depth * sub.right;
}
int part2(){
    vector<instruction> input = loadInput();
    Submarine sub;
    for(auto i : input){
        if(i.first == "up"){
            sub.aim -= i.second;
            continue;
        }
        if(i.first == "down"){
            sub.aim += i.second;
            continue;
        }
        if(i.first == "forward"){
            sub.right += i.second;
            sub.depth += sub.aim * i.second;
            continue;
        }
    }
    return sub.depth * sub.right;
}
int main(){
    cout << "Day 2" << endl;
    cout << "Part 1: " << part1() << endl;
    cout << "Part 2: " << part2() << endl;
    return 0;
}