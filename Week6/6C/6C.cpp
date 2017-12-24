#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <deque>

using namespace std;

struct Case {
    int h; // max height
    int w; // source height
    int** g; // grid/garden
    int srow; // star row
    int scol; // star col
};


// readers
vector<Case> readInput();
vector<Case> readInputFile();

// writers
void writeOutput(vector<int> &answers);
void writeOutputFile(vector<int> &answers);

// helpers
vector<int> solveProblem(vector<Case> &cases);
int solveCase(Case &thisCase);

// file names
string inFileName = "in.txt";
string outFileName = "out.txt";


int main() {
//    vector<Case> cases = readInputFile();
//    vector<Case> cases = readInput();

//    vector<int> answers = solveProblem(cases);




//    writeOutput(answers);
//    writeOutputFile(answers);
    return 0;
}


// helpers
int getFlowVal(int i, int j, vector<vector<int>> &garden){
    return -1 * garden[i][j] - 1;
}

bool isValid(int i, int j){
    return i >= 0 && i <= 3 && j >= 0 && j <= 3;
}

bool isFlower(int i, int j, vector<vector<int>> &garden){
    return garden[i][j] >= 0 && garden[i][j] <= 6;
}

bool isSource(int i, int j, vector<vector<int>> &garden){
    return garden[i][j] < 0;
}

bool isActive(int i, int j, vector<vector<int>> &newgarden, Case &thisCase){
    return newgarden[i][j] >= 0 && newgarden[i][j] <= thisCase.h;
}

bool isDecisionPoint(int i, int j, vector<vector<int>> &garden){
    return garden[i][j] == 8;
}




vector<vector<int>> getGarden(Case &thisCase){
    vector<vector<int>> garden(4);
    for(int i = 0; i < 4; i++){
        vector<int> v(4);
        garden[i] = v;
    }

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            garden[i][j] = thisCase.g[i][j];
        }
    }

    return garden;
}

vector<vector<int>> getLocations(vector<vector<int>> &garden){
    vector<vector<int>> rv;
    for(int j = 0; j < 4; j++){ // for each col
        for(int i = 0; i < 4; i++){ // for each row
            if(isDecisionPoint(i,j,garden)){
                vector<int> v({i, j});
                rv.push_back(v);
            }
        }
    }
    return rv;
}

deque<deque<int>> generateSolutions(int index, vector<vector<int>> &locations, vector<vector<int>> &garden, Case &thisCase){
    // base case 1
    if(index == locations.size()){
        deque<deque<int>> d;
        return d;
    }

    // base case 2
    if(index == locations.size()-1){
        deque<deque<int>> rv(thisCase.h+1);
        int c = 0;
        for(int i = 0; i <= thisCase.h; i++){
            deque<int> d;
            d.push_front(i);
            rv[c] = d;
            c++;
        }
        return rv;
    }


    // loop case
    deque<deque<int>> therest = generateSolutions(index+1, locations, garden, thisCase);
    deque<deque<int>> rv(therest.size() * (thisCase.h+1));
    int c = 0;
    for(int i = 0; i <= thisCase.h; i++){
        for(int j = 0; j < therest.size(); j++) {
            deque<int> d(therest[j]);
            d.push_front(i);
            rv[c] = d;
            c++;
        }
    }

    return rv;
}

bool testSolution(deque<int> &solution, vector<vector<int>> &indices, vector<vector<int>> &garden, Case &thisCase){
    vector<vector<int>> newgarden(garden);
    for(int i = 0; i < solution.size(); i++){
        newgarden[indices[i][0]][indices[i][1]] = solution[i];
    }

    vector<vector<bool>> visited(4);
    for(int i = 0; i < 4; i++){
        vector<bool> v(4);
        visited[i] = v;
    }

    deque<vector<int>> queue;
    vector<int> v({thisCase.srow, thisCase.scol});
    queue.push_back(v);

    while(!queue.empty()){
        vector<int> node;
        node = queue.front();
        queue.pop_front();

        int row = node[0];
        int col = node[1];
        visited[row][col] = true;

        // up

        if(isValid(row-1,col) && !visited[row-1][col] && isActive(row-1,col,newgarden,thisCase) && newgarden[row][col] >= newgarden[row-1][col]){
            vector<int> nodeup({row-1, col});
            queue.push_back(nodeup);
        }

        // down
        if(isValid(row+1,col) && !visited[row+1][col] && isActive(row+1,col,newgarden,thisCase) && newgarden[row][col] >= newgarden[row+1][col]){
            vector<int> nodedown({row+1, col});
            queue.push_back(nodedown);
        }

        // left
        if(isValid(row,col-1) && !visited[row][col-1] && isActive(row,col-1,newgarden,thisCase) && newgarden[row][col] >= newgarden[row][col-1]){
            vector<int> nodeleft({row, col-1});
            queue.push_back(nodeleft);
        }

        // right
        if(isValid(row,col+1) && !visited[row][col+1] && isActive(row,col+1,newgarden,thisCase) && newgarden[row][col] >= newgarden[row][col+1]){
            vector<int> noderight({row, col+1});
            queue.push_back(noderight);
        }
    }

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(isFlower(i,j,garden) && !visited[i][j]) return false;
        }
    }

    return true;
}

vector<int> solveProblem(vector<Case> &cases){
    vector<int> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}

int solveCase(Case &thisCase){
    vector<vector<int>> garden = getGarden(thisCase);
    vector<vector<int>> locations = getLocations(garden);
    deque<deque<int>> solutions = generateSolutions(0, locations, garden, thisCase);

    if(!solutions.size()){
        deque<int> solutions;
        if(testSolution(solutions, locations, garden, thisCase)) return 1;
        return 0;
    }

    int c = 0;
    for(int i = 0; i < solutions.size(); i++){
        if(testSolution(solutions[i], locations, garden, thisCase)) c++;
    }
    return c;
}


// readers
vector<Case> readInput(){
    // read # of cases
    char dummy;
    int t;
    scanf("%d", &t);

    // read each case
    vector<Case> cases(t);
    for(int i = 0; i < t; i++){
        Case thisCase;
        scanf("%d", &thisCase.h);
        scanf("%d", &thisCase.w);
        scanf("%c", &dummy); // skip \n

        thisCase.g = new int*[4];

        for(int j = 0; j < 4; j++){
            thisCase.g[j] = new int[4];
            for(int k = 0; k < 4; k++) {
                char c = '-';
                scanf("%c", &c);
                if (c == '*') { // source here value * -1
                    thisCase.g[j][k] = thisCase.w;
                    thisCase.srow = j;
                    thisCase.scol = k;
                } else if (c == '_') { // nothing here i.e. 7
                    thisCase.g[j][k] = 7;
                } else if (c == '?') { // decision point here i.e. 8
                    thisCase.g[j][k] = 8;
                }else{ // a flower here i.e. whatever the value is
                    thisCase.g[j][k] = c - '0';
                }
            }
            scanf("%c", &dummy); // skip \n
        }

        cases[i] = thisCase;
    }

    return cases;
}

vector<Case> readInputFile(){
    FILE* file;
    file = fopen (inFileName.c_str(),"r");

    // read # of cases
    char dummy;
    int t;
    fscanf(file, "%d", &t);

    // read each case
    vector<Case> cases(t);
    for(int i = 0; i < t; i++){
        Case thisCase;
        fscanf(file, "%d", &thisCase.h);
        fscanf(file, "%d", &thisCase.w);
        fscanf(file, "%c", &dummy); // skip \n

        thisCase.g = new int*[4];

        for(int j = 0; j < 4; j++){
            thisCase.g[j] = new int[4];
            for(int k = 0; k < 4; k++) {
                char c = '-';
                fscanf(file, "%c", &c);
                if (c == '*') { // source here
                    thisCase.g[j][k] = thisCase.w;
                    thisCase.srow = j;
                    thisCase.scol = k;
                } else if (c == '_') { // nothing here i.e. 7
                    thisCase.g[j][k] = 7;
                } else if (c == '?') { // decision point here
                    thisCase.g[j][k] = 8;
                }else{ // a flower here
                    thisCase.g[j][k] = c - '0';
                }
            }
            fscanf(file, "%c", &dummy); // skip \n
        }

        cases[i] = thisCase;
    }

    fclose (file);
    return cases;
}


// writers
void writeOutput(vector<int> &answers){
    for(int i = 0; i < answers.size(); i++){
        printf("Case #%d: %d\n", i+1, answers[i]);
    }
}

void writeOutputFile(vector<int> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < answers.size(); i++){
        fprintf(file, "Case #%d: %d\n", i+1, answers[i]);
    }

    fclose(file);
}