#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <map>
#include <algorithm>

using namespace std;

typedef vector<string> Display;

struct InputLine {
    Display signalPattern;
    Display outputValue;
};

InputLine parseInputLine(string line){
    InputLine out;
    regex sdelim("\\|");
    vector<string> split(sregex_token_iterator(line.begin(), line.end(), sdelim, -1), sregex_token_iterator());
    regex segment("[a-g]+");
    out.signalPattern = vector<string>(sregex_token_iterator(split[0].begin(), split[0].end(), segment, 0), sregex_token_iterator());
    out.outputValue = vector<string>(sregex_token_iterator(split[1].begin(), split[1].end(), segment, 0), sregex_token_iterator());
    return out;
}


vector<InputLine> buildInput(string filename){
    ifstream file;
    vector<InputLine> out;
    file.open(filename);
    if(file.fail()){
        cout << "Cannot open input file: " << filename << endl;
        exit(1);
    }
    while(!file.eof()){
        string line;
        getline(file, line);
        if(line.length() > 0){
            out.push_back(parseInputLine(line));
        }
    }
    return out;
}
int part1(const vector<InputLine> & input){
    map<int, int> lenToDigit = {{2,1},{4,4},{3,7},{7,8}};
    int count = 0;
    for(InputLine i : input){
        for(string d :  i.outputValue){
            if(lenToDigit.count(d.length())){
                count +=1;
            }
        }
    }
    return count;
}
// get character with frequency
char getCharWithFrequency(const map<char, int> & freq, int f){
    for(auto fa = freq.begin(); fa != freq.end(); fa++){
        if(fa->second == f){
            return fa->first;
        }
    }
    return 0;
}
int vectNumbToNumb(vector<int> & vectNumb){
    int out = 0;
    for(int i = 0; i < vectNumb.size(); i++){
        out = out * 10;
        out += vectNumb[i];
    }
    return out;
}
int part2(const vector<InputLine> & input){
    int finalValue = 0;
    map<string, int> segmentToNumb = {{"abcefg",0},{"cf",1},{"acdeg",2},{"acdfg",3},{"bcdf",4},{"abdfg",5},{"abdefg",6},{"acf",7},{"abcdefg",8},{"abcdfg",9}};
    map<int, int> lenToDigit = {{2,1},{4,4},{3,7},{7,8}};
    
    for(InputLine i : input){
        map<char, int> frequencyAnalysis = {{'a', 0},{'b', 0},{'c', 0},{'d', 0},{'e', 0},{'f', 0},{'g', 0}}; 
        map<char, char> matchedCharacters;
        for(string d :  i.signalPattern){
            if(!lenToDigit.count(d.length())){
                // frequency analysis
                for(char c : d){
                    frequencyAnalysis[c]++;
                }
            }
        }
        // find which character occurs least number of times, this is e.
        // if char occurs 3 times it must be e.
        char c = getCharWithFrequency(frequencyAnalysis, 3);
        matchedCharacters[c] = 'e';
        frequencyAnalysis.erase(c);

        // count those segments which occur only in 1,4,7,8
        map<char, int> frequencyAnalysisOneFourSevenEight; 
        for(string d :  i.signalPattern){
            if(lenToDigit.count(d.length())){
                // frequency analysis
                for(char c : d){
                    if(!matchedCharacters.count(c)){
                        // haven't already matched it
                        if(frequencyAnalysisOneFourSevenEight.count(c)){
                            frequencyAnalysisOneFourSevenEight[c]++;
                        }
                        else{
                            frequencyAnalysisOneFourSevenEight[c] = 1;
                        }
                         
                    }
                    
                }
            }
        }
        //if frequency = 1, then must be g
        c = getCharWithFrequency(frequencyAnalysisOneFourSevenEight, 1);
        matchedCharacters[c] = 'g';
        frequencyAnalysis.erase(c);
        frequencyAnalysisOneFourSevenEight.erase(c);

        // a must be the other most occuring character
        c = getCharWithFrequency(frequencyAnalysis, 6);
        matchedCharacters[c] = 'a';
        frequencyAnalysis.erase(c);
        frequencyAnalysisOneFourSevenEight.erase(c);

        // d & e have frequency of 5 in freqAn but 4 and 2 in 1,4,7,8
        for(auto fchar = frequencyAnalysis.begin(); fchar != frequencyAnalysis.end(); fchar++){
            if(fchar->second==5){
                if(frequencyAnalysisOneFourSevenEight.at(fchar->first)==4){
                    // must be d
                    matchedCharacters[fchar->first] = 'f';
                }
                else {
                    // must be e
                    matchedCharacters[fchar->first] = 'd';
                }
            }
            else {
                if(frequencyAnalysisOneFourSevenEight.at(fchar->first)==4){
                    // must be g
                    matchedCharacters[fchar->first] = 'c';
                }
                else{
                    // must be b
                    matchedCharacters[fchar->first] =  'b';
                }
            }
        }

        // convert each output string to its correct form
        Display corrected;
        for(auto ov : i.outputValue){
            vector<char> correct;
            for(char c : ov){
                correct.push_back(matchedCharacters[c]);
            }
            sort(correct.begin(), correct.end());
            corrected.push_back(string(correct.begin(), correct.end()));
        }
        // convert output to number
        vector<int> val;
        for(auto c : corrected){
            val.push_back(segmentToNumb.at(c));
        }
        finalValue += vectNumbToNumb(val);
    }
    return finalValue;
}
// frequency analyis:
// not 1, 4, 7, 8
// a : 6/6
// g : 6/6
// d : 5/6
// f : 5/6
// c : 4/6
// b : 4/6
// e : 3/6 **

// if we find out what 'g' is, then we know what 'a' is.
// a,g,b,c,d,f in 1,4,7,8
// a : 2/4
// g : 1/4 **
// b : 2/4
// c : 4/4
// d : 2/4 
// f : 4/4


// seperate d & f, and c & b.
// d,f,c,b in 1,4,7
// d : 1/3 
// f : 3/3
// c : 3/3
// b : 1/3

// done.



int main(){
    cout << "Day 8" << endl;
    vector<InputLine> input = buildInput("input");
    cout << "Part 1: " << part1(input) << endl;
    cout << "Part 2: " << part2(input) << endl;
    return 0;
}