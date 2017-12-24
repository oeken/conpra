#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <tuple>

using namespace std;

struct Case {
    int n; // number of coins
    int c; // money to spend
    int* v; // values of coins
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
//    vector<Case> cases = readInputFile();
    vector<Case> cases = readInput();

    vector<vector<int>> answers = solveProblem(cases);

    writeOutput(answers);
//    writeOutputFile(answers);
    return 0;
}


// helpers
tuple<vector<int>, vector<vector<int>>> solveCoinChange(vector<int> &values, int target){
    int N = values.size();

    // init tables
    vector<int> cointCount(target+1);
    for (int i = 0; i <= target; ++i) cointCount[i] = -1; // i.e. unset

    vector<vector<int>> coinSummary(target+1);
    for (int i = 0; i <= target; ++i){
        for (int j = 0; j < N; ++j) {
            coinSummary[i].push_back(-1); // i.e. unset
        }
    }

    // base case
    cointCount[0] = 0;
    vector<int> v(N);
    coinSummary[0] = v;

    // tabulate
    for (int i = 0; i <= target; ++i) { // for each cost up to target
        for (int j = 0; j < N; ++j) { // for each type that could be added
            int coinValue = values[j];
            int destValue = i + coinValue;
            if(destValue > target) continue; // ignore, never will be used

            if(cointCount[destValue] != -1){ // update existing entry
                if(cointCount[i] + 1 < cointCount[destValue]){ // only if better found
                    cointCount[destValue] = cointCount[i] + 1;
                    vector<int> v(coinSummary[i]);
                    v[j]++;
                    coinSummary[destValue] = v;
                }
            }else{ // first entry
                cointCount[destValue] = cointCount[i] + 1;
                vector<int> v(coinSummary[i]);
                v[j]++;
                coinSummary[destValue] = v;
            }
        }
    }

    tuple<vector<int>, vector<vector<int>>> tup(cointCount, coinSummary);
    return tup;
}

vector<vector<int>> solveProblem(vector<Case> &cases){
    vector<vector<int>> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}

vector<int> solveCase(Case &thisCase){
    vector<int> values(thisCase.n);
    values.assign(thisCase.v, thisCase.v + thisCase.n);

    vector<int> coinCount;
    vector<vector<int>> coinSummary;

    tuple<vector<int>, vector<vector<int>>> tup = solveCoinChange(values, thisCase.c);

    tie(coinCount, coinSummary) = tup;

    vector<int> rv = coinSummary[thisCase.c];
    return rv;
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
        scanf("%d", &thisCase.c);

        thisCase.v = new int[thisCase.n];

        for(int j = 0; j < thisCase.n; j++){
            scanf("%d", &thisCase.v[j]);
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
        fscanf(file, "%d", &thisCase.c);

        thisCase.v = new int[thisCase.n];

        for(int j = 0; j < thisCase.n; j++){
            fscanf(file, "%d", &thisCase.v[j]);
        }

        cases[i] = thisCase;
    }

    fclose (file);
    return cases;
}


// writers
void writeOutput(vector<vector<int>> &answers){
    for(int i = 0; i < answers.size(); i++){
        printf("Case #%d:", i+1);
        for (int j = 0; j < answers[i].size(); ++j) {
            printf(" %d", answers[i][j]);
        }
        printf("\n");
    }
}

void writeOutputFile(vector<vector<int>> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < answers.size(); i++){
        fprintf(file, "Case #%d:", i+1);
        for (int j = 0; j < answers[i].size(); ++j) {
            fprintf(file, " %d", answers[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}