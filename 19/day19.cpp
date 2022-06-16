#include <iostream>
#include <regex>
#include <vector>
#include <fstream>
#include <map>
#include <cmath>
#include <set>

using namespace std;

struct Coord {
    int x,y,z;
    Coord(int x, int y, int z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Coord(){};
    friend bool operator==(Coord & lhs, Coord & rhs){
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
    }
    bool operator==(const Coord & rhs){
        return this->x == rhs.x && this->y == rhs.y && this->z == rhs.z;
    }
    bool operator<(const Coord & rhs) const{
        if(this->z < rhs.z){
            return true;
        }
        else if(this->z == rhs.z){
            if(this->y < rhs.y){
                return true;
            }
            else if(this->y == rhs.y){
                if(this->x < rhs.x){
                    return true;
                }
                else{
                    return false;
                }
            }
            else{
                return false;
            }
        }
        else{
            return false;
        }
    }
    void absolute(){
        x = std::abs(x);
        y = std::abs(y);
        z = std::abs(z);
    }   
    friend Coord operator-(Coord & lhs, Coord & rhs){
        return Coord(lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z);
    }
    friend Coord operator+(Coord & lhs, Coord & rhs){
        return Coord(lhs.x+rhs.x, lhs.y+rhs.y, lhs.z+rhs.z);
    }
};
Coord roll(Coord v){
    return Coord(v.x, v.z, -v.y);
}
Coord turn(Coord v){
    return Coord(-v.y, v.x, v.z);
}
Coord pitch(Coord v){
    return Coord(-v.z ,v.y, v.x);
}
Coord inverseRoll(Coord v){
    return Coord(v.x, -v.z, v.y);
}
Coord inverseTurn(Coord v){
    return Coord(v.y, -v.x, v.z);
}
Coord inversePitch(Coord v){
    return Coord(v.z, v.y, -v.x);
}
// vector<pair<Coord, string>> allOrientations(Coord v){
//     auto cmp = [](pair<Coord, string> a, pair<Coord, string> b){return a.first < b.first;};
//     set<pair<Coord, string>, decltype(cmp)> uniqueOrientations(cmp);

//     string transformation;
//     for(int i = 0; i < 2; i++){
//         for(int j = 0; j < 3; j++){
//             v = roll(v);
//             transformation.push_back('R'); // R for roll
//             uniqueOrientations.insert({v, transformation});
//             for(int k = 0; k < 3; k++){
//                 v = turn(v);
//                 transformation.push_back('T'); // T for turn
//                 uniqueOrientations.insert({v, transformation});
                
//             }
//             v = roll(turn(roll(v)));
//             transformation.push_back('R'); // roll turn roll
//             transformation.push_back('T'); 
//             transformation.push_back('R');
//         }
//     }
//     vector<pair<Coord, string>> out(uniqueOrientations.begin(), uniqueOrientations.end());
//     return out;
// }
// vector<pair<Coord, string>> allOrientations(Coord v){
//     // auto cmp = [](pair<Coord, string> a, pair<Coord, string> b){return a.first < b.first;};
//     // set<pair<Coord, string>, decltype(cmp)> uniqueOrientations(cmp);
//     set<Coord> unique;
//     vector<pair<Coord, string>> out;

//     string transformation = "";
//     // identity
//     Coord c = v;
//     for(int i = 0; i < 4; i++){
//         for(int j = 0; j < 4; j++){
//             v = roll(v);
//             transformation.push_back('R'); // R for roll
//             out.push_back({v, transformation});
//             unique.insert(v);
//         }
//         v = turn(v);
//         transformation.push_back('T'); // R for roll
//     }
//     // back to begining, make x face upwards then do 4 turns
//     v = pitch(v);
//     transformation.push_back('P'); // P for pitch
//     for(int j = 0; j < 4; j++){
//             v = turn(v);
//             transformation.push_back('T');
//             out.push_back({v, transformation});
//             unique.insert(v);
//     }
//     v = pitch(v);
//     transformation.push_back('P'); // P for pitch
//     unique.insert(v);
//     v = pitch(v);
//     transformation.push_back('P'); // P for pitch
//     unique.insert(v);
//     for(int j = 0; j < 4; j++){
//             v = turn(v);
//             transformation.push_back('T');
//             out.push_back({v, transformation});
//             unique.insert(v);
//     }
//     // vector<pair<Coord, string>> out(uniqueOrientations.begin(), uniqueOrientations.end());
//     return out;
// }
vector<pair<Coord, string>> allOrientations(Coord v){
    set<Coord> uniqueOrientations;
    set<Coord> unique;
    vector<pair<Coord, string>> out;

    string transformation = "";
    // identity
    Coord c = v;
    for(int i = 0; i < 8; i++){
        v = roll(v);
        transformation.push_back('R');
        if(uniqueOrientations.find(v) == uniqueOrientations.end()){
            out.push_back({v, transformation});
            uniqueOrientations.insert(v);
        }

        v = turn(v);
        transformation.push_back('T');
        
        if(uniqueOrientations.find(v) == uniqueOrientations.end()){
            out.push_back({v, transformation});
            uniqueOrientations.insert(v);
        }

        if(i % 2 == 0){
            v = roll(v);
            transformation.push_back('R');
            if(uniqueOrientations.find(v) == uniqueOrientations.end()){
                out.push_back({v, transformation});
                uniqueOrientations.insert(v);
            }
            
            
        }
        v = pitch(v);
        transformation.push_back('P');
        if(uniqueOrientations.find(v) == uniqueOrientations.end()){
            out.push_back({v, transformation});
            uniqueOrientations.insert(v);
        } 
        
         
    }
    return out;
}
struct absBeaconDiff {
    pair<Coord, Coord> beacons;
    vector<pair<Coord, string>> orientations;
    absBeaconDiff(pair<Coord, Coord> beacons, vector<pair<Coord, string>> orientations){
        this->beacons = beacons;
        this->orientations = orientations;
    }
};


struct Scanner {
    vector<Coord> beacons;
    map<Coord, absBeaconDiff> absoluteBeaconDifferences;
    void generateAllAbsoluteBeaconDifferences(){
        for(auto b = beacons.begin(); b != prev(beacons.end()); b++){
            for(auto a = next(b); a != beacons.end(); a++){
                // Coord absolute = (*a-*b);
                // absolute.absolute();
                vector<pair<Coord, string>> aOrientations = allOrientations(*a);
                vector<pair<Coord, string>> bOrientations = allOrientations(*b);
                vector<pair<Coord, string>> orientationDifferences;
                for(int i = 0; i < aOrientations.size(); i++){
                    Coord k = (aOrientations.at(i)).first - (bOrientations.at(i)).first;
                    orientationDifferences.push_back({k, aOrientations.at(i).second});
                }
                absoluteBeaconDifferences.insert({*a-*b, absBeaconDiff({*a, *b}, orientationDifferences)});
            }
        }
    }
};

typedef map<int, Scanner> ScannerMap;

int parseScannerId(string line){
    regex delim("[0-9]+");
    vector<string> out(sregex_token_iterator(line.begin(), line.end(), delim, 0), sregex_token_iterator());
    if(out.size() != 1){
        throw runtime_error("Scanner ID parse fail - found " + to_string(out.size()) + " matching ids, should be one.");
    }
    return stoi(out.at(0));
}
Coord parseCoord(string line){
    regex delim("-?[0-9]+");
    vector<string> out(sregex_token_iterator(line.begin(), line.end(), delim, 0), sregex_token_iterator());
    if(out.size() != 3){
        throw runtime_error("Coord parse fail - found " + to_string(out.size()) + " matching coords, should be three.");
    }
    return Coord(stoi(out.at(0)),stoi(out.at(1)), stoi(out.at(2)));
}

ScannerMap buildInput(string filename){
    ifstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("Cannot open file: " + filename);
    }
    ScannerMap sm;
    while(!file.eof()){
        // parse scanner id;
        string line;
        int scannerId; 
        getline(file, line);
        scannerId = parseScannerId(line);
        getline(file, line);
        Scanner scan;
        while(line.length() && !file.eof()){
            scan.beacons.push_back(parseCoord(line));
            getline(file,line);
        }
        scan.generateAllAbsoluteBeaconDifferences();
        sm.insert({scannerId, scan});
    }
    file.close();
    return sm;
}
struct matchingBeaconPair{
    pair<Coord, Coord> p1;
    pair<Coord, Coord> p2;
    string transformation;
    matchingBeaconPair(pair<Coord, Coord> p1, pair<Coord, Coord> p2, string transformation){
        this->p1 = p1;
        this->p2 = p2;
        this->transformation = transformation;
    }
};
// Find groups of beacons
// get the absolute positions between beacons
// rotate the difference in positions between 6 different directions up down left right forward backward
// find those absolute difference in positions between pairs in other scanners.
vector<matchingBeaconPair> findMatchingBeaconPairs(Scanner & s1, Scanner & s2 ){
    vector<matchingBeaconPair> foundBeaconPairs; // {pair from scanner0 , pair from scanner1}
    for(auto a = s1.absoluteBeaconDifferences.begin(); a != s1.absoluteBeaconDifferences.end(); a++){
        vector<pair<Coord, string>> orientationDifferences = a->second.orientations;
        for(pair<Coord, string> rotation: orientationDifferences){
            Coord c = rotation.first;
            if(s2.absoluteBeaconDifferences.count(rotation.first)){
                foundBeaconPairs.push_back(
                    matchingBeaconPair(
                        a->second.beacons,
                        s2.absoluteBeaconDifferences.at(rotation.first).beacons,
                        rotation.second
                        ));
            }
        }
    }
    return foundBeaconPairs;
}
Coord applyTransformation(Coord c1, string RollTurnString){
    // apply transformation in reverse
    for(auto c = RollTurnString.begin(); c != RollTurnString.end(); c++){
        if(*c=='R'){
            c1 = roll(c1);
        }
        if(*c=='T'){
            c1 = turn(c1);
        }
        if(*c=='P'){
            c1 = pitch(c1);
        }
    }
    return c1;
}
Coord applyInverseTransformation(Coord c1, string RollTurnString){
    // apply transformation in reverse
    for(auto c = RollTurnString.rbegin(); c != RollTurnString.rend(); c++){
        if(*c=='R'){
            c1 = inverseRoll(c1);
        }
        if(*c=='T'){
            c1 = inverseTurn(c1);
        }
        if(*c=='P'){
            c1 = inversePitch(c1);
        }
    }
    return c1;
}

struct ScannerTransformationInformation {
    Coord absolutedifferenceBetweenScannerBandA;
    string transformationBetweenScannerBandA;
    ScannerTransformationInformation(Coord absolutedifferenceBetweenScannerBandA, string transformationBetweenScannerBandA){
        this->absolutedifferenceBetweenScannerBandA = absolutedifferenceBetweenScannerBandA;
        this->transformationBetweenScannerBandA = transformationBetweenScannerBandA;
    }
};

vector<Coord> applyScannerTransformation(Scanner & s, ScannerTransformationInformation & sti){
    vector<Coord> out;
    for(Coord c : s.beacons){
        Coord _id = applyInverseTransformation(c, sti.transformationBetweenScannerBandA);
        Coord beaconPos = _id + sti.absolutedifferenceBetweenScannerBandA;
        out.push_back(beaconPos);
    }
    return out;
}
bool checkTransform(Coord s1, Coord s2){
    vector<int> a = {abs(s1.x), abs(s1.y), abs(s1.z)};
    vector<int> b = {abs(s2.x), abs(s2.y), abs(s2.z)};
    for(int i : a){
        if(find(b.begin(), b.end(), i) == b.end()){
            return false;
        }
    }
    return true;
}
int part1(ScannerMap & sm){
    map<pair<int, int>, ScannerTransformationInformation> scannerConversionMap;
    // for all combinations of scanner
    for(int j = 0; j < sm.size()-1; j++){
        for(int k = j+1; k < sm.size(); k++){
            Scanner s0 = sm.at(j);
            Scanner s1 = sm.at(k);
            vector<matchingBeaconPair> beaconPairsFound = findMatchingBeaconPairs(s0,s1);
            map<string, int> transformationCount;
            
            if(beaconPairsFound.size() < 5){
                continue; // didn't find any pairs, skip
            }

            for(matchingBeaconPair mbp : beaconPairsFound){
                // get the common transformation.
                if(transformationCount.count(mbp.transformation)){
                    transformationCount.at(mbp.transformation)++;
                }
                else{
                    transformationCount.insert({mbp.transformation, 1});
                }
            }
            string transformationUsed;
            int transformationSum = 0;
            for(auto transCount = transformationCount.begin(); transCount != transformationCount.end(); transCount++){
                if(transCount->second > transformationSum){
                    transformationUsed = transCount->first;
                    transformationSum = transCount->second;
                }
            }
            
            // calculate the absolute positional differences between the two
            Coord scannerPos;
            for(matchingBeaconPair mbp : beaconPairsFound){
                if(mbp.transformation == transformationUsed){
                    // Coord _d = applyTransformation(mbp.p2.first, transformationUsed);
                    Coord _d = applyInverseTransformation(mbp.p2.first, transformationUsed);
                    scannerPos = mbp.p1.first - _d;
                    if(!checkTransform(mbp.p2.first, _d)){
                        throw(runtime_error("Transformation corruption."));
                    }
                    // Coord _id = applyInverseTransformation(mbp.p2.first, transformationUsed);
                    // Coord beaconPos = _id + scannerPos;
                    // Coord _f = applyTransformation(mbp.p2.second, transformationUsed);
                    // Coord scannerPos2 = mbp.p1.first - _f;
                    // Coord test_a = applyInverseTransformation(mbp.p2.first, transformationUsed);
                    // Coord test_coord = mbp.p1.first - test_a;
                    // if(k != 4){
                    //     int test =1;
                    //     // break;
                    // }
                    break;
                    
                    // cout << beaconPos.x << " " << beaconPos.y << " " << beaconPos.z << endl;
                }
            }
            
            scannerConversionMap.insert({{j,k}, ScannerTransformationInformation(scannerPos, transformationUsed)});
            // Found matching beacons between scanner 0 and scanner 1
            // got the distance between the 2 scanners and transformation.
            // can now convert between beacons of scanner 1 to position in scanner 0.

            // Feed some sort of mega list that start with scanner 0 beacons, then repeatedly use this list


            // store how to convet s1 to s0
            // store how to convert s2 to s1
            // .. etc
            // then work backwards and convert all to s0

            // 68 = -345 - -(413x)  -1
            // -1246 = -311 - (935y) 1
            // -43 = 381 - -(-424z) -1

            // 68 = 404 - (336x)     1
            // -1246 = -588 - (658y) 1
            // -43 = -901 - (-858z) -1

            // 68 = 544 - -(-476x)  -1
            // -1246 = -627 - 619    1
            // -43 = -890 - (-847)  -1

            // 68 = 423 - -(-355x) -1
            // -1246 = -701 - 545   1
            // -43 = 434 - -(-477) -1
        }


    }
    // do back conversion for all now
    map<int, bool> converted; // keep track if a scanner was converted to
    for(auto k = sm.begin(); k != sm.end(); k++){
        converted.insert({k->first, false});
    }
    for(int k = sm.size()-1; k > 0; k--){
        for(int i = k-1; i >= 0; i--){
            // cout << i << " " << k << endl;
            if(scannerConversionMap.count({i,k})){
                vector<Coord> beacons = applyScannerTransformation(sm.at(k),scannerConversionMap.at({i,k}));
                converted.at(k) = true;
                for(Coord b : beacons){
                    if(find(sm.at(i).beacons.begin(), sm.at(i).beacons.end(), b) == sm.at(i).beacons.end()){
                        sm.at(i).beacons.push_back(b);
                    }
                }
            }
        }
    }
    // works on example, correct number of beacons
    // incorrect answer on regular input: 267, too few beacons
    // 681 too many
    // 618 too many
    // vector<Coord> beacons = applyScannerTransformation(sm.at(2),scannerConversionMap.at({0,2}));
    int sum = 0;
    for(auto c = converted.begin(); c != converted.end(); c++){
        if(c->second == false){
            sum += sm.at(c->first).beacons.size();
        }
    }
    return sum;
}

int main(){
    cout << "Day 19" << endl;
    ScannerMap sm = buildInput("input_small");
    cout << "[Working...] Build Scanner map." << endl;
    cout << "Part 1: " << part1(sm) << endl;
    // allOrientations(Coord(1,2,3));
    return 0;
}