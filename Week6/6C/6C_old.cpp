#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <deque>
#include <tuple>
#include <math.h>


using namespace std;

struct Case {
    int h; // max height
    int w; // source height
    char** g; // grid/garden
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
bool isValid(int i, int j){
    return i >= 0 && i <= 3 && j >= 0 && j <= 3;
}

int rowcol2index(int row, int col){
    return 4*col+row;
}

void connectToNeighbours(int row, int col, vector<vector<int>> &garden, vector<char> &types){
    int src = rowcol2index(row, col);
    // up
    if(isValid(row-1,col) && types[rowcol2index(row-1,col)] != '_'){
        int trg = rowcol2index(row-1, col);
        garden[src].push_back(trg);
    }
    // down
    if(isValid(row+1,col) && types[rowcol2index(row+1,col)] != '_'){
        int trg = rowcol2index(row+1, col);
        garden[src].push_back(trg);
    }
    // left
    if(isValid(row,col-1) && types[rowcol2index(row,col-1)] != '_'){
        int trg = rowcol2index(row, col-1);
        garden[src].push_back(trg);
    }
    // right
    if(isValid(row,col+1) && types[rowcol2index(row,col+1)] != '_'){
        int trg = rowcol2index(row, col+1);
        garden[src].push_back(trg);
    }
}

tuple<vector<vector<int>>, vector<char>, vector<int>> getGarden(Case &thisCase){
    vector<vector<int>> garden(16);
    vector<int> heights(16);
    vector<char> types(16);

    for(int j = 0; j < 4; j++){ // for each col
        for(int i = 0; i < 4; i++){ // for each row
            int index = rowcol2index(i,j);

            char type = thisCase.g[i][j];
            if(!(type == '_' || type == '*' || type == '?')) type = 'F';
            types[index] = type;

            int height = -1;
            if(type == '*') height = thisCase.w;
            if(type == 'F') height = thisCase.g[i][j] - '0';
            heights[index] = height;
        }
    }

    for(int j = 0; j < 4; j++){ // for each col
        for(int i = 0; i < 4; i++) { // for each row
            int index = rowcol2index(i, j);
            if(types[index] == '*' || types[index] == '?' || types[index] == 'F')
                connectToNeighbours(i,j,garden,types);
        }
    }

    tuple<vector<vector<int>>, vector<char>, vector<int>> tup(garden, types, heights);
    return tup;
}

vector<int> getNextBatch(int srcindex, vector<vector<int>> &garden, vector<char> &types, vector<int> &heights){
    vector<int> batch;
    vector<bool> visited(types.size());
    deque<int> q;

    q.push_back(srcindex);
    visited[srcindex] = true;
    while(!q.empty()){
        int node = q.front();
        q.pop_front();

        for(int i = 0; i < garden[node].size(); i++){
            int trg = garden[node][i];
            if(types[trg] == '?' && heights[trg] == -1 && find(batch.begin(), batch.end(), trg) == batch.end()) {batch.push_back(trg);}
            else if(!visited[trg]){
                q.push_back(trg);
                visited[trg] = true;
            }
        }
    }
    return batch;
}

deque<deque<int>> getBatchAssigments(int index, int length, int maxheight){
    // base case 0
    if(index == length){
        deque<deque<int>> d;
        return d;
    }


    int minval = 0;
    int maxval = maxheight;

    // base case 1
    if(index == length-1){
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
    deque<deque<int>> therest = getBatchAssigments(index+1, length, maxheight);
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

vector<int> exertAssignment(vector<int> heights, vector<int> &batch, deque<int> &assignment){
    for(int i = 0; i < batch.size(); i++){
        heights[batch[i]] = assignment[i];
    }
    return heights;
}

bool flowersFed(int srcindex, vector<vector<int>> &garden, vector<char> &types, vector<int> &heights){
    vector<bool> visited(types.size());
    deque<int> q;
    q.push_back(srcindex);
    visited[srcindex] = true;

    while(!q.empty()){
        int node = q.front();
        q.pop_front();

        for(int i = 0; i < garden[node].size(); i++){
            int trg = garden[node][i];
            if(!visited[trg] && heights[trg] <= heights[node]) {
                q.push_back(trg);
                visited[trg] = true;
            }
        }
    }

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            int index = rowcol2index(i,j);
            if(types[index] == 'F' && !visited[index]) return false;
        }
    }
    return true;
}

bool flowersFeedable(int srcindex, vector<vector<int>> &garden, vector<char> &types, vector<int> &heights){
    vector<bool> visited(types.size());
    deque<int> q;
    q.push_back(srcindex);
    visited[srcindex] = true;

    while(!q.empty()){
        int node = q.front();
        q.pop_front();

        for(int i = 0; i < garden[node].size(); i++){
            int trg = garden[node][i];
            if(!visited[trg]) {
                if(heights[trg] == -1){ // relax conditions for unset ?s
                    q.push_back(trg);
                    visited[trg] = true;
                }else if(heights[trg] <= heights[node]) // normal conditions for set ?s
                    q.push_back(trg);
                    visited[trg] = true;
            }
        }
    }

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            int index = rowcol2index(i,j);
            if(types[index] == 'F' && !visited[index]) return false;
        }
    }
    return true;
}

int countGarden(int srcindex, int maxheight, vector<vector<int>> &garden, vector<char> &types, vector<int> heights){
    // get immediate neighbours
    vector<int> batch = getNextBatch(srcindex, garden, types, heights);

    if(batch.empty()){ // base case
        int rv = flowersFed(srcindex, garden, types, heights);
        return rv;
    }else if(!flowersFeedable(srcindex, garden, types, heights)){
        return 0; // backtrack
    }else {
        int count = 0;
        deque<deque<int>> assignments = getBatchAssigments(0, batch.size(), maxheight);
        // for each assignment, perform it on garden and call countGarden()
        for(int i = 0; i < assignments.size(); i++) {
            vector<int> newheights = exertAssignment(heights, batch, assignments[i]);
            int subcount = countGarden(srcindex, maxheight, garden, types, newheights);
            count += subcount;
        }
        return count;
    }
}

vector<int> solveProblem(vector<Case> &cases){
    vector<int> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}

int solveCase(Case &thisCase){
    // get garden
    vector<vector<int>> garden;
    vector<char> types;
    vector<int> heights;

    tuple<vector<vector<int>>, vector<char>, vector<int>> tup = getGarden(thisCase);
    tie(garden, types, heights) = tup;

    // figure out srcindex
    int srcindex;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            int index = rowcol2index(i,j);
            if(types[index] == '*') srcindex = index;
        }
    }


    // kill islands (run bfs)
    vector<bool> visited(types.size());
    deque<int> q;

    q.push_back(srcindex);
    visited[srcindex] = true;
    while(!q.empty()){
        int node = q.front();
        q.pop_front();
        for(int i = 0; i < garden[node].size(); i++){
            int trg = garden[node][i];
            if(!visited[trg]){
                q.push_back(trg);
                visited[trg] = true;
            }
        }
    }

    int islands = 0;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            int index = rowcol2index(i,j);
            if(types[index] == '?' && !visited[index]) {
                islands++;
                types[index] = '_'; // kill it
                heights[index] = -1;
            }
        }
    }


    // run recursive algo
    int maxheight = thisCase.h;
    int count = countGarden(srcindex, maxheight, garden, types, heights);
    count = count * pow(maxheight+1, islands);

    return count;
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

        thisCase.g = new char*[4];

        for(int j = 0; j < 4; j++){
            thisCase.g[j] = new char[4];
            for(int k = 0; k < 4; k++) {
                scanf("%c", &thisCase.g[j][k]);
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

        thisCase.g = new char*[4];

        for(int j = 0; j < 4; j++){
            thisCase.g[j] = new char[4];
            for(int k = 0; k < 4; k++) {
                fscanf(file, "%c", &thisCase.g[j][k]);
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