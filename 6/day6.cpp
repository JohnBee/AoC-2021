#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <map>

using namespace std;

typedef map<int, unsigned long long> FishBins;

FishBins parseInputStringImproved(string s){
    regex s_regex("[0-9]+");
    vector<string> sout(sregex_token_iterator(s.begin(), s.end(), s_regex, 0), sregex_token_iterator());
    vector<int> out;
    FishBins fishCountBins = {{0,0},{1,0},{2,0},{3,0},{4,0},{5,0},{6,0},{7,0},{8,0}};
    for(auto s : sout){
        fishCountBins[stoi(s)] += 1;
    }
    return fishCountBins;
}
FishBins buildInputImproved(){
    ifstream file;

    file.open("input");
    if(file.fail()){
        cout << "Cannot open input file." << endl;
        exit(1);
    }
    string line;
    getline(file, line);
    file.close();
    return parseInputStringImproved(line);
}
void printFishBins(FishBins & fishCountBins){
    unsigned long long count = 0;
    for(auto fi = fishCountBins.begin(); fi != fishCountBins.end(); fi++){
        cout << "[ " << fi->first << " : " << fi->second << " ] ";
        count += fi->second;
    }

    cout << "Total: " << count << endl;
}
void tickFishBins(FishBins & fishCountBins){
    FishBins nextBins = {{0,0},{1,0},{2,0},{3,0},{4,0},{5,0},{6,0},{7,0},{8,0}};
    for(int i = 0; i < fishCountBins.size(); i++){
        int b = fishCountBins.size();
        int a = i - 1;
        int next = (b + (a%b)) % b;
        if(next == 8){
            nextBins[next] += fishCountBins[i];
            nextBins[6] += fishCountBins[i];
        }
        else{
            nextBins[next] += fishCountBins[i];
        }
    }
    fishCountBins = nextBins;
}
unsigned long long countFishBins(FishBins & fishCountBins){
    unsigned long long count = 0;
    for(auto fi = fishCountBins.begin(); fi != fishCountBins.end(); fi++){
        count += fi->second;
    }
    return count;
}
void runSim(FishBins & fb, int maxDays, bool printFishCount = true){
    int day = 0;
    cout << "Init: " << endl;
    printFishBins(fb);
    while(day < maxDays){
        day++;
        tickFishBins(fb);
        if(printFishCount){
            printFishBins(fb);
        }
    }
}


int main(){
    cout << "Day 6" << endl;
    FishBins fb = buildInputImproved();
    runSim(fb, 80, false);
    cout << "Part 1: " << countFishBins(fb) << endl;
    fb = buildInputImproved();
    runSim(fb, 256, false);
    cout << "Part 2: " << countFishBins(fb) << endl;
    return 0;
}