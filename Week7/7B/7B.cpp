#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Case {
    int n; // number of activities
    int m; // number of free hours
    int* fun; // funsy values
    int* len; // lengths
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
vector<int> solveProblem(vector<Case> &cases){
    vector<int> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}

int solveCase(Case &thisCase){
    int minutes = thisCase.m * 60;
    vector<int> activities(thisCase.n);
    for (int i = 0; i < thisCase.n; ++i) activities[i] = i;
    vector<int> funsies(thisCase.n);
    vector<int> lengths(thisCase.n);
    funsies.assign(thisCase.fun, thisCase.fun + thisCase.n);
    lengths.assign(thisCase.len, thisCase.len + thisCase.n);

    sort(activities.begin(), activities.end(), [&](int a, int b){
        return funsies[a] > funsies[b];
    });

    int totalfun = 0;
    int c = 0;
    while(minutes && c < thisCase.n){
        int activity = activities[c];
        int time = min(minutes, lengths[activity]);
        totalfun += funsies[activity] * time;
        minutes -= time;
        c++;
    }

    return totalfun;
}


// readers
vector<Case> readInput(){
    // read # of cases
    char* dummy = new char[30];
    int t;
    scanf("%d", &t);

    // read each case
    vector<Case> cases(t);
    for(int i = 0; i < t; i++){
        Case thisCase;
        scanf("%d", &thisCase.n);
        scanf("%d", &thisCase.m);

        thisCase.fun = new int[thisCase.n];
        thisCase.len = new int[thisCase.n];

        for(int j = 0; j < thisCase.n; j++){
            scanf("%s", dummy); // skip the name
            scanf("%d", &thisCase.fun[j]);
            scanf("%d", &thisCase.len[j]);
        }
        cases[i] = thisCase;
    }

    return cases;
}

vector<Case> readInputFile(){
    FILE* file;
    file = fopen (inFileName.c_str(),"r");

    // read # of cases
    char* dummy = new char[30];
    int t;
    fscanf(file, "%d", &t);

    // read each case
    vector<Case> cases(t);
    for(int i = 0; i < t; i++){
        Case thisCase;
        fscanf(file, "%d", &thisCase.n);
        fscanf(file, "%d", &thisCase.m);

        thisCase.fun = new int[thisCase.n];
        thisCase.len = new int[thisCase.n];

        for(int j = 0; j < thisCase.n; j++){
            fscanf(file, "%s", dummy); // skip the name
            fscanf(file, "%d", &thisCase.fun[j]);
            fscanf(file, "%d", &thisCase.len[j]);
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