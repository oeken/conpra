#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <unordered_map>


using namespace std;

struct Case {
    int n; // number of clocks
    int* d1; // digit 1
    int* d2; // digit 2
    int* d3; // digit 3
    int* d4; // digit 4
};


// readers
vector<Case> readInput();
vector<Case> readInputFile();

// writers
void writeOutput(vector<vector<int>> &answers);
void writeOutputFile(vector<vector<int>> &answers);

// helpers
vector<vector<int>> solveProblem(vector<Case> &cases);
vector<int> solveCase(Case &thisCase);

// file names
string inFileName = "in.txt";
string outFileName = "out.txt";


int main() {
    vector<Case> cases = readInputFile();
//    vector<Case> cases = readInput();

//    vector<vector<int>> answers = solveProblem(cases);





//    writeOutput(answers);
//    writeOutputFile(answers);

    return 0;
}


// helpers
vector<vector<int>> solveProblem(vector<Case> &cases){
    vector<vector<int>> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}

vector<int> solveCase(Case &thisCase){




}


// readers
vector<Case> readInput(){
    // read # of cases
    char dummy = '-'; // dummy shit
    int t;
    scanf("%d", &t);

    // read each case
    vector<Case> cases(t);
    for(int i = 0; i < t; i++){
        Case thisCase;
        scanf("%d", &thisCase.n);

        thisCase.d1 = new int[thisCase.n];
        thisCase.d2 = new int[thisCase.n];
        thisCase.d3 = new int[thisCase.n];
        thisCase.d4 = new int[thisCase.n];

        for(int j = 0; j < thisCase.n; j++){
            int left;
            int right;
            scanf("%d", &left);
            scanf("%c", &dummy); // remove ':'
            scanf("%d", &right);
            thisCase.d1[j] = left / 10;
            thisCase.d2[j] = left % 10;

            thisCase.d3[j] = right / 10;
            thisCase.d4[j] = right % 10;
        }
        cases[i] = thisCase;
    }

    return cases;
}

vector<Case> readInputFile(){
    FILE* file;
    file = fopen (inFileName.c_str(),"r");

    // read # of cases
    char dummy = '-'; // dummy shit
    int t;
    fscanf(file, "%d", &t);

    // read each case
    vector<Case> cases(t);
    for(int i = 0; i < t; i++){
        Case thisCase;
        fscanf(file, "%d", &thisCase.n);

        thisCase.d1 = new int[thisCase.n];
        thisCase.d2 = new int[thisCase.n];
        thisCase.d3 = new int[thisCase.n];
        thisCase.d4 = new int[thisCase.n];

        for(int j = 0; j < thisCase.n; j++){
            int left;
            int right;
            fscanf(file, "%d", &left);
            fscanf(file, "%c", &dummy); // remove ':'
            fscanf(file, "%d", &right);
            thisCase.d1[j] = left / 10;
            thisCase.d2[j] = left % 10;

            thisCase.d3[j] = right / 10;
            thisCase.d4[j] = right % 10;
        }
        cases[i] = thisCase;
    }

    fclose (file);
    return cases;
}


// writers
void writeOutput(vector<vector<int>> &answers){
    for(int i = 0; i < answers.size(); i++){
        printf("Case #%d:\n", i+1);
        if(answers[i].empty()){
            printf("none\n");
        }else{
            for(int j = 0; j < answers[i].size()/4; j++){ // for each clock
                printf("%d%d:%d%d\n", answers[i][j*4], answers[i][j*4+1], answers[i][j*4+2], answers[i][j*4+3]);
            }
        }
    }
}

void writeOutputFile(vector<vector<int>> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < answers.size(); i++){
        fprintf(file, "Case #%d:\n", i+1);
        if(answers[i].empty()){
            fprintf(file, "none\n");
        }else{
            for(int j = 0; j < answers[i].size()/4; j++){ // for each clock
                fprintf(file, "%d%d:%d%d\n", answers[i][j*4], answers[i][j*4+1], answers[i][j*4+2], answers[i][j*4+3]);
            }
        }
    }

    fclose(file);
}