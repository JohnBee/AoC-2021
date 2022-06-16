#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <fstream>
#include <map>
#include <deque>
#include <algorithm>
#include <functional>

using namespace std;
typedef map<string,vector<string>> PathMap;

vector<string> parseLine(string line){
    regex delim("-");
    vector<string> out(sregex_token_iterator(line.begin(), line.end(), delim, -1), sregex_token_iterator());
    return out;
}

PathMap buildInput(string filename){
    ifstream file;
    file.open(filename);
    if(file.fail()){
        cout << "Cannot open file: " << filename << endl;
        exit(1);
    }
    PathMap pmap;
    while(!file.eof()){
        string line;
        getline(file, line);
        if(line.length() > 0){
            vector<string> parsed = parseLine(line);
            if(pmap.count(parsed[0])){
                pmap[parsed[0]].push_back(parsed[1]);
            }
            else{
                pmap[parsed[0]] = {parsed[1]};
            }
            //bi-directional
            if(pmap.count(parsed[1])){
                pmap[parsed[1]].push_back(parsed[0]);
            }
            else{
                pmap[parsed[1]] = {parsed[0]};
            }
        }
        
    }
    file.close();

    return pmap;
}
bool lowerCaseName(string s){
    for(char c : s){
        if(!islower(c)){
            return false;
        }
    }
    return true;
}

vector<string> getNext(PathMap & pmap, string curr, vector<string> & path){
    vector<string> next = pmap.at(curr);
    vector<string> out;
    for(string s : next){
        bool lower = lowerCaseName(s);
        bool inpath = find(path.begin(), path.end(), s)!=path.end();

        if(!(s == "start" || lower && inpath )){
            out.push_back(s);
        }
    }
    return out;
}
vector<string> getNextP2(PathMap & pmap, string curr, vector<string> & path){
    vector<string> next = pmap.at(curr);
    vector<string> out;
    map<string, int> lowersInPath;

    // count all lowers in path
    for(string node : path){
        if(lowerCaseName(node)){
            lowersInPath[node] = count(path.begin(), path.end(), node);
        }
    }
    for(string s : next){
        bool lower = lowerCaseName(s);
        int inpath = count(path.begin(), path.end(), s);
        bool already2CopyOfLowerInPath = false;
        if(lower){
            for(auto it = lowersInPath.begin(); it != lowersInPath.end(); it++){
                if(it->second == 2 && lowersInPath.count(s) > 0){
                    already2CopyOfLowerInPath = true;
                }
            }
        }
        
        if(!(s == "start" || lower && already2CopyOfLowerInPath)){
            out.push_back(s);
        }
    }
    return out;
}
void dfs_all_paths(PathMap & pmap, function<vector<string>(PathMap&, string, vector<string>&)> getN, vector<vector<string>> & allPaths, vector<string> path = {}, string current = "start", string end = "end"){
    path.push_back(current);
    if(current == end){
        allPaths.push_back(path);
        return;
    }
    vector<string> toVisit = getN(pmap, current, path);
    for(string next : toVisit){
        dfs_all_paths(pmap, getN, allPaths, path, next);
    }
    return;
}
void printAllPaths(vector<vector<string>> & paths){
    for(vector<string> path : paths){
        for(string s : path){
            cout << s << ",";
        }
        cout << endl;
    }
}
int part1(PathMap & pmap){
    vector<vector<string>> allPaths;
    dfs_all_paths(pmap, getNext, allPaths);
    return allPaths.size();
}
int part2(PathMap & pmap){
    vector<vector<string>> allPaths;
    dfs_all_paths(pmap, getNextP2, allPaths);
    return allPaths.size();
}


int main(){
    cout << "Day 12" << endl;
    PathMap pmap = buildInput("input");
    cout << "Part 1: " << part1(pmap) << endl;
    cout << "Part 2: " << part2(pmap) << endl;
    return 0;
}