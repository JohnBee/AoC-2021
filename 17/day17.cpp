#include <iostream>
#include <regex>
#include <vector>
#include <fstream>
#include <string>
#include <exception>
#include <algorithm>
#include <cmath>
#include <set>

using namespace std;

#define DEBUG true

struct Coord {
    int x = 0;
    int y = 0;
    Coord() = default;
    Coord(int x, int y){
        this->x = x;
        this->y = y;
    }
    friend bool operator==(const Coord & lhs, const Coord & rhs){
        return (lhs.x == rhs.x && lhs.y == rhs.y);
    }
    friend bool operator<(const Coord & lhs, const Coord & rhs){
        if(lhs.y < rhs.y){
            return true;
        }
        else if(lhs.y == rhs.y){
            if(lhs.x < rhs.x){
                return true;
            }
            return false;
        }
        return false;
    }
};

struct Zone {
    int x1,x2,y1,y2;
    Zone(int x1, int x2, int y1, int y2){
        if(x1 < x2){
            this->x1 = x1;
            this->x2 = x2;
        }
        else {
            this->x1 = x2;
            this->x2 = x1;
        }
        if(y1 > y2){
            this->y1 = y1;
            this->y2 = y2;
        }
        else {
            this->y1 = y2;
            this->y2 = y1;
        } 
    }
};
vector<string> getNumbers(string s){
    regex d("-?[1-9][0-9]*");
    vector<string> out(sregex_token_iterator(s.begin(), s.end(), d, 0), sregex_token_iterator());
    return out;
}
Zone buildInput(string filename){
    ifstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("Cannot open file: " + filename);
    }
    string line;
    getline(file, line);
    file.close();
    if(line.length() == 0){
        throw runtime_error(filename + " is empty.");
    }
    vector<string> snumbers = getNumbers(line);
    if(snumbers.size() != 4){
        throw runtime_error(filename + " does not contain enough coordinates.");
    }
    vector<int> coords;
    for_each(snumbers.begin(), snumbers.end(), [&coords](string numb){
        coords.push_back(stoi(numb));
    });
    Zone z(coords.at(0), coords.at(1), coords.at(2), coords.at(3));
    return z;
}
pair<vector<Coord>, int> runVelocity(Zone r, int velX, int velY){
    // returns 0 if landed in zone
    // returns 1 if over shot
    // return -1 if under shot
    int curX = 0;
    int curY = 0;

    vector<Coord> positions;
    positions.push_back(Coord(curX, curY));
    while(curX <= r.x2 && curY >= r.y2){
        if(curX >= r.x1 && curX <= r.x2 && curY <= r.y1 && curY >= r.y2){
            return {positions, 0};
        }
        curX += velX;
        curY += velY;
        if(velX > 0){
            velX -=1;
        }
        else if(velX < 0){
            velX += 1;
        }
        velY -= 1;
        positions.push_back(Coord(curX, curY));
    }
    int underOverShot;
    if(curX < r.x1){
        underOverShot = -1;
    }
    else{
        underOverShot = 1;
    }
    return {positions, underOverShot};
}
void printRun(Zone z, vector<Coord> pos){
    Coord topLeft(INT_MAX, INT_MIN);
    Coord botRight(INT_MIN,INT_MAX);
    
    for(Coord c : pos){
        if(c.x < topLeft.x){
            topLeft.x = c.x;
        }
        if(c.x > botRight.x){
            botRight.x = c.x;
        }
        if(c.y > topLeft.y){
            topLeft.y = c.y;
        }
        if(c.y < botRight.y){
            botRight.y = c.y;
        }
    }
    if(topLeft.x > z.x1){
        topLeft.x = z.x1;
    }
    if(topLeft.y < z.y1){
        topLeft.y = z.y1;
    }
    if(botRight.x < z.x2){
        botRight.x = z.x2;
    }
    if(botRight.y > z.y2){
        botRight.y = z.y2;
    }
    ofstream file;
    file.open("visualise.txt");

    for(int y = topLeft.y; y >= botRight.y; y--){
        for(int x = topLeft.x; x <= botRight.x; x++){
            if(find(pos.begin(), pos.end(), Coord(x,y)) != pos.end()){
                file << "#";
            }
            else if(x >= z.x1 && x <= z.x2 && y <= z.y1 && y >= z.y2 ){
                file << 'T';
            }
            else if(x == 0 && y == 0){
                file << "S";
            }
            else{
                file << '.';
            }
        }
        file << endl;
    }
    file.close();
}
int calculateBestX(int targetX){
    // triangle numbers
    // (n^2 + n)/2 = z.x1

    // quadratic formula
    // n^2 + n - 2*z.x1 = 0

    // a = 1
    // b = 1
    // c = -2 * z.x1
    // (-1 + sqrt(1 + 8 * z.x1)) / 2
    double j = (1 + 8 * targetX);
    double a1 = ((-1 + sqrt(j)) / 2);
    double b1 = ((-1 - sqrt(j)) / 2);
    int a = round(a1);
    int b = round(b1);
    return (a > b) ? a : b;
}
int calculateBestY(int targetY){
    return abs(targetY)-1;
}
int highestY(vector<Coord> pos){
    int high = INT_MIN;
    for(Coord c : pos){
        if(c.y > high){
            high = c.y;
        }
    }
    return high;
}
int part1(string filename){
    Zone z = buildInput(filename);
    int x = calculateBestX(z.x2);
    int y = calculateBestY(z.y2);
    
    pair<vector<Coord>, int> out = runVelocity(z, x, y);
    if(DEBUG){
        cout << "Best x: " << x << " Best y: " << y << endl;
        cout << "Final x: " << out.first.back().x << " y: " << out.first.back().y << endl;
        cout << "Highest y: " << highestY(out.first) << endl;
        printRun(z, out.first);
    }
    return highestY(out.first);
}
bool goesThroughBoxX(Zone z, int x){
    int start = x;
    int sum = 0;
    while(start > 0){
        sum += start;
        start -=1;
        if(sum >= z.x1 && sum <= z.x2){
            return true;
        }
        if(sum > z.x2){
            return false;
        }
    }
    if(sum < z.x1){
        return false;
    }
    return true;

}
bool goesThroughBoxY(Zone z, int y){
    int start = y;
    int sum = 0;
    while(sum >= z.y2){
        sum += start;
        start -= 1;
        if(sum <= z.y1 && sum >= z.y2){
            return true;
        }
        if(sum < z.y2){
            return false;
        }
    }
    return true;
}
int part2(string filename){
    Zone z = buildInput(filename);
    set<Coord> possibleVelocity;
    set<int> possibleX;
    set<int> possibleY;

    for(int x = 0; x <= z.x2; x++){
        if(goesThroughBoxX(z, x)){
            possibleX.insert(x);
        }
    }
    for(int y = 0; y >= z.y2; y--){
        if(goesThroughBoxY(z, y)){
            possibleY.insert(y);
        }
        if(goesThroughBoxY(z, -y)){
            possibleY.insert(-y);
        }
    }
    vector<Coord> possibleVectors;
    for(int y : possibleY){
        for(int x : possibleX){
            pair<vector<Coord>, int> out = runVelocity(z, x, y);
            if(out.second == 0){
                possibleVectors.push_back(Coord(x, y));
            }
        }
    }
    pair<vector<Coord>, int> out = runVelocity(z, 6, 10);
    printRun(z, out.first);
    return possibleVectors.size();
    // for all numbers less than z.x2
    // 
}

int main(){
    cout << "Day 17" << endl;
    // cout << "Part 1: " << part1("input_test") << endl;
    // cout << "Part 2: " << part2("input_test") << endl;
    cout << part2("input") << endl;
    return 0;
}