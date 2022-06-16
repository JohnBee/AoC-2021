#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <math.h>

using namespace std;

vector<string> loadInput(){
    ifstream file;
    vector<string> out;
    file.open("input");
    if(file.fail()){
        cout << "Cannot open input file." << endl;
        exit(1);
    }
    string line;
    while(!file.eof()){
        getline(file, line);
        out.push_back(line);
    }
    file.close();
    return out;
}
int binVectorToInt(vector<int> in){
    int twoPow = 0;
    int out = 0;
    for(int i = in.size()-1; i >= 0; i--){
        out += in[i]*pow(2, twoPow);
        twoPow +=1;
    }
    return out;
}
int binVectorToInt(string in){
    int twoPow = 0;
    int out = 0;
    for(int i = in.length()-1; i >= 0; i--){
        if(in[i] == '1'){
            out += pow(2, twoPow);
        }
        twoPow +=1;
    }
    return out;
}
int part1(){
    vector<string> input = loadInput();
    vector<int> mcb;
    vector<int> lcb;
    int bitsLength = input[0].length();
    for(int i = 0; i < bitsLength; i++){
        int zeroCount = 0;
        int oneCount = 0;
        for(string s : input){
            if(s[i]=='0'){
                zeroCount +=1;
            }
            else if(s[i]=='1'){
                oneCount +=1;
            }
        }
        if(zeroCount > oneCount){
            mcb.push_back(0);
            lcb.push_back(1);
        }
        else{
            mcb.push_back(1);
            lcb.push_back(0);
        }
    }
    return binVectorToInt(mcb) * binVectorToInt(lcb);
}
vector<string> discard(vector<string> & input, int position, char bit){
    // copy vector
    vector<string> filteredInput;
    for(string s : input){
        if(s[position] == bit){
            filteredInput.push_back(s);
        }
    }
    return filteredInput;
}
int part2(){
    vector<string> inputOxygen = loadInput();
    vector<string> inputCO2 = loadInput();
    int bitsLength = inputOxygen[0].length();
    // find oxygen rating
    for(int i = 0; i < bitsLength && inputOxygen.size() > 1; i++){
        int zeroCount = 0;
        int oneCount = 0;
        for(string s : inputOxygen){
            if(s[i]=='0'){
                zeroCount +=1;
            }
            else if(s[i]=='1'){
                oneCount +=1;
            }
        }
        // discard all from input list where this bit is not found
        if(oneCount >= zeroCount){
            inputOxygen = discard(inputOxygen, i, '1');
        }
        else{
            inputOxygen = discard(inputOxygen, i, '0');
        }
    }
    // find CO2 rating
    for(int i = 0; i < bitsLength && inputCO2.size() > 1; i++){
        int zeroCount = 0;
        int oneCount = 0;
        for(string s : inputCO2){
            if(s[i]=='0'){
                zeroCount +=1;
            }
            else if(s[i]=='1'){
                oneCount +=1;
            }
        }
        // discard all from input list where this bit is not found
        if(oneCount < zeroCount){
            inputCO2 = discard(inputCO2, i, '1');
        }
        else{
            inputCO2 = discard(inputCO2, i, '0');
        }
    }
    string oxygenRating = inputOxygen[0];
    string co2Rating = inputCO2[0];
    return binVectorToInt(oxygenRating)*binVectorToInt(co2Rating);
}


int main(){
    cout << "Day 3" << endl;
    cout << part1() << endl;
    cout << part2() << endl;
    return 0;
}