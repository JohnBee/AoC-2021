#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <exception>
#include <map>
#include <list>

#define DEBUG false

using namespace std;

struct Coord {
    int x = INT_MAX;
    int y = INT_MAX;
    Coord(){}
    Coord(int x, int y){
        this->x = x;
        this->y = y;
    }
    friend bool operator==(const Coord & lhs, const Coord & rhs){
        return (lhs.x == rhs.x && lhs.y == rhs.y);
    }
    friend bool operator<(const Coord & lhs, const Coord & rhs) {
        if(lhs.y < rhs.y){
            return true;
        }
        else if(lhs.y > rhs.y){
            return false;

        }
        else{
            if(lhs.x < rhs.x){
                return true;
            }
            else{
                return false;
            }
        }
    }
};

struct Vertex {
    Coord pos;
    int value;

    Vertex(){
        this->value = INT_MAX;
    }
    Vertex(int x, int y, int value){
        pos = Coord(x,y);
        this->value = value;
    }
    friend bool operator==(const Vertex & lhs, const Vertex & rhs){
        return(lhs.pos == rhs.pos);
    }
    friend bool operator<(const Vertex & lhs, const Vertex & rhs){
        return (lhs.pos < lhs.pos);
    }
};

struct RiskMap {
    vector<vector<Vertex>> rmap;
    void addLine(string row){
        vector<Vertex> vrow;
        for(int x = 0; x < row.size(); x++){
            vrow.push_back(Vertex(x, rmap.size(), row[x] - '0'));
        }
        rmap.push_back(vrow);
    }
    void fromIntMap(vector<vector<int>> & im){
        for(int y = 0; y < im.size(); y++){
            vector<Vertex> vrow;
            for(int x = 0; x < im.at(y).size(); x++){
                vrow.push_back(Vertex(x, y, im.at(y).at(x)));
            }
            rmap.push_back(vrow);
        }
    }

    Vertex at(int x, int y){
        return rmap.at(y).at(x);
    }
    Vertex * atPtr(int x, int y){
        return &(rmap[y][x]);
    }
    int height() const{
        return rmap.size();
    }
    int width() const {
        return rmap.at(0).size();
    }
    void print(){
        for(int y = 0; y < rmap.size(); y++){
            for(int x = 0; x < rmap.at(y).size(); x++){
                cout << at(x,y).value;
            }
            cout << endl;
        }
    }
    vector<Vertex*> getNeighbours(Vertex * v){
        vector<Vertex*> out;
        Coord pos = (*v).pos;
        // up
        if(pos.y > 0){
            out.push_back(atPtr(pos.x, pos.y-1));
        }
        // down
        if(pos.y < height()-1){
            out.push_back(atPtr(pos.x, pos.y+1));
        }
        // left
        if(pos.x > 0){
            out.push_back(atPtr(pos.x-1, pos.y));
        }
        // right
        if(pos.x < width()-1){
            out.push_back(atPtr(pos.x+1, pos.y));
        }
        return out;
    }
};

RiskMap buildInput(string filename){
    ifstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("File not found: " + filename);
    }
    RiskMap r;
    while(!file.eof()){
        string line;
        getline(file, line);
        if(line.length() > 0){
            r.addLine(line);
        }
    }
    file.close();
    return r;
}
void writePathToFile(RiskMap & rmap, list<Vertex> & path){
    ofstream file;
    file.open("map.txt");
    for(int y = 0; y < rmap.height(); y++){
        for(int x = 0; x < rmap.width(); x++){
            if(find(path.begin(), path.end(), rmap.at(x,y)) != path.end()){
                file << '#';
            }
            else{
                file << rmap.at(x,y).value;
            }

        }
        file << endl;
    }
    file.close();
}
Vertex minDist(vector<Vertex> & Q, map<Vertex, int> & dist){
    int mini = INT_MAX;
    Vertex vmin;
    for(auto s = Q.begin(); s != Q.end(); s++){
        if(dist.at(*s) < mini){
            mini = dist.at(*s);
            vmin = *s;
        }
    }
    return vmin;
}
struct comp{
    bool operator()(const pair<Vertex*, int> & lhs, const pair<Vertex* ,int> & rhs){
        return lhs.second > rhs.second; 
    }
};
list<Vertex> dijkstras(RiskMap & rmap, Vertex * source, Vertex * target){
    map<Vertex*, int> dist = {{source, 0}};
    map<Vertex*, Vertex*> prev = {{source, nullptr}};

    priority_queue<pair<Vertex*, int>, vector<pair<Vertex*, int>>, comp> Q;
    Q.push({source, 0});

    while(!Q.empty()){
        Vertex * u = Q.top().first;
        Q.pop();

        if(u == target){
            list<Vertex> S;
            if(prev.at(u) != nullptr || u == source){
                while(u != nullptr){
                    vector<int> v;
                    S.push_front(*u);
                    u = prev.at(u);
                }
            }
            return S;
        }

        vector<Vertex*> neighbours = rmap.getNeighbours(u);
        for(auto v = neighbours.begin(); v != neighbours.end(); v++){
            Vertex * next = *v;
            int alt = dist.at(u) + next->value;
            auto f = dist.find(next);
            if(f == dist.end() || alt < dist.at(next)){
                if(f != dist.end()){
                    f->second = alt;
                }
                else{
                    dist.insert({next, alt});
                } 
                auto g = prev.find(next);
                if( g != prev.end()){
                    g->second = u;
                }
                else{
                    prev.insert({next, u});
                }
                Q.push({next, alt});
            }
        }

    }
    return list<Vertex>();
}

int part1(){
    RiskMap rmap = buildInput("input_small");
    auto start = rmap.atPtr(0,0);
    auto end = rmap.atPtr(rmap.width()-1, rmap.height()-1);
    list<Vertex> path = dijkstras(rmap, start, end);
    if(DEBUG){
        writePathToFile(rmap, path);
    }
    int sum = 0;
    for(auto it = next(path.begin()); it != path.end(); it++){
        sum += it->value;
    }
    cout << "Part 1" << endl;
    cout << "Risk sum: " << sum << endl;
    return 0;
}
RiskMap buildInputPart2(string filename){
    ifstream file;
    file.open(filename);
    if(file.fail()){
        throw runtime_error("File not found: " + filename);
    }
    vector<vector<int>> m;
    while(!file.eof()){
        string line;
        getline(file, line);
        if(line.length() > 0){
            vector<int> row;
            for(int i = 0; i < 5; i++){
                for(char c : line){
                    int val = ((c - '0') + i);
                    if(val >=10){
                        val = (val % 10) + 1;
                    }
                    row.push_back(val);
                }
            }
            m.push_back(row);
        }
    }
    vector<vector<int>> expanded;
    for(int i = 0; i < 5; i++){
        for(vector<int> r : m){
            vector<int> row;
            for(int c : r){
                int val = (c + i);
                if(val >=10){
                    val = (val % 10) + 1;
                }
                row.push_back(val);
            }
            expanded.push_back(row);
        }
    }
    file.close();
    RiskMap r;
    r.fromIntMap(expanded);
    return r; 
}
list<Vertex> reconsturctPath(map<Vertex*, Vertex*> & cameFrom, Vertex* current){
    list<Vertex> path = {*current};
    while(cameFrom.count(current)){
        current = cameFrom.at(current);
        if(current != nullptr){
            path.push_front(*current);
        }   
    }
    return path;
}

list<Vertex> part2AStar(RiskMap & rmap, Vertex * start, Vertex * goal){
    auto h = [=](Vertex current, Vertex goal){
        return (abs(current.pos.x - goal.pos.x) + abs(current.pos.y - goal.pos.y));
    };
    priority_queue<pair<Vertex*, int>, vector<pair<Vertex*, int>>, comp> frontier;
    frontier.push({start, h(*start, *goal)});
    map<Vertex*, int> costSoFar = {{start, 0}};
    map<Vertex*, Vertex*> cameFrom = {{start , nullptr}};

    while(!frontier.empty()){
        Vertex * current = frontier.top().first;
        frontier.pop();

        if(current == goal){
            if(DEBUG){
                cout << "Goal!" << endl;
            }
            return reconsturctPath(cameFrom, current);
        }

        vector<Vertex*> neighbours = rmap.getNeighbours(current);
        for(auto it = neighbours.begin(); it != neighbours.end(); it++){
            int newCost = costSoFar.at(current) + (*it)->value;
            auto f = costSoFar.find(*it);
            if(f == costSoFar.end() || newCost < (f->second)){
                if(f==costSoFar.end()){
                    costSoFar.insert({*it, newCost});
                }
                else{
                    f->second = newCost;
                }
                frontier.push({(*it), newCost + h(*(*it), *goal)});
                auto v = **it;
                auto g = cameFrom.find(*it);
                if(g == cameFrom.end()){
                    cameFrom.insert({*it, current});
                }
                else{
                    g->second = current;
                }
            } 
        }
    }
    // fail
    return list<Vertex>();
}
int part2(){
    cout << "Part 2" << endl;
    cout << "[Working] Building map" << endl;
    RiskMap r = buildInputPart2("input");
    auto start = r.atPtr(0,0);
    auto end = r.atPtr(r.width()-1, r.height()-1);
    cout << "[Working] Calculating..." << endl;
    list<Vertex> path = part2AStar(r, start, end);
    if(DEBUG){
        writePathToFile(r, path);
    }
    int sum = 0;
    for(auto v = next(path.begin()); v != path.end(); v++){
        sum+=v->value;
    }
    cout << "Risk sum: " << sum << endl;
    return 0;
}
int main(){
    cout << "Day 15" << endl;
    
    part1();
    part2();
    
    return 0;
}