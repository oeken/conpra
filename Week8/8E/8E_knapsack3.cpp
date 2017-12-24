#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <numeric>
#include <unordered_map>

using namespace std;

struct Case {
    long n; // number of districts
    vector<long> p; // people count
    vector<long> c; // money needed
};

struct Ans{
    long money; // total minimum money
    long visits; // minimum # of districts visited
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
    vector<Case> cases = readInputFile();
//    vector<Case> cases = readInput();

    vector<Ans> answers = solveProblem(cases);

//    writeOutput(answers);
    writeOutputFile(answers);
    return 0;
}


// helpers


//unordered_map<string, long> visitCountTable;
//unordered_map<string, long> moneySpentTable;
//
//void visitTableInsert(long index, long barrier, long value){
//    string query("");
//    query += to_string(index) + "," + to_string(barrier);
//    visitCountTable.emplace(query, value);
//}
//
//void moneyTableInsert(long index, long barrier, long value){
//    string query("");
//    query += to_string(index) + "," + to_string(barrier);
//    moneySpentTable.emplace(query, value);
//}
//
//long visitTableAccess(long index, long barrier){
//    string query("");
//    query += to_string(index) + "," + to_string(barrier);
//    if(visitCountTable.find(query) != visitCountTable.end()) return visitCountTable[query];
//    return -2;
//}
//
//long moneyTableAccess(long index, long barrier){
//    string query("");
//    query += to_string(index) + "," + to_string(barrier);
//    if(moneySpentTable.find(query) != moneySpentTable.end()) return moneySpentTable[query];
//    return -2;
//}

Ans solveElections(long index, long barrier, Case &thisCase, vector<vector<long>> &visitTable, vector<vector<long>> &moneyTable){
    Ans rv;

    // base case 1
    if(barrier < 0){
        rv.visits = 0;
        rv.money = 0;
        return rv;
    }

    // base case 2
    if(index == -1){
        rv.visits = -1;
        rv.money = -1;
        return rv;
    }

    // check table
//    long visits = visitTableAccess(index, barrier);
//    if(visits != -2){
//        rv.visits = visits;
//        rv.money = moneyTableAccess(index, barrier);
//        return rv;
//    }
    if(visitTable[index][barrier] != -1){
        rv.visits = visitTable[index][barrier];
        rv.money = moneyTable[index][barrier];
        return rv;
    }

    // recursion
    // opt1: do not invest in this city
    Ans opt1 = solveElections(index-1, barrier, thisCase, visitTable, moneyTable);

    // opt2: invest in this city
    Ans opt2 = solveElections(index-1, barrier-thisCase.p[index], thisCase, visitTable, moneyTable);
    if(opt2.visits != -1){
        opt2.visits++;
        opt2.money += thisCase.c[index];
    }

    if(opt1.visits == -1 && opt2.visits == -1) rv = opt1;
    else if(opt1.visits == -1 && opt2.visits != -1) rv = opt2;
    else if(opt1.visits != -1 && opt2.visits == -1) rv = opt1;
    else{
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
//    visitTableInsert(index, barrier, rv.visits);
//    moneyTableInsert(index, barrier, rv.money);
    visitTable[index][barrier] = rv.visits;
    moneyTable[index][barrier] = rv.money;

    return rv;
}

vector<Ans> solveProblem(vector<Case> &cases){
    vector<Ans> answers(cases.size());
    for(long i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}

Ans solveCase(Case &thisCase){
    long barrier = accumulate(thisCase.p.begin(), thisCase.p.end(), 0) / 2;

    vector<vector<long>> visitTable(thisCase.n);
    vector<vector<long>> moneyTable(thisCase.n);
    for (long i = 0; i < thisCase.n; ++i) {
        for (long j = 0; j <= barrier; ++j) {
            visitTable[i].push_back(-1);
            moneyTable[i].push_back(-1);
        }
    }

//    visitCountTable = unordered_map<string, long>();
//    moneySpentTable = unordered_map<string, long>();
    Ans answer = solveElections(thisCase.n-1, barrier, thisCase, visitTable, moneyTable);
    return answer;
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

        scanf("%li", &thisCase.n);

        vector<long> p(thisCase.n);
        vector<long> c(thisCase.n);

        for(long j = 0; j < thisCase.n; j++){
            long pe;
            long ce;
            scanf("%li", &pe);
            scanf("%li", &ce);
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
    long t;
    fscanf(file, "%li", &t);

    // read each case
    vector<Case> cases(t);
    for(long i = 0; i < t; i++){
        Case thisCase;

        fscanf(file, "%li", &thisCase.n);

        vector<long> p(thisCase.n);
        vector<long> c(thisCase.n);

        for(long j = 0; j < thisCase.n; j++){
            long pe;
            long ce;
            fscanf(file, "%li", &pe);
            fscanf(file, "%li", &ce);
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
    for(long i = 0; i < answers.size(); i++){
        printf("Case #%li: %li %li\n", i+1, answers[i].money, answers[i].visits);
    }
}

void writeOutputFile(vector<Ans> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(long i = 0; i < answers.size(); i++){
        fprintf(file, "Case #%li: %li %li\n", i+1, answers[i].money, answers[i].visits);
    }

    fclose(file);
}