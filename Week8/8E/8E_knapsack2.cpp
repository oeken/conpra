#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <numeric>
#include <unordered_map>

using namespace std;

long long deneme = 0;

struct Case {
    long long n; // number of districts
    vector<long long> p; // people count
    vector<long long> c; // money needed
};

struct Ans{
    bool valid;
    long long money; // total minimum money
    long long visits; // minimum # of districts visited
};

// readers
vector<Case> readInput();
vector<Case> readInputFile();

// writers
void writeOutput(vector<Ans> &answers);
void writeOutputFile(vector<Ans> &answers);

// helpers
vector<Ans> solveProblem(vector<Case> &cases);
Ans solveCase(Case &thisCase);

// file names
string inFileName = "in.txt";
string outFileName = "out.txt";


int main() {
//    vector<Case> cases = readInputFile();
    vector<Case> cases = readInput();

    vector<Ans> answers = solveProblem(cases);

    writeOutput(answers);
//    writeOutputFile(answers);
    return 0;
}


// helpers
Ans solveElections(long long index, long long barrier, Case &thisCase, vector<long long> &cumsum, vector<vector<bool>> &validTable, vector<vector<long long>> &visitTable, vector<vector<long long>> &moneyTable){
    Ans rv;
    deneme++;

    // base case 0
    if(cumsum[index] < barrier){
        rv.valid = false;
        rv.visits = -1;
        rv.money = -1;
        return rv;
    }

    // base case 1
    if(barrier < 0){
        rv.valid = true;
        rv.visits = 0;
        rv.money = 0;
        return rv;
    }

    // base case 2
    if(index == -1){
        rv.valid = false;
        rv.visits = -1;
        rv.money = -1;
        return rv;
    }

    // check table
    if(validTable[index][barrier]){
        rv.valid = true;
        rv.visits = visitTable[index][barrier];
        rv.money = moneyTable[index][barrier];
        return rv;
    }

    // recursion
    // opt1: do not invest in this city
    Ans opt1 = solveElections(index-1, barrier, thisCase, cumsum, validTable, visitTable, moneyTable);

    // opt2: invest in this city
    Ans opt2 = solveElections(index-1, barrier-thisCase.p[index], thisCase, cumsum, validTable, visitTable, moneyTable);
    opt2.visits++;
    opt2.money += thisCase.c[index];


    if(!opt1.valid && !opt2.valid) rv = opt1;
    else if(!opt1.valid && opt2.valid) rv = opt2;
    else if(opt1.valid && !opt2.valid) rv = opt1;
    else{ // both valid
        if(opt1.money < opt2.money){
            rv = opt1;
        }else if(opt1.money == opt2.money){
            if(opt1.visits < opt2.visits){
                rv = opt1;
            }else{
                rv = opt2;
            }
        }else{
            rv = opt2;
        }
    }

    // save in the tables
    validTable[index][barrier] = rv.valid;
    visitTable[index][barrier] = rv.visits;
    moneyTable[index][barrier] = rv.money;

    return rv;
}

vector<Ans> solveProblem(vector<Case> &cases){
    vector<Ans> answers(cases.size());
    for(long long i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}

Ans solveCase(Case &thisCase){
    long long barrier = accumulate(thisCase.p.begin(), thisCase.p.end(), 0) / 2;

    vector<long long> cumsum(thisCase.n);
    cumsum[0] = thisCase.p[0];
    for (int i = 1; i < thisCase.n; ++i) {
        cumsum[i] = cumsum[i-1] + thisCase.p[i];
    }


    vector<vector<bool>> validTable(thisCase.n, vector<bool>(barrier+1));
    vector<vector<long long>> visitTable(thisCase.n, vector<long long>(barrier+1));
    vector<vector<long long>> moneyTable(thisCase.n, vector<long long>(barrier+1));

    Ans answer = solveElections(thisCase.n-1, barrier, thisCase, cumsum, validTable, visitTable, moneyTable);
    return answer;
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

        vector<long long> p(thisCase.n);
        vector<long long> c(thisCase.n);

        for(long long j = 0; j < thisCase.n; j++){
            long long pe;
            long long ce;
            scanf("%lli", &pe);
            scanf("%lli", &ce);
            p[j] = pe;
            c[j] = ce;
        }

        thisCase.p = p;
        thisCase.c = c;

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

        vector<long long> p(thisCase.n);
        vector<long long> c(thisCase.n);

        for(long long j = 0; j < thisCase.n; j++){
            long long pe;
            long long ce;
            fscanf(file, "%lli", &pe);
            fscanf(file, "%lli", &ce);
            p[j] = pe;
            c[j] = ce;
        }

        thisCase.p = p;
        thisCase.c = c;

        cases[i] = thisCase;
    }

    fclose (file);
    return cases;
}


// writers
void writeOutput(vector<Ans> &answers){
    for(long long i = 0; i < answers.size(); i++){
        printf("Case #%lli: %lli %lli\n", i+1, answers[i].money, answers[i].visits);
    }
}

void writeOutputFile(vector<Ans> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(long long i = 0; i < answers.size(); i++){
        fprintf(file, "Case #%lli: %lli %lli\n", i+1, answers[i].money, answers[i].visits);
    }

    fclose(file);
}