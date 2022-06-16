#include <iostream>
#include <vector>
#include <regex>
#include <string>
#include <map>
#include <algorithm>
#include <set>
#include <fstream>

#define DEBUG false

using namespace std;
struct Coord {
    unsigned int x;
    unsigned int y;
    Coord(unsigned int x, unsigned int y){
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

struct Fold {
    char direction;
    int pos;
    Fold(char d, int p){
        direction = d;
        pos = p;
    }
};

struct Paper{
    set<Coord> points;
    vector<Fold> folds;
    void addPoint(unsigned int x, unsigned int y){
        points.insert(Coord(x,y));
    }
    void delPoint(unsigned int x, unsigned int y){
        points.erase(find(points.begin(), points.end(), Coord(x,y)));
    }
    void delPoint(Coord p){
        points.erase(find(points.begin(), points.end(), p));
    }
    void addPoint(Coord p){
        points.insert(p);
    }
    void addFold(char d, int p){
        folds.push_back(Fold(d,p));
    }
    
    void printPaper(){
        int largestx = 0;
        int largesty = 0;
        for(Coord p : points){
            if(p.x > largestx){
                largestx = p.x;
            }
            if(p.y > largesty){
                largesty = p.y;
            }
        }
        for(int y = 0; y <= largesty; y++){
            for(int x = 0; x <= largestx; x++){
                if(points.count(Coord(x,y))){
                    cout << '#';
                }
                else{
                    cout << '.';
                }
            }
            cout << endl;
        }
    }
    void movePoint(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2){
        delPoint(x1,y1);
        addPoint(x2,y2);
    }
    void verticalFold(int pos){
        vector<Coord> removeList;
        for(Coord p : points){
            if(p.x > pos){
                addPoint(p.x-(p.x-pos)*2,p.y);
                removeList.push_back(p);
            }
        }
        for(Coord r : removeList){
            delPoint(r);
        }
    }
    void horizontalFold(int pos){
        vector<Coord> removeList;
        for(Coord p : points){
            if(p.y > pos){
                addPoint(p.x,p.y-(p.y-pos)*2);
                removeList.push_back(p);
            }
        }
        for(Coord r : removeList){
            delPoint(r);
        }
    }
    void doAllFolds(){
        if(DEBUG){
            cout << "Start" << endl;
            printPaper();
            cout << endl;
        }
        for(Fold f: folds){
            if(f.direction=='x'){
                verticalFold(f.pos);
            }
            else{
                horizontalFold(f.pos);
            }
            if(DEBUG){
                printPaper();
                cout << endl;
            }
            
        }
    }
    void oneFold(){
        if(DEBUG){
            cout << "Start" << endl;
            printPaper();
            cout << endl;
        }
        Fold f = folds.front();
        if(f.direction=='x'){
            verticalFold(f.pos);
        }
        else{
            horizontalFold(f.pos);
        }
        if(DEBUG){
            printPaper();
            cout << endl;
        }
        
    }
};
vector<string> parseString(string delim, string input){
    regex d(delim);
    vector<string> out(sregex_token_iterator(input.begin(), input.end(), d, -1), sregex_token_iterator());
    return out;
}


Paper buildInput(string filename){
    ifstream file;
    file.open(filename);
    if(file.fail()){
        cout << "Cannot open file: " << filename << endl;
        exit(1);
    }
    unsigned int readingState = 0;
    Paper paper;
    while(!file.eof()){
        string line;
        getline(file, line);
        if(line.length() == 0){
            readingState++;
        }
        else{
            if(readingState == 0){
                vector<string> coords = parseString(",",line);
                paper.addPoint(stoi(coords.at(0)), stoi(coords.at(1)));
            }
            if(readingState == 1){
                vector<string> foldstrings = parseString(" ",line);
                vector<string> fold = parseString("=",foldstrings.at(2));
                paper.addFold(fold.at(0).at(0), stoi(fold.at(1)));
            }
        }
    }
    file.close();
    return paper;
}

int part1(Paper & p){
    p.oneFold();
    return(p.points.size());
}
void part2(Paper & p){
    p.doAllFolds();
    p.printPaper();
}

int main(){
    cout << "Day 13" << endl;
    Paper p = buildInput("input");
    cout << "Part 1: " << part1(p) << endl;
    cout << "Part 2" << endl;
    part2(p);
    return 0;
}