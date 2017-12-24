#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <queue>

using namespace std;

struct Case {
    long long n; // number of trees
    long long m; // number of disks
    long long* l; // lengths of trees
};


// readers
vector<Case> readInput();
vector<Case> readInputFile();

// writers
void writeOutput(vector<long long> &answers);
void writeOutputFile(vector<long long> &answers);

// helpers
vector<long long> solveProblem(vector<Case> &cases);
long long solveCase(Case &thisCase);

// file names
string inFileName = "in.txt";
string outFileName = "out.txt";


int main() {
//    vector<Case> cases = readInputFile();
    vector<Case> cases = readInput();

    vector<long long> answers = solveProblem(cases);

    writeOutput(answers);
//    writeOutputFile(answers);
    return 0;
}


// helpers
//long long getMachine(vector<long long> &loads){
//    long long minmach= -1;
//    long long minload = INT_MAX;
//    for (long long i = 0; i < loads.size(); ++i) {
//        if(loads[i] < minload){
//            minmach = i;
//            minload = loads[i];
//        }
//    }
//    return minmach;
//}

long long scheduleJobs(vector<long long> &lengths, long long m){
    long long N = lengths.size();

    vector<long long> jobs(N);
    for (long long i = 0; i < N; ++i) jobs[i] = i;
    sort(jobs.begin(), jobs.end(), [&](long long a, long long b){
        return lengths[a] > lengths[b];
    });

    priority_queue<long long, vector<long long>, greater<long long> > loads;
    for (long long i = 0; i < m; ++i) loads.push(0);

    for (long long i = 0; i < N; ++i) {
        long long jindex = jobs[i];
        long long newspan = lengths[jindex] + loads.top();
        loads.pop();
        loads.push(newspan);
    }

    long long makespan = 0;
    while(!loads.empty()){
        makespan = loads.top();
        loads.pop();
    }

//    for (long long i = 0; i < m; ++i) {
//        if(loads[i] > makespan) makespan = loads[i];
//    }
    return makespan;
}

vector<long long> solveProblem(vector<Case> &cases){
    vector<long long> answers(cases.size());
    for(long long i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}

long long solveCase(Case &thisCase){
    vector<long long> lengths(thisCase.n);
    lengths.assign(thisCase.l, thisCase.l + thisCase.n);

    long long makespan = scheduleJobs(lengths, thisCase.m);


    return makespan;
}


// readers
vector<Case> readInput(){
    // read # of cases
    long long t;
    scanf("%lli", &t);

    // read each case
    vector<Case> cases(t);
    for(long long i = 0; i < t; i++){
        Case thisCase;
        scanf("%lli", &thisCase.n);
        scanf("%lli", &thisCase.m);

        thisCase.l = new long long[thisCase.n];
        for(long long j = 0; j < thisCase.n; j++){
            scanf("%lli", &thisCase.l[j]);
        }
        cases[i] = thisCase;
    }

    return cases;
}

vector<Case> readInputFile(){
    FILE* file;
    file = fopen (inFileName.c_str(),"r");

    // read # of cases
    long long t;
    fscanf(file, "%lli", &t);

    // read each case
    vector<Case> cases(t);
    for(long long i = 0; i < t; i++){
        Case thisCase;
        fscanf(file, "%lli", &thisCase.n);
        fscanf(file, "%lli", &thisCase.m);

        thisCase.l = new long long[thisCase.n];
        for(long long j = 0; j < thisCase.n; j++){
            fscanf(file, "%lli", &thisCase.l[j]);
        }
        cases[i] = thisCase;
    }

    fclose (file);
    return cases;
}


// writers
void writeOutput(vector<long long> &answers){
    for(long long i = 0; i < answers.size(); i++){
        printf("Case #%lli: %lli\n", i+1, answers[i]);
    }
}

void writeOutputFile(vector<long long> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(long long i = 0; i < answers.size(); i++){
        fprintf(file, "Case #%lli: %lli\n", i+1, answers[i]);
    }

    fclose(file);
}