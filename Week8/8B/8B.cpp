#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Case {
    long d; // number of days
    long c; // number of chpts/day
    long m; // change time length
    vector<vector<long>> a; // car times between chpts on every day
    vector<vector<long>> b; // bike times between chpts on every day
};


// readers
vector<Case> readInput();
vector<Case> readInputFile();

// writers
void writeOutput(vector<long> &answers);
void writeOutputFile(vector<long> &answers);

// helpers
vector<long> solveProblem(vector<Case> &cases);
long solveCase(Case &thisCase);

// file names
string inFileName = "in.txt";
string outFileName = "out.txt";


int main() {
//    vector<Case> cases = readInputFile();
    vector<Case> cases = readInput();

    vector<long> answers = solveProblem(cases);

    writeOutput(answers);
//    writeOutputFile(answers);
    return 0;
}


// helpers
long solveRallye(vector<long> &carTimes, vector<long> &bikeTimes, long chpts, long changeTime){
    // define tables
    vector<long> rallyeCumTimeCar(chpts+1);
    vector<long> rallyeCumTimeBike(chpts+1);

    // base case (start from 1) index 0 are undefined for yes change tables
    rallyeCumTimeCar[0] = carTimes[0];
    rallyeCumTimeBike[0] = bikeTimes[0];

    // tabulate
    for (long i = 1; i <= chpts; ++i) {
        rallyeCumTimeCar[i] = rallyeCumTimeCar[i-1] + carTimes[i];
        rallyeCumTimeBike[i] = rallyeCumTimeBike[i-1] + bikeTimes[i];
    }

    long soln = LONG_MAX;
    long left;
    long right;
    for (long i = 0; i < chpts; ++i) {
        // opt1 bike2car
        left = rallyeCumTimeBike[i];
        right = rallyeCumTimeCar[chpts] - rallyeCumTimeCar[i];
        soln = min(soln, left + right + changeTime);
        // opt2 car2bike
        left = rallyeCumTimeCar[i];
        right = rallyeCumTimeBike[chpts] - rallyeCumTimeBike[i];
        soln = min(soln, left + right + changeTime);
    }

    soln = min(soln, rallyeCumTimeCar[chpts]);
    soln = min(soln, rallyeCumTimeBike[chpts]);

    return soln;
}

vector<long> solveProblem(vector<Case> &cases){
    vector<long> answers(cases.size());
    for(long i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}

long solveCase(Case &thisCase){
    long rv = 0;
    for (long i = 0; i < thisCase.d; ++i) {
        long temp = solveRallye(thisCase.a[i], thisCase.b[i], thisCase.c, thisCase.m);
        rv += temp;
    }
    return rv;
}


// readers
vector<Case> readInput(){
    // read # of cases
    long t;
    scanf("%li", &t);

    // read each case
    vector<Case> cases(t);
    for(long i = 0; i < t; i++){
        Case thisCase;
        scanf("%li", &thisCase.d);
        scanf("%li", &thisCase.c);
        scanf("%li", &thisCase.m);

        // car times
        vector<vector<long>> a(thisCase.d);
        long temp = -1;
        for (long j = 0; j < thisCase.d; ++j) { // for each day
            for (long k = 0; k < thisCase.c + 1; ++k) { // for each chpts longerval
                scanf("%li", &temp);
                a[j].push_back(temp);
            }
        }
        thisCase.a = a;

        // bike times
        vector<vector<long>> b(thisCase.d);
        temp = -1;
        for (long j = 0; j < thisCase.d; ++j) { // for each day
            for (long k = 0; k < thisCase.c + 1; ++k) { // for each chpts longerval
                scanf("%li", &temp);
                b[j].push_back(temp);
            }
        }
        thisCase.b = b;

        cases[i] = thisCase;
    }

    return cases;
}

vector<Case> readInputFile(){
    FILE* file;
    file = fopen (inFileName.c_str(),"r");

    // read # of cases
    long t;
    fscanf(file, "%li", &t);

    // read each case
    vector<Case> cases(t);
    for(long i = 0; i < t; i++){
        Case thisCase;
        fscanf(file, "%li", &thisCase.d);
        fscanf(file, "%li", &thisCase.c);
        fscanf(file, "%li", &thisCase.m);

        // car times
        vector<vector<long>> a(thisCase.d);
        long temp = -1;
        for (long j = 0; j < thisCase.d; ++j) { // for each day
            for (long k = 0; k < thisCase.c + 1; ++k) { // for each chpts longerval
                fscanf(file, "%li", &temp);
                a[j].push_back(temp);
            }
        }
        thisCase.a = a;

        // bike times
        vector<vector<long>> b(thisCase.d);
        temp = -1;
        for (long j = 0; j < thisCase.d; ++j) { // for each day
            for (long k = 0; k < thisCase.c + 1; ++k) { // for each chpts longerval
                fscanf(file, "%li", &temp);
                b[j].push_back(temp);
            }
        }
        thisCase.b = b;

        cases[i] = thisCase;
    }

    fclose (file);
    return cases;
}


// writers
void writeOutput(vector<long> &answers){
    for(long i = 0; i < answers.size(); i++){
        printf("Case #%li: %li\n", i+1, answers[i]);
    }
}

void writeOutputFile(vector<long> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(long i = 0; i < answers.size(); i++){
        fprintf(file, "Case #%li: %li\n", i+1, answers[i]);
    }

    fclose(file);
}