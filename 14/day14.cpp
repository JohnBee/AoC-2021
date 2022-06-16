#include <iostream>
#include <Vector>
#include <fstream>
#include <string>
#include <map>
#include <exception>
#include <regex>
#include <list>

#define DEBUG false

using namespace std;

struct PolymerData {
    list<char> polymerString;
    map<string, char> pairRules;

    PolymerData(string & ps, map<string, char> & pr){
        for_each(ps.begin(), ps.end(), [this](char c){this->polymerString.push_back(c); });
        pairRules = pr;
    }
    void runRules(int steps){
        for(int i = 0; i < steps; i++){
            auto it = polymerString.begin();
            while(it != prev(polymerString.end())){
                string ps(it, next(next(it)));
                if(pairRules.count(ps)){
                    polymerString.insert(next(it), pairRules.at(ps));
                    it = next(next(it));
                }
                else{
                    it++;
                }
                
            }
            if(DEBUG){
                cout << "Step " << i << endl;
                for_each(polymerString.begin(), polymerString.end(), [&](char c){ cout << c;});
                cout << endl;
            }
        }
    }
};

vector<string> splitString(const string & s, const string & delim){
    regex d(delim);
    vector<string> out(sregex_token_iterator(s.begin(), s.end(), d, -1), sregex_token_iterator());
    return out;
}

PolymerData buildInput(string filename){
    ifstream file;
    file.open(filename);
    if(file.fail()){
        throw std::runtime_error("File not found: " + filename);
    }
    int readingMode = 0;
    string polymerTemplate;
    map<string, char> pairs;
    while(!file.eof()){
        string line;
        getline(file, line);
        if(line.length() == 0){
            readingMode +=1;
        }
        else{
            if(readingMode == 0){
                polymerTemplate = line;
            }
            else{
                vector<string> s = splitString(line, " -> ");
                pairs.insert({s.at(0), s.at(1).at(0)});
            }
        }
    }
    file.close();
    return PolymerData(polymerTemplate, pairs);
}
int part1(PolymerData p, int steps){
    p.runRules(steps);
    map<char, int> letterCount;
    for(auto it = p.polymerString.begin(); it != p.polymerString.end(); it++){
        auto at = letterCount.find(*it);
        if(at!=letterCount.end()){
            at->second += 1;
        }
        else{
            letterCount.insert({*it, 1});
        }
    }
    int least = INT_MAX;
    int most = 0;
    for(auto it = letterCount.begin(); it != letterCount.end(); it++){
        if(it->second > most){
            most = it->second;
        }
        if(it->second < least){
            least = it->second;
        }
    }
    return most-least;
}
unsigned long long part2(PolymerData p, int steps){
    map<string, unsigned long long> madePairs;
    
    // make pairs
    for(auto it = p.pairRules.begin(); it != p.pairRules.end(); it++){
        madePairs.insert({it->first, 0});
    }
    for(auto it = p.polymerString.begin(); it != prev(p.polymerString.end()); it++){
        string ps(it, next(next(it)));
        auto pair = madePairs.find(ps);
        pair->second++;
    }
    // for each step, find the pairs and expand them to make new pairs.

    for(int i = 0; i < steps; i++){
        map<string, unsigned long long> nextPairs;
        for(auto it = madePairs.begin(); it != madePairs.end(); it++){
            nextPairs.insert({it->first, 0});
            if(it->second > 0){
                // NN -> NC++ -> CN++
                unsigned long long count = it->second;
                string left({it->first.at(0),p.pairRules.at(it->first)});
                string right({p.pairRules.at(it->first),it->first.at(1)});
                auto p1 = nextPairs.find(left);
                if(p1==nextPairs.end()){
                    nextPairs.insert({left, count});
                }
                else{
                    p1->second += count;
                }
                p1 = nextPairs.find(right);
                if(p1==nextPairs.end()){
                    nextPairs.insert({right, count});
                }
                else{
                    p1->second += count;
                }

            }
            
        }
        madePairs = nextPairs;
    }

    // Find least and most Chars
    map<char, unsigned long long> letterCount;
    for(auto it = madePairs.begin(); it != madePairs.end(); it++){
        char left = it->first.at(0);
        char right= it->first.at(0);
        auto at = letterCount.find(left);
        if(at!=letterCount.end()){
            at->second += it->second;
        }
        else{
            letterCount.insert({left, it->second});
        }
    }

    // +1 for last character
    char lastLetter = p.polymerString.back();
    auto at = letterCount.find(lastLetter);
    if(at!=letterCount.end()){
        at->second += 1;
    }
    else{
        letterCount.insert({lastLetter, 1});
    }
    
    unsigned long long least = ULLONG_MAX;
    unsigned long long most = 0;
    for(auto it = letterCount.begin(); it != letterCount.end(); it++){
        if(it->second > most){
            most = it->second;
        }
        if(it->second < least){
            least = it->second;
        }
    }
    // cout << most << " " << least << endl;
    return most-least;
    
}
int main(){
    cout << "Day 14" << endl;
    PolymerData p = buildInput("input");
    cout << "Part 1: " << part2(p,10) << endl;
    p = buildInput("input");
    cout << part2(p,40) << endl;
    return 0;
}