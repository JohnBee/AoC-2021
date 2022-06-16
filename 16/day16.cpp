#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <exception>
#include <algorithm>
#include <map>
#include <memory>
#include <cmath>
#include <list>

#define DEBUG_1 false
#define DEBUG_2 false
using namespace std;

typedef list<int> Bitstream;

typedef unsigned long long BigNumber;
#define MAX_VAL ULLONG_MAX 

struct Packet {
    int version;
    int typeID;
    BigNumber value = MAX_VAL;
    vector<shared_ptr<Packet>> packets;
    Packet() = default;
    virtual ~Packet() = default;
};

vector<char> readInput(string filename){
    ifstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("Cannot open file: " + filename);
    }
    string line;
    getline(file, line);
    if(!line.size()){
        throw runtime_error("No packet data in " + filename);
    }
    file.close();
    vector<char> out;
    for_each(line.begin(), line.end(), [&out](char c){out.push_back(c);});
    return out;
}
Bitstream hexStringToBits(vector<char> d){
    map<char, string> hexToBin = {
        {'0',"0000"},{'1',"0001"},{'2',"0010"},{'3',"0011"},
        {'4',"0100"},{'5',"0101"},{'6',"0110"},{'7',"0111"},
        {'8',"1000"},{'9',"1001"},{'A',"1010"},{'B',"1011"},
        {'C',"1100"},{'D',"1101"},{'E',"1110"},{'F',"1111"},
    };
    // to bit string
    Bitstream bits;
    string s;
    for(char c : d){
        s.append(hexToBin.at(c));
    }
    for(char c : s){
        bits.push_back(c - '0');
    }

    return bits;
}
BigNumber bitsToInt(Bitstream bits){
    BigNumber sum = 0;
    BigNumber mul = 0;
    for(auto it = bits.rbegin(); it != bits.rend(); it++){
        sum += pow(2, mul)*(*it);
        mul++; 
    }
    return sum;
}
void debugPrintStatus(int status, BigNumber valueRead, int depth = 0){
    if(DEBUG_1){
        // padding for depth
        for(int i = 0; i < depth; i++){
            cout << "\t";
        }
        
        switch(status){
            case 0:
                cout << "[Version] ";
                break;
            case 1:
                cout << "[Type ID] ";
                break;
            case 2:
                cout << "[Literal] ";
                break;
            case 3:
                cout << "[Operator] ";
                break;
            case 8:
                cout << "[Operator] total Length in bits ";
                break;
            case 9:
                cout << "[Operator] Subpacket Length ";
                break;
            case 10:
                cout << "[Type Length ERROR]";
                break;
            default:
                cout << "[ERROR] Unknown status: " << status << " ";              
        }
        cout << valueRead << endl;
    }
    return;
}
Bitstream readN(Bitstream & d, int n){
    Bitstream out;
    for(int i = 0; i < n; i++){
        int j = d.front();
        d.pop_front();
        out.push_back(j);
    }
    return out;
}
shared_ptr<Packet> parsePacket(Bitstream & b, int depth = 0){
    shared_ptr<Packet> packet(new Packet);
    map<int, int> typeIDToStatus = {{4, 2}, {6, 3},{3, 4}};
    // typeID
    // 4 - literal
    // not 4 - operator 
    // status
    // 0 - version
    // 1 - type id
    // 2 - literal value
    //
    int status = 0;
    // parse version
    int version;
    int typeID;
    BigNumber literalValue;
    if(status == 0){
        version = bitsToInt(readN(b,3));
        packet->version = version;
        debugPrintStatus(status, version, depth);
        status = 1;
    }
    if(status == 1){
        typeID = bitsToInt(readN(b,3));
        packet->typeID = typeID;
        debugPrintStatus(status, typeID, depth);
    }
    auto f = typeIDToStatus.find(typeID);
    if(f != typeIDToStatus.end()){
        status = f->second;
    }
    if(status == 2){
        // read literal
        bool keepReading = 1;
        Bitstream literalBits;
        while(keepReading){
            Bitstream j = readN(b, 5);
            keepReading = j.front();
            for_each(next(j.begin()), j.end(), [&literalBits](int i){literalBits.push_back(i);});
        }
        readN(b,literalBits.size() % 4); //throw extra bits away
        literalValue = bitsToInt(literalBits);
        debugPrintStatus(status, literalValue, depth);
        packet->value = literalValue;
        return packet;
    }
    if(status){ //any other status is operator
        int lengthTypeID = readN(b,1).front(); // read length type id
        debugPrintStatus(3, status, depth);
        int length;
        if(lengthTypeID == 0){
            length = bitsToInt(readN(b, 15));
            debugPrintStatus(8, length, depth);
            Bitstream bits = readN(b, length);
            while(!bits.empty()){
                packet->packets.push_back(parsePacket(bits, depth+1));
            }
        }
        else if(lengthTypeID == 1){
            length = bitsToInt(readN(b, 11));
            debugPrintStatus(9, length, depth);
            for(int i = 0; i < length; i++){
                packet->packets.push_back(parsePacket(b, depth+1));
            }
        }
        else{
            debugPrintStatus(10, lengthTypeID, depth);
        }

    }
    return packet;
}
int recursiveVersionSum(shared_ptr<Packet> packet){
    if(packet->packets.size() == 0){
        return packet->version;
    }
    int sum = packet->version;
    for(auto p : packet->packets){
        sum += recursiveVersionSum(p);
    }
    return sum;
}
int part1(string filename){
    vector<char> data = readInput(filename);
    Bitstream b = hexStringToBits(data);
    shared_ptr<Packet> packet = parsePacket(b);
    int sum = recursiveVersionSum(packet);
    return sum;
}
void debugPrintRecursiveMath(int typeID, BigNumber value, int depth){
    if(DEBUG_2){
        for(int i = 0; i < depth; i++){
            cout << "\t";
        }
        switch(typeID){
            case 0:
                cout << "SUM " << value << endl;
                break;
            case 1:
                cout << "PROD " << value << endl;
                break;
            case 2:
                cout << "MIN " << value << endl;
                break;
            case 3:
                cout << "MAX " << value << endl;
                break;
            case 4:
                cout << value << endl;
                break;
            case 5:
                cout << "GREATER THAN " << value << endl;
                break;
            case 6:
                cout << "LESS THAN " << value << endl;
                break;
            case 7:
                cout << "EQUAL TO " << value << endl;
                break;
        }
    }
}
BigNumber recursivelyProcessPacket(shared_ptr<Packet> packet, int depth = 0){
    //literal
    int typeID = packet->typeID;
    if(typeID==4){
        debugPrintRecursiveMath(typeID, packet->value,depth);
        return packet->value;
    }
    // sum
    if(typeID==0){
        BigNumber sum = 0;
        for(shared_ptr<Packet> p : packet->packets){
            sum += recursivelyProcessPacket(p, depth+1); 
        }
        debugPrintRecursiveMath(typeID, sum,depth);
        return sum;
    }
    // product
    if(typeID==1){
        BigNumber prod = 1;
        for(shared_ptr<Packet> p : packet->packets){
            BigNumber a = recursivelyProcessPacket(p, depth+1);
            prod = prod * a;
        }
        debugPrintRecursiveMath(typeID, prod ,depth);
        return prod;
    }
    // minimum
    if(typeID==2){
        BigNumber minimum = MAX_VAL;
        for(shared_ptr<Packet> p : packet->packets){
            BigNumber v = recursivelyProcessPacket(p, depth+1);
            if(v < minimum){
                minimum = v;
            }   
        }
        debugPrintRecursiveMath(typeID, minimum,depth);
        return minimum;
    }
    // maximum
    if(typeID==3){
        BigNumber maximum = 0;
        for(shared_ptr<Packet> p : packet->packets){
            BigNumber v = recursivelyProcessPacket(p, depth+1);
            if(v > maximum){
                maximum = v;
            }   
        }
        debugPrintRecursiveMath(typeID, maximum,depth);
        return maximum;
    }
    // greater than
    if(typeID==5){
        BigNumber a = recursivelyProcessPacket(packet->packets.at(0), depth+1);
        BigNumber b = recursivelyProcessPacket(packet->packets.at(1), depth+1);
        debugPrintRecursiveMath(typeID, a>b,depth);
        return  a > b ;
    }
    // less than
    if(typeID==6){
        BigNumber a = recursivelyProcessPacket(packet->packets.at(0), depth+1);
        BigNumber b = recursivelyProcessPacket(packet->packets.at(1), depth+1);
        debugPrintRecursiveMath(typeID, a<b,depth);
        return a < b;
    }
    // equal to
    if(typeID==7){
        BigNumber a = recursivelyProcessPacket(packet->packets.at(0), depth+1);
        BigNumber b = recursivelyProcessPacket(packet->packets.at(1), depth+1);
        debugPrintRecursiveMath(typeID, a==b, depth);
        return a == b;
    }
    throw runtime_error("Do not know type ID: " + packet->typeID);
}
BigNumber part2(string filename){
    vector<char> data = readInput(filename);
    Bitstream b = hexStringToBits(data);
    shared_ptr<Packet> packet = parsePacket(b);
    return recursivelyProcessPacket(packet);
}

int main(){
    cout << "Day 16" << endl;
    cout << "Part 1: " << part1("input") << endl;
    cout << "Part 2: " << part2("input") << endl;
    return 0;
}