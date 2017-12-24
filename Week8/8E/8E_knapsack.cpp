#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <numeric>
#include <unordered_map>

using namespace std;

struct Case {
    long long n; // number of districts
    vector<long long> p; // people count
    vector<long long> c; // money needed
};

struct Ans{
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
    vector<Case> cases = readInputFile();
//    vector<Case> cases = readInput();

    vector<Ans> answers = solveProblem(cases);

//    writeOutput(answers);
    writeOutputFile(answers);
    return 0;
}


// helpers


//unordered_map<string, long long> visitCountTable;
//unordered_map<string, long long> moneySpentTable;
//
//void visitTableInsert(long long index, long long barrier, long long value){
//    string query("");
//    query += to_string(index) + "," + to_string(barrier);
//    visitCountTable.emplace(query, value);
//}
//
//void moneyTableInsert(long long index, long long barrier, long long value){
//    string query("");
//    query += to_string(index) + "," + to_string(barrier);
//    moneySpentTable.emplace(query, value);
//}
//
//long long visitTableAccess(long long index, long long barrier){
//    string query("");
//    query += to_string(index) + "," + to_string(barrier);
//    if(visitCountTable.find(query) != visitCountTable.end()) return visitCountTable[query];
//    return -2;
//}
//
//long long moneyTableAccess(long long index, long long barrier){
//    string query("");
//    query += to_string(index) + "," + to_string(barrier);
//    if(moneySpentTable.find(query) != moneySpentTable.end()) return moneySpentTable[query];
//    return -2;
//}

//Ans solveElections(long long index, long long barrier, Case &thisCase, vector<vector<long long>> &visitTable, vector<vector<long long>> &moneyTable){
//    Ans rv;
//
//    // base case
//    if(index == -1 || barrier == 0){
//        rv.visits = 0;
//        rv.money = 0;
//        return rv;
//    }
//
//    // check table
//    if(visitTable[index][barrier] != -1){
//        rv.visits = visitTable[index][barrier];
//        rv.money = moneyTable[index][barrier];
//        return rv;
//    }
//
//    // recursion
//    // opt1: do not consider this city
//    Ans opt1 = solveElections(index-1, barrier, thisCase, visitTable, moneyTable);
//
//    // opt2: consider this city
//    Ans opt2;
//    opt2.visits = 0;
//    opt2.money = 0;
//    if(barrier-thisCase.p[index] >= 0){
//        opt2 = solveElections(index-1, barrier-thisCase.p[index], thisCase, visitTable, moneyTable);
//        opt2.visits++;
//        opt2.money += thisCase.c[index];
//    }
//
//    if(opt1.money > opt2.money){
//        rv = opt1;
//    }else if(opt1.money == opt2.money){
//        if(opt1.visits > opt2.visits){
//            rv = opt1;
//        }else{
//            rv = opt2;
//        }
//    }else{
//        rv = opt2;
//    }
//
//    // save in the tables
//    visitTable[index][barrier] = rv.visits;
//    moneyTable[index][barrier] = rv.money;
//
//    return rv;
//}

vector<Ans> solveProblem(vector<Case> &cases){
    vector<Ans> answers(cases.size());
    for(long long i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}

Ans solveCase(Case &thisCase){
    long long totalmoney = accumulate(thisCase.c.begin(), thisCase.c.end(), 0);
    long long barrier = accumulate(thisCase.p.begin(), thisCase.p.end(), 0) / 2;

    // init tables
    vector<vector<long long>> visitTable(thisCase.n);
    vector<vector<long long>> moneyTable(thisCase.n);
    for (long long i = 0; i < thisCase.n; ++i) {
        visitTable[i].resize(barrier+1);
        moneyTable[i].resize(barrier+1);
//        for (long long j = 0; j <= barrier; ++j) {
//            vector<long long> v1(barrier+1);
//            vector<long long> v2(barrier+1);
//            visitTable[i] = v1;
//            moneyTable[i] = v2;

//        }
    }

    // base case
    for (long long i = 0; i <= barrier; ++i) { // when we have only the first city
        if(thisCase.p[0] <= i) {
            visitTable[0][i] = 1;
            moneyTable[0][i] = thisCase.c[0];
        }
    }


    // tabulate
    long long deneme = 0;
    for (long long i = 1; i <= thisCase.n - 1; ++i) { // for each city
        for (long long j = 1; j <= barrier; ++j) { // for each capacity
            deneme++;

            // opt1: do not take this city
            long long opt1money = moneyTable[i-1][j];
            long long opt1visit = visitTable[i-1][j];

            // opt2: take this city
            long long opt2money = 0;
            long long opt2visit = 0;
            if(thisCase.p[i] <= j){
                opt2money = moneyTable[i-1][j-thisCase.p[i]] + thisCase.c[i];
                opt2visit = visitTable[i-1][j-thisCase.p[i]] + 1;
            }

            if(opt1money < opt2money){
                moneyTable[i][j] = opt2money;
                visitTable[i][j] = opt2visit;
            }else if(opt1money == opt2money){
                if(opt1visit < opt2visit){
                    moneyTable[i][j] = opt2money;
                    visitTable[i][j] = opt2visit;
                }else{
                    moneyTable[i][j] = opt1money;
                    visitTable[i][j] = opt1visit;
                }
            }else{
                moneyTable[i][j] = opt1money;
                visitTable[i][j] = opt1visit;
            }
        }
    }

    cout << deneme;
    Ans answer;
    answer.visits = thisCase.n - visitTable[thisCase.n-1][barrier];
    answer.money = totalmoney - moneyTable[thisCase.n-1][barrier];
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