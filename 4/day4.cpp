#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <map>

using namespace std;

typedef vector<int> bingoLine;

typedef vector<bingoLine> bingoBoard;
typedef vector<bingoBoard> bingoBoardSet;
typedef vector<int> bingoInput; 

vector<string> regexMatch(string s, string regex_delim){
    regex s_regex(regex_delim);
    vector<string> out(sregex_token_iterator(s.begin(), s.end(), s_regex, 0), sregex_token_iterator());
    return out;
}

pair<bingoInput, bingoBoardSet> buildInput(){
    ifstream file;
    file.open("input");
    if(file.fail()){
        cout << "Cannot not open input" << endl;
        exit(1);
    }
    string line;
    // first line is bingo calls
    getline(file, line);
    vector<string> rawNumbers = regexMatch(line, "[0-9]+");
    bingoInput bi;
    for(auto s : rawNumbers){
        bi.push_back(stoi(s));
    }

    // read sets of 5 lines
    bingoBoardSet boardSet;
    getline(file, line); // Skip blank line;
    

    while(!file.eof()){
        bingoBoard bb;
        for(int i = 0; i < 5; i++){
            getline(file, line);
            vector<string> rawNumbers = regexMatch(line, "[0-9]+");
            bingoLine bl;
            for(auto s : rawNumbers){
                bl.push_back(stoi(s));
            }
            bb.push_back(bl);
        }
        boardSet.push_back(bb);
        getline(file, line); // skip blank line;
    }
    return make_pair(bi, boardSet);
}


pair<bingoLine, bingoBoard> findWinnerBoard(const bingoInput & bi, const bingoBoardSet & bbs, map<int, int> & m){
    // create bingo call to position map, the lower the position the earlier it gets called.
    

    // for each bingo board, and row, calculate which row will be done first
    bingoBoard lowestBB;
    bingoLine lowestBL;
    int fastestCompletion = INT_MAX;
    for(bingoBoard bb : bbs){
        // per row
        for(bingoLine bl : bb){
            // calculate completion time;
            int completionTime = 0;
            for(int i : bl){
                if(m[i] > completionTime){
                    completionTime = m[i];
                }   
            }
            if(completionTime < fastestCompletion){
                lowestBB = bb;
                lowestBL = bl;
                fastestCompletion = completionTime;
            }
        }
        // per column
        for(int column = 0; column < bb.size(); column++){
            int completionTime = 0;
            for(int row = 0; row < bb[column].size(); row++){
                if(m[bb[row][column]] > completionTime){
                    completionTime = m[bb[row][column]];
                }   
            }
            if(completionTime < fastestCompletion){
                lowestBB = bb;
                // build bingo line using column
                bingoLine bl;
                for(int row = 0; row < bb[column].size(); row++){
                    bl.push_back(bb[row][column]);
                }
                lowestBL = bl;
                fastestCompletion = completionTime;
            }
        }

    }
    return make_pair(lowestBL, lowestBB);
}
pair<bingoLine, bingoBoard> findLoserBoard(const bingoInput & bi, const bingoBoardSet & bbs, map<int, int> & m){
    // go through all the boards, find the first index inwhich this board get its first line
    // if this index is higher than the last, then update, return board.
    
    bingoBoard highestBB;
    bingoLine highestBL;
    bingoBoard lowestBB;
    bingoLine lowestBL;
    int slowestCompletion;
    map<int, pair<bingoLine, bingoBoard>> fastestLoserMap;
    for(bingoBoard bb: bbs){
        // find the earliest time when this board wins.
        // per row
        int fastestCompletion = INT_MAX;
        for(bingoLine bl : bb){
            // calculate completion time;
            int completionTime = 0;
            for(int i : bl){
                if(m[i] > completionTime){
                    completionTime = m[i];
                }   
            }
            if(completionTime < fastestCompletion){
                lowestBB = bb;
                lowestBL = bl;
                fastestCompletion = completionTime;
            }
        }
        // per column
        for(int column = 0; column < bb.size(); column++){
            int completionTime = 0;
            for(int row = 0; row < bb[column].size(); row++){
                if(m[bb[row][column]] > completionTime){
                    completionTime = m[bb[row][column]];
                }   
            }
            if(completionTime < fastestCompletion){
                lowestBB = bb;
                // build bingo line using column
                bingoLine bl;
                for(int row = 0; row < bb[column].size(); row++){
                    bl.push_back(bb[row][column]);
                }
                lowestBL = bl;
                fastestCompletion = completionTime;
            }
        }
        // use the fastest completion index
        // fastestLoserMap[fastestCompletion] = make_pair(lowestBL, lowestBB);
        if(fastestCompletion > slowestCompletion){
            highestBB = lowestBB;
            highestBL = lowestBL;
            slowestCompletion = fastestCompletion;
        }
    }

    return make_pair(highestBL, highestBB);
}

int sumUncalled(const bingoBoard & bb, map<int, int> & m, int finalIndex){
    // for every number on the bingo card, check if it got called, calculate uncalled sum
    int uncalledNumberSum = 0;
    for(bingoLine t_bl : bb){
        for(int numb : t_bl){
            if(m[numb] > finalIndex){
                // number did not get called, sum it;
                uncalledNumberSum += numb;
            }
        }
    } 
    return uncalledNumberSum;
}

int part1(pair<bingoInput, bingoBoardSet> bingo){
    map<int, int> m;
    bingoInput bi = bingo.first;
    bingoBoardSet bbs = bingo.second;

    for(int i = 0; i < bi.size(); i++){
        m[bi[i]] = i;
    }

    pair<bingoLine, bingoBoard> blbb = findWinnerBoard(bi, bbs, m);
    bingoLine lowestBL = blbb.first;
    bingoBoard lowestBB = blbb.second;
    
    // find all called and uncalled numbers on the board
    // find where the last number is called before winning
    int finalIndex = 0;
    for(auto i : lowestBL){
        if(m[i] > finalIndex){
            finalIndex = m[i];
        }
    }
    int uncalledNumberSum = sumUncalled(lowestBB, m, finalIndex);
    return uncalledNumberSum * bi[finalIndex];

}
int part2(pair<bingoInput, bingoBoardSet> bingo){
    map<int, int> m;
    bingoInput bi = bingo.first;
    bingoBoardSet bbs = bingo.second;

    for(int i = 0; i < bi.size(); i++){
        m[bi[i]] = i;
    }

    pair<bingoLine, bingoBoard> blbb = findLoserBoard(bi, bbs, m);
    bingoLine highestBL = blbb.first;
    bingoBoard highestBB = blbb.second;
    
    // find all called and uncalled numbers on the board
    // find where the last number is called before winning
    int finalIndex = 0;
    for(auto i : highestBL){
        if(m[i] > finalIndex){
            finalIndex = m[i];
        }
    }
    int uncalledNumberSum = sumUncalled(highestBB, m, finalIndex);
    return uncalledNumberSum * bi[finalIndex];

}

int main(){
    cout << "Day 4" << endl;
    pair<bingoInput, bingoBoardSet> bingo = buildInput();
    cout << part1(bingo) << endl;
    cout << part2(bingo) << endl;
    return 0;
}