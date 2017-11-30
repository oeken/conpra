#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Case {
    int n; // number of planets
    int* x; // x's of planets
    int* y; // y's of planets
    int* z; // z's of planets
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