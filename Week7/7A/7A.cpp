#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Case {
    int n; // number of cities
    int** c; // streets
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
vector<vector<int>> constructGlist(Case &thisCase){
    vector<vector<int>> Glist(thisCase.n);

    for (int i = 0; i < thisCase.n; ++i) {
        int count = thisCase.c[i][0];
        for (int j = 1; j <= count; ++j) {
            int src = i;
            int trg = thisCase.c[i][j]-1;
            bool check1 = src != trg;
            bool check2 = find(Glist[src].begin(), Glist[src].end(), trg) == Glist[src].end();
            if(check1 && check2){
                Glist[src].push_back(trg);
                Glist[trg].push_back(src);
            }
        }
    }
    return Glist;
}

vector<vector<int>> constructGmatrix(Case &thisCase){
    vector<vector<int>> Gmatrix(thisCase.n);
    for (int i = 0; i < thisCase.n; ++i) {
        vector<int> v(thisCase.n);
        Gmatrix[i] = v;
    }

    for (int i = 0; i < thisCase.n; ++i) {
        int count = thisCase.c[i][0];
        for (int j = 1; j <= count; ++j) {
            int src = i;
            int trg = thisCase.c[i][j]-1;
            bool check = src != trg;
            if(check){
                Gmatrix[src][trg]++;
                Gmatrix[trg][src]++;
            }
        }
    }
    return Gmatrix;
}

vector<int> maxcutApprox(vector<vector<int>> &Glist, vector<vector<int>> &Gmatrix){
    int N = Glist.size();
    int counter = 0;
    bool sweep = true;
    vector<bool> A(N);
    while(sweep){ // repeat until cannot improve anymore
        sweep = false;

        for(int i = 0; i < N; i++){ // do a sweep over all variables
            int src = i;
            int Acount = 0;
            int Bcount = 0;
            for(int j = 0; j < Glist[src].size(); j++){
                int trg = Glist[src][j];
                if(A[trg]){
                    Acount += Gmatrix[src][trg];
                }else{
                    Bcount += Gmatrix[src][trg];
                }
            }
            if(A[src] && Acount > Bcount){ // move src from A to B
                A[src] = false;
                sweep = true;
                counter++;
            }
            if(!A[src] && Bcount > Acount){ // move src from B to A
                A[src] = true;
                sweep = true;
                counter++;
            }
        }
    }

    vector<int> rv;
    for (int i = 0; i < N; ++i) {
        if(A[i]) rv.push_back(i+1);
    }

    if(rv.empty()) rv.push_back(1);
    return rv;
}

vector<vector<int>> solveProblem(vector<Case> &cases){
    vector<vector<int>> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}

vector<int> solveCase(Case &thisCase){
    vector<vector<int>> Glist = constructGlist(thisCase);
    vector<vector<int>> Gmatrix = constructGmatrix(thisCase);

    vector<int> A = maxcutApprox(Glist, Gmatrix);
    return A;
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

        thisCase.c = new int*[thisCase.n];

        for(int j = 0; j < thisCase.n; j++){
            int count;
            scanf("%d", &count);
            thisCase.c[j] = new int[count+1];
            thisCase.c[j][0] = count;
            for(int k = 1; k <= count; k++){
                scanf("%d", &thisCase.c[j][k]);
            }
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

        thisCase.c = new int*[thisCase.n];

        for(int j = 0; j < thisCase.n; j++){
            int count;
            fscanf(file, "%d", &count);
            thisCase.c[j] = new int[count+1];
            thisCase.c[j][0] = count;
            for(int k = 1; k <= count; k++){
                fscanf(file, "%d", &thisCase.c[j][k]);
            }
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
        for(int j = 0; j < answers[i].size(); j++){
            printf("%d ", answers[i][j]);
        }
        printf("\n");
    }
}

void writeOutputFile(vector<vector<int>> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < answers.size(); i++){
        fprintf(file, "Case #%d:\n", i+1);
        for(int j = 0; j < answers[i].size(); j++){
            fprintf(file, "%d ", answers[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}