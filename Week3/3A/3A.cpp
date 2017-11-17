#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <queue>
#include <ctime>

using namespace std;

struct Case {
    int n; // number of planets
    int* x; // x's of planets
    int* y; // y's of planets
    int* z; // z's of planets
};

struct ND{
    int node;
    int distance;
};

// readers
vector<Case> readInput();
vector<Case> readInputFile();

// writers
void writeOutput(vector<int> &answers);
void writeOutputFile(vector<int> &answers);

// helpers
void print_edges(vector<vector<int>> &edges);
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
int distance(int i, int j, Case &thisCase){
    int dx = abs(thisCase.x[i] - thisCase.x[j]);
    int dy = abs(thisCase.y[i] - thisCase.y[j]);
    int dz = abs(thisCase.z[i] - thisCase.z[j]);
    return dx+dy+dz;
}

int primMST(vector<vector<int>> M){
    int n = M.size();
    int mst_weight = 0;

    vector<int> distance(n); // 0 distance means --> already in mst
    for(int i = 1; i < n; i++) distance[i] = M[0][i]; // node 0 is in mst, then init distances

    for(int i = 1; i < n; i++){
        // search for closest node and the distance to it
        int minimum = INT_MAX;
        int minimumNode = -1;
        for(int j = 0; j < n; j++){
            if(distance[j] > 0 && distance[j] < minimum){ minimum = distance[j]; minimumNode = j; }
        }

        // add it to mst
        distance[minimumNode] = 0;
        mst_weight += minimum;

        // update distances
        for(int j = 0; j < n; j++){
            if(distance[j] > M[minimumNode][j]) distance[j] = M[minimumNode][j];
        }
    }

    return mst_weight;
}

vector<vector<int>> constructGraph(Case &thisCase){
    vector<vector<int>> adjMat(thisCase.n);
    for(int i = 0; i < thisCase.n; i++){
        vector<int> v(thisCase.n);
        adjMat[i] = v;
        for(int j = 0; j < thisCase.n; j++){
            adjMat[i][j] = distance(i, j, thisCase);
        }
    }
    return adjMat;
}


vector<int> solveProblem(vector<Case> &cases){
    vector<int> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}


int solveCase(Case &thisCase){
    vector<vector<int>> M = constructGraph(thisCase);
    int answer = primMST(M);
    return answer;
}


// readers
vector<Case> readInput(){
    // read # of cases
    int t;
    scanf("%d", &t);

    // read each case
    vector<Case> cases(t);
    for(int i = 0; i < t; i++){
        Case thisCase;
        scanf("%d", &thisCase.n);

        thisCase.x = new int[thisCase.n];
        thisCase.y = new int[thisCase.n];
        thisCase.z = new int[thisCase.n];

        for(int j = 0; j < thisCase.n; j++){
            scanf("%d", &thisCase.x[j]);
            scanf("%d", &thisCase.y[j]);
            scanf("%d", &thisCase.z[j]);
        }

        cases[i] = thisCase;
    }

    return cases;
}

vector<Case> readInputFile(){
    FILE* file;
    file = fopen (inFileName.c_str(),"r");

    // read # of cases
    int t;
    fscanf(file, "%d", &t);

    // read each case
    vector<Case> cases(t);
    for(int i = 0; i < t; i++){
        Case thisCase;
        fscanf(file, "%d", &thisCase.n);

        thisCase.x = new int[thisCase.n];
        thisCase.y = new int[thisCase.n];
        thisCase.z = new int[thisCase.n];

        for(int j = 0; j < thisCase.n; j++){
            fscanf(file, "%d", &thisCase.x[j]);
            fscanf(file, "%d", &thisCase.y[j]);
            fscanf(file, "%d", &thisCase.z[j]);
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