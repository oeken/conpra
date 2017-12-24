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
    vector<Case> cases = readInput();

    vector<int> answers = solveProblem(cases);

    writeOutput(answers);
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



bool isSatisfiedHelper(int i, int j, vector<vector<int>> &garden, vector<vector<bool>> &inquery){

    inquery[i][j] = true;

    // check up
    if(isValid(i-1,j) && !inquery[i-1][j]){
        if(isSource(i-1,j,garden) && garden[i][j] <= getFlowVal(i-1,j,garden)) return true;
        if(isFlower(i-1,j,garden) && garden[i][j] <= garden[i-1][j] && isSatisfiedHelper(i-1,j,garden,inquery)) return true;
    }

    // check down
    if(isValid(i+1,j) && !inquery[i+1][j]){
        if(isSource(i+1,j,garden) && garden[i][j] <= getFlowVal(i+1,j,garden)) return true;
        if(isFlower(i+1,j,garden) && garden[i][j] <= garden[i+1][j] && isSatisfiedHelper(i+1,j,garden,inquery)) return true;
    }

    // check left
    if(isValid(i,j-1) && !inquery[i][j-1]){
        if(isSource(i,j-1,garden) && garden[i][j] <= getFlowVal(i,j-1,garden)) return true;
        if(isFlower(i,j-1,garden) && garden[i][j] <= garden[i][j-1] && isSatisfiedHelper(i,j-1,garden,inquery)) return true;
    }

    // check right
    if(isValid(i,j+1) && !inquery[i][j+1]){
        if(isSource(i,j+1,garden) && garden[i][j] <= getFlowVal(i,j+1,garden)) return true;
        if(isFlower(i,j+1,garden) && garden[i][j] <= garden[i][j+1] && isSatisfiedHelper(i,j+1,garden,inquery)) return true;
    }

    return false;
}

bool isSatisfied(int i, int j, vector<vector<int>> &garden){
    vector<vector<bool>> inquery(4);
    for(int i = 0; i < 4; i++){
        vector<bool> v(4);
        inquery[i] = v;
    }
    return isSatisfiedHelper(i, j, garden, inquery);
}

int areFlowersFed(vector<vector<int>> &garden){

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(isFlower(i,j,garden)){
                if(!isSatisfied(i,j,garden)) return 0;
            }
        }
    }
    return 1;
}

void evaluateRowCol(int *maxval, int *minval, bool *ineigh, int i, int j, vector<vector<int>> &garden){
    if(isValid(i,j)){
        if(isSource(i,j,garden)){ // also implies that this source is satisfied
            int flow = getFlowVal(i,j,garden);
            *maxval = max(*maxval, flow);
            *ineigh = true;
        }

        if(isFlower(i,j,garden) && isSatisfied(i,j,garden)) {
            int floheight = garden[i][j];
            *maxval = max(*maxval, floheight);
            *ineigh = true;
        }

        if(isFlower(i,j,garden) && !isSatisfied(i,j,garden)){
            int floheight = garden[i][j];
            *minval = max(*minval, floheight);
        }
    }
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

vector<vector<int>> getImmediateNeighbours(vector<vector<int>> &garden, Case &thisCase){
    vector<vector<int>> rv;
    for(int i = 0; i<4; i++){
        for(int j = 0; j<4; j++){
            if(garden[i][j] == 8){
                int maxval = 0;
                int minval = 0;
                bool ineigh = false;

                // up direction
                evaluateRowCol(&maxval, &minval, &ineigh, i-1, j, garden);

                // down direction
                evaluateRowCol(&maxval, &minval, &ineigh, i+1, j, garden);

                // left direction
                evaluateRowCol(&maxval, &minval, &ineigh, i, j-1, garden);

                // right direction
                evaluateRowCol(&maxval, &minval, &ineigh, i, j+1, garden);

                bool fed = areFlowersFed(garden);
                if(fed){
                    maxval = thisCase.h;
                    minval = 0;
                }

                if(ineigh){
                    vector<int> v({i, j, minval, maxval});
                    rv.push_back(v);
                }
            }
        }
    }
    return rv;
}

deque<deque<int>> getAllAssigments(int index, vector<vector<int>> &neighbours){
    // base case 0
    if(index == neighbours.size()){
        deque<deque<int>> d;
        return d;
    }

    // edge case
//    int minval = neighbours[index][2];
//    int maxval = neighbours[index][3];

    int minval = 0;
    int maxval = 6;

    if(maxval-minval+1 < 0) {
        deque<deque<int>> d;
        return d;
    }

    // base case 1
    if(index == neighbours.size()-1){
        deque<deque<int>> rv(maxval-minval+1);
        int c = 0;
        for(int i = minval; i <= maxval; i++){
            deque<int> d;
            d.push_front(i);
            rv[c] = d;
            c++;
        }
        return rv;
    }


    // loop case
    deque<deque<int>> therest = getAllAssigments(index+1, neighbours);
    deque<deque<int>> rv(therest.size() * (maxval-minval+1));
    int c = 0;
    for(int i = minval; i <= maxval; i++){
        for(int j = 0; j < therest.size(); j++) {
            deque<int> d(therest[j]);
            d.push_front(i);
            rv[c] = d;
            c++;
        }
    }

    return rv;
}

vector<vector<int>> exertAssignment(vector<vector<int>> &neighbours, deque<int> &assignment, vector<vector<int>> &garden){
    vector<vector<int>> newgarden(garden);
    for(int i = 0; i < assignment.size(); i++){
        int row = neighbours[i][0];
        int col = neighbours[i][1];
        int val = -1 * (assignment[i] + 1);
        newgarden[row][col] = val;
    }
    return newgarden;
}



int countGarden(int row, int col, vector<vector<int>> garden, Case &thisCase){
    if(areFlowersFed(garden))
    int count = 0;

    // get immediate neighbours
    vector<vector<int>> neighbours = getImmediateNeighbours(garden, thisCase);
    deque<deque<int>> assignments = getAllAssigments(0, neighbours);

    // for each assignment, perform it on garden and call cout
    for(int i = 0; i < assignments.size(); i++){
        vector<vector<int>> newgarden = exertAssignment(neighbours, assignments[i], garden);
        int lowcount = countGarden(row, col, newgarden, thisCase);
        count += lowcount;
    }


    if(!count) return areFlowersFed(garden);
    return count;
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
    int answer = countGarden(thisCase.srow, thisCase.srow, garden, thisCase);
    return answer;
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
                    thisCase.g[j][k] = -1 * (thisCase.w+1);
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
                    thisCase.g[j][k] = -1 * (thisCase.w+1);
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