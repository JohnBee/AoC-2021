#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

struct Coord{
    int x;
    int y;
    Coord(int x, int y){
        this->x = x;
        this->y = y;
    }
    friend bool operator==(const Coord & lhs, const Coord & rhs){
        return (lhs.x == rhs.x && lhs.y == rhs.y);
    }
};

struct HeightMap {
    vector<vector<int>> map;
    void addRow(string srow){
        vector<int> row;
        for(char c : srow){
            row.push_back(c-'0');
        }
        map.push_back(row);
    }
    void addRow(vector<int> irow){
        map.push_back(irow);
    }
    int at(int x, int y) const {
        return map.at(y).at(x);
    }
    bool inBounds(int x, int y) const{
        return (x >= 0 && y >= 0 && y < getHeight() && x < getWidth());
    }
    int getWidth() const {
        if(map.size() > 0){
            return map.at(0).size();
        }
        else{
            return 0;
        }
    }
    int getHeight() const {
        return map.size();
    }
    deque<Coord> getAdjacent(Coord co){
        deque<Coord> out;
        if(inBounds(co.x+1, co.y) && at(co.x+1, co.y)!=9){
            out.push_back(Coord(co.x+1, co.y));
        }
        if(inBounds(co.x-1, co.y) && at(co.x-1, co.y)!=9){
            out.push_back(Coord(co.x-1, co.y));
        }
        if(inBounds(co.x, co.y+1) && at(co.x, co.y+1)!=9){
            out.push_back(Coord(co.x, co.y+1));
        }
        if(inBounds(co.x, co.y-1) && at(co.x, co.y-1)!=9){
            out.push_back(Coord(co.x, co.y-1));
        }
        return out;
    }
    bool isLowPoint(int x, int y) const{
        int curh = at(x, y);
        if(inBounds(x-1, y) && at(x-1, y) <= curh){
            return false;
        }
        if(inBounds(x+1, y) && at(x+1, y) <= curh){
            return false;
        }
        if(inBounds(x, y-1) && at(x, y-1) <= curh){
            return false;
        }
        if(inBounds(x, y+1) && at(x, y+1) <= curh){
            return false;
        }
        return true;
    }
};


HeightMap buildInput(string filename){
    ifstream file;
    file.open(filename);
    if(file.fail()){
        cout << "Cannot open file: " << filename << endl;
        exit(1);
    }
    string line;
    HeightMap hmap;
    while(!file.eof()){
        getline(file, line);
        if(line.length() > 0){
            hmap.addRow(line);
        }
    }
    return hmap;
}

int part1(HeightMap hmap){
    int out = 0;
    for(int y = 0; y < hmap.getHeight(); y++){
        for(int x = 0; x < hmap.getWidth(); x++){
            if(hmap.isLowPoint(x,y)){
                out += hmap.at(x,y)+1;
            }
        }
    }
    return out;
}
pair<deque<Coord>, int> bfs_area_size(HeightMap hmap, Coord start){
    deque<Coord> toVisit = {start};
    deque<Coord> visited = {start};
    while(toVisit.size() > 0){
        Coord current = toVisit.front();
        toVisit.pop_front();

        for(Coord c : hmap.getAdjacent(current)){
            if(find(visited.begin(),visited.end(), c) == visited.end()){
                visited.push_back(c);
                toVisit.push_back(c);
            }
        }
    }
    return make_pair(visited, visited.size()); // size of the zone

}


int part2(HeightMap hmap){
    // use bfs, flood area, when no more to visit area size in length of visited list.
    vector<Coord> toCheckCoords;
    vector<int> areaSizes;
    for(int y = 0; y < hmap.getHeight(); y++){
        for(int x = 0; x < hmap.getWidth(); x++){
            if(hmap.at(x,y) != 9){
                toCheckCoords.push_back(Coord(x,y));
            }
        }
    }
    while(toCheckCoords.size() > 0){
        Coord next = toCheckCoords.back();
        pair<deque<Coord>, int> bfs_result = bfs_area_size(hmap, next);
        areaSizes.push_back(bfs_result.second);
        for(Coord c : bfs_result.first){
            toCheckCoords.erase(find(toCheckCoords.begin(), toCheckCoords.end(),c));
        }
    }
    sort(areaSizes.begin(), areaSizes.end());
    if(areaSizes.size() < 3){
        cout << "Didn't find at least 3 basins";
        exit(1);
    }
    return areaSizes[areaSizes.size()-1]*areaSizes[areaSizes.size()-2]*areaSizes[areaSizes.size()-3];
}

int main(){
    cout << "Day 9" << endl;
    HeightMap hmap = buildInput("input");
    cout << "Part 1: " << part1(hmap) << endl;
    cout << "Part 2: " << part2(hmap) << endl;
    return 0;
}