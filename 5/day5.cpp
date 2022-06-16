#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include <map>
#include <math.h>

using namespace std;

struct Coord {
    int x;
    int y;
    Coord(int x, int y){
        this->x = x;
        this->y = y;
    }
    friend bool operator<(const Coord& lhs, const Coord& rhs){
        return tie(lhs.x, lhs.y) < tie(rhs.x, rhs.y);
    }
    friend bool operator==(const Coord& lhs, const Coord& rhs){
        return lhs.x == rhs.x && lhs.y == lhs.y;
    }
    friend bool operator> (const Coord& lhs, const Coord& rhs){ return rhs < lhs; }
    friend bool operator<=(const Coord& lhs, const Coord& rhs){ return !(lhs > rhs); }
    friend bool operator>=(const Coord& lhs, const Coord& rhs){ return !(lhs < rhs); }
};

typedef pair<Coord, Coord> line;

line getLineFromString(string s){
    regex s_regex("[0-9]+");
    vector<string> out(sregex_token_iterator(s.begin(), s.end(), s_regex, 0), sregex_token_iterator());
    vector<int> rawCoords;
    for(string cs : out){
        rawCoords.push_back(stoi(cs));
    }
    if(rawCoords.size() < 4){
        cout << "Error parsing string: " << s << endl;
        exit(1);
    }
    Coord c1(rawCoords.at(0), rawCoords.at(1));
    Coord c2(rawCoords.at(2), rawCoords.at(3));
    return make_pair(c1, c2);
}

vector<line> buildInput(){
    ifstream file;
    file.open("input");
    if(file.fail()){
        cout << "Cannot read file." << endl;
        exit(1);
    }
    vector<line> out;
    while(!file.eof()){
        string line;
        getline(file, line);
        if(line.length() > 0){
            out.push_back(getLineFromString(line));
        }
    }
    file.close();
    return out;
}


void plotLineHigh(int x0, int y0, int x1, int y1, map<Coord, int> &coordHitMap){
    //Bresnehams line algorithm
            // plotLineHigh(x0, y0, x1, y1)
            // dx = x1 - x0
            // dy = y1 - y0
            // xi = 1
            // if dx < 0
            //     xi = -1
            //     dx = -dx
            // end if
            // D = (2 * dx) - dy
            // x = x0

            // for y from y0 to y1
            //     plot(x, y)
            //     if D > 0
            //         x = x + xi
            //         D = D + (2 * (dx - dy))
            //     else
            //         D = D + 2*dx
            //     end if

            int dx = x1 - x0;
            int dy = y1 - y0;

            int xi = 1;
            if(dx < 0){
                xi = -1;
                dx = -dx;
            }
            int D = (2 * dx) - dy;
            int x = x0;
            for(int y = y0; y <= y1; y++){
                if(coordHitMap.count(Coord(x, y))){
                    coordHitMap[Coord(x, y)] += 1;
                }
                else{
                    coordHitMap[Coord(x, y)] = 1;
                }
                if(D > 0){
                    x = x + xi;
                    D = D + (2 * (dx - dy));
                }
                else{
                    D = D + 2*dx;
                }
            }
            
}
void plotLineLow(int x0, int y0, int x1, int y1, map<Coord, int> &coordHitMap){
    // dx = x1 - x0
    // dy = y1 - y0
    // yi = 1
    // if dy < 0
    //     yi = -1
    //     dy = -dy
    // end if
    // D = (2 * dy) - dx
    // y = y0

    // for x from x0 to x1
    //     plot(x, y)
    //     if D > 0
    //         y = y + yi
    //         D = D + (2 * (dy - dx))
    //     else
    //         D = D + 2*dy
    //     end if

    int dx = x1 - x0;
    int dy = y1 - y0;

    int yi = 1;
    if(dy < 0){
        yi = -1;
        dy = -dy;
    }
    int D = (2 * dy) - dx;
    int y = y0;
    for(int x = x0; x <= x1; x++){
        if(coordHitMap.count(Coord(x, y))){
            coordHitMap[Coord(x, y)] += 1;
        }
        else{
            coordHitMap[Coord(x, y)] = 1;
        }
        if(D > 0){
            y = y + yi;
            D = D + (2 * (dy - dx));
        }
        else{
            D = D + 2*dy;
        }
    }
}
void plotLine(line l, map<Coord, int> & coordHitMap){
    // plotLine(x0, y0, x1, y1)
    // if abs(y1 - y0) < abs(x1 - x0)
    //     if x0 > x1
    //         plotLineLow(x1, y1, x0, y0)
    //     else
    //         plotLineLow(x0, y0, x1, y1)
    //     end if
    // else
    //     if y0 > y1
    //         plotLineHigh(x1, y1, x0, y0)
    //     else
    //         plotLineHigh(x0, y0, x1, y1)
    //     end if
    // end if
    int x0 = l.first.x;
    int x1 = l.second.x;
    int y0 = l.first.y;
    int y1 = l.second.y;

    if(abs(y1 - y0) < abs(x1 - x0)){
        if(x0 > x1){
            plotLineLow(x1, y1, x0, y0, coordHitMap);
        }
        else{
            plotLineLow(x0, y0, x1, y1, coordHitMap);
        }
        
    }
    else{
        if(y0 > y1){
            plotLineHigh(x1, y1, x0, y0, coordHitMap);
        }
        else{
            plotLineHigh(x0, y0, x1, y1, coordHitMap);
        }
    }
}
void drawHitMap(map<Coord, int> &coordHitMap){
    //find furthest x and y coordinates
    int bottomX = 0;
    int bottomY = 0;
    cout << endl;
    for(auto hmi  = coordHitMap.begin(); hmi != coordHitMap.end(); hmi++){
        if(hmi->first.x > bottomX){
            bottomX = hmi->first.x;
        }
        if(hmi->first.y > bottomY){
            bottomY = hmi->first.y;
        }
    }
    for(int y = 0; y <= 9; y++){
        for(int x = 0; x <= 9; x++){
            if(coordHitMap.count(Coord(x,y)) > 0){
                cout << coordHitMap[Coord(x, y)];
            }
            else {
                cout << ".";
            }
        }
        cout << endl;
    }
}
void buildCoordHitMapPart1(vector<line> & input, map<Coord, int> &coordHitMap){
    for(line l : input){
        if(l.first.x == l.second.x || l.first.y == l.second.y){
            // cout << l.first.x << ", " << l.first.y << " => " << l.second.x << ", " << l.second.y << endl;
            plotLine(l, coordHitMap);
            // drawHitMap(coordHitMap);
            // cout << endl;
        }
    }
    return;
}
void buildCoordHitMapPart2(vector<line> & input, map<Coord, int> &coordHitMap){
    for(line l : input){
        // cout << l.first.x << ", " << l.first.y << " => " << l.second.x << ", " << l.second.y << endl;
        plotLine(l, coordHitMap);
        // drawHitMap(coordHitMap);
        // cout << endl;
    }
    return;
}


int part1(vector<line> & input){
    map<Coord, int> coordHitMap;

    buildCoordHitMapPart1(input, coordHitMap);
    int count = 0;
    for(auto hmi = coordHitMap.begin(); hmi != coordHitMap.end(); hmi++){
        if(hmi->second > 1){
            //cout << hmi->first.x << " " << hmi->first.y << endl;
            count++;
        }
    }
    return count;
}
int part2(vector<line> & input){
    map<Coord, int> coordHitMap;

    buildCoordHitMapPart2(input, coordHitMap);
    int count = 0;
    for(auto hmi = coordHitMap.begin(); hmi != coordHitMap.end(); hmi++){
        if(hmi->second > 1){
            //cout << hmi->first.x << " " << hmi->first.y << endl;
            count++;
        }
    }
    return count;
}



int main(){
    cout << "day 5" << endl;
    vector<line> input = buildInput();
    cout << "Part 1: " << part1(input) << endl;
    cout << "Part 2: " << part2(input) << endl;
    return 0;
}