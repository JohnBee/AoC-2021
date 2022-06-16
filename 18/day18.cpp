#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <fstream>
#include <list>


// [1,2]
// [[1,2],3]
// [9,[8,7]]
// [[1,9],[8,5]]
// [[[[1,2],[3,4]],[[5,6],[7,8]]],9]
// [[[9,[3,8]],[[0,9],6]],[[[3,7],[4,9]],3]]
// [[[[1,3],[5,3]],[[1,3],[8,7]]],[[[4,9],[6,9]],[[8,2],[7,3]]]]


using namespace std;


bool isNumber(string s){
    char c = s.front();
    if(c >= '0' && c <= '9'){
        return true;
    }
    return false;
}

list<string> stringToList(string line){
    regex delim("[\\[\\]\\,]|[0-9]+");
    list<string> out(sregex_token_iterator(line.begin(), line.end(), delim, 0), sregex_token_iterator());
    return out;
}
vector<list<string>> buildInput(string filename){
    ifstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("Cannot open file: " + filename);
    }
    vector<list<string>> inps ;
    while(!file.eof()){
        string line;
        getline(file, line);
        if(line.length()){
            list<string> lq = stringToList(line);
            inps.push_back(lq);
        }
    }
    file.close();
    return inps;
}

bool explode(list<string> & sfn){
    list<string>::iterator it = sfn.begin();
    int depth = 0;
    list<string>::iterator left = sfn.end();
    list<string>::iterator right = sfn.end();
    while(it != sfn.end()){
        if(*it == "["){
            depth++;
        }
        if(*it == "]"){
            depth--;
        }
        if(depth == 5){
            left = next(it);
            right = next(next(left)); // skip over comma
            break;
        }
        it++;
    }
    if(left != sfn.end() && right != sfn.end()){
        it = prev(left);
        // explode left first
        while(it != sfn.begin()){
            
            if(isNumber(*it)){
                int v = stoi(*it) + stoi(*left);
                *it = to_string(v);
                break;
            }
            it--;
        }
        // explode right
        it = next(right);
        while(it !=sfn.end()){
            if(isNumber(*it)){
                int v = stoi(*it) + stoi(*right);
                *it = to_string(v);
                break;
            }
            it++;
        }
        list<string>::iterator leftBracket = prev(left);
        list<string>::iterator comma = next(left);
        list<string>::iterator rightBracket = next(right);
        sfn.erase(rightBracket);
        sfn.erase(right);
        sfn.erase(comma);
        sfn.erase(left);
        *leftBracket = to_string(0);
    }
    return left != sfn.end() || right != sfn.end();
}
bool split(list<string> & sfn){
    list<string>::iterator it = sfn.begin();
    while(it != sfn.end()){
        if(isNumber(*it) && stoi(*it) > 9){
            int val = stoi(*it);
            int leftVal = floor(((double)val)/2);
            int rightVal = ceil((double)val/2);
            list<string> toInsert = {"[",to_string(leftVal), ",", to_string(rightVal), "]"};
            sfn.insert(it, toInsert.begin(), toInsert.end());
            sfn.erase(it);
            break;
        }
        it++;
    }
    return it != sfn.end();
}
list<string> addNumbers(list<string> left, list<string> right){
    auto it = right.begin();
    right.insert(it, left.begin(), left.end());
    right.insert(it, ",");
    right.insert(right.begin(), "[");
    right.insert(right.end(), "]");
    return right;
}

void printSfn(list<string> sfn){
    for(auto it = sfn.begin(); it!= sfn.end(); it++){
        cout << *it;
    }
    cout << endl;
}
void processString(list<string> & sfn){
    bool didExplode = true;
    bool didSplit = true;
    int steps = 0;
    while(didExplode || didSplit){
        didExplode = true;
        didSplit = true;
        int leftBracket = 0;
        int rightBracket = 0;
        while(didExplode){
            didExplode = explode(sfn);
            // printSfn(sfn);
        }
        didSplit = split(sfn);
        // printSfn(sfn);
    }
}
int calculateMagnitude(list<string> sfn){
    while(sfn.size() > 1){
        int sum;
        list<string>::iterator it = sfn.end();
        for(it = sfn.begin(); it != sfn.end(); it++){
            if(isNumber(*it) && isNumber(*next(next(it)))){
                sum = stoi(*it)*3 + stoi(*next(next(it)))*2;
                break;
            }
        }
        if(it != sfn.end()){
            it = prev(it);
            *it = to_string(sum);
            list<string>::iterator left = next(it);
            list<string>::iterator comma = next(left);
            list<string>::iterator right = next(comma);
            list<string>::iterator rightBracket = next(right);
            sfn.erase(rightBracket);
            sfn.erase(right);
            sfn.erase(comma);
            sfn.erase(left);
        }
    }
    string mag = sfn.front();
    return stoi(mag); 
}
int part1(){
    vector<list<string>> input = buildInput("input");
    list<string> sum = input.front();
    for(auto it = next(input.begin()); it != input.end(); it++){
        // printSfn(sum);
        sum = addNumbers(sum, *it);
        processString(sum);
    }
    // printSfn(sum);
    return calculateMagnitude(sum);
}
int part2(){
    vector<list<string>> input = buildInput("input");
    int magnitude = 0;
    for(auto x = input.begin(); x != prev(input.end()); x++){
        for(auto y = next(x); y != input.end(); y++){
                list<string> sum1 = addNumbers(*x,*y);
                list<string> sum2 = addNumbers(*y, *x);
                processString(sum1);
                processString(sum2);
                int mag1 = calculateMagnitude(sum1);
                int mag2 = calculateMagnitude(sum2);
                if(mag1 > magnitude){
                    magnitude = mag1;
                }
                if(mag2 > magnitude){
                    magnitude = mag2;
                }

        }
            

    }
    return magnitude;
}
    


int main(){
    cout << "Day 18" << endl;
    // [[[[[9,8],1],2],3],4] - explode
    // [[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]] - explode
    // [[1,2],[[[[2,3],4],5],6]]
    // cout << "Part 1: " << part1() << endl;
    // list<string> input = stringToList("[[[[1,[2,3]],[4,5]],[[[6,7],[8,9]],[10,11]]],[12,[[[13,14],[15,16]],[[17,18],[19,20]]]]]");
    // list<string> input = stringToList("[[[[0,[4,5]],[0,0]],[[[4,5],[2,6]],[9,5]]],[7,[[[3,7],[4,3]],[[6,3],[8,8]]]]]");
    // list<string> left = stringToList("[[[[4,3],4],4],[7,[[8,4],9]]]");
    // list<string> right = stringToList("[1,1]");
    // processString(input);
    // part1();
    // list<string> input = stringToList("[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]");
    // cout << calculateMagnitude(input) << endl;
    cout << "Part 1: " << part1() << endl;
    cout << "Part 2: " << part2() << endl;
    return 0;
}