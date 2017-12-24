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
    long long totalpeople = accumulate(thisCase.p.begin(), thisCase.p.end(), 0);

    // init tables
    vector<bool> sumsToTable(totalpeople+1);
    vector<long long> visitTable(totalpeople+1);
    vector<long long> moneyTable(totalpeople+1);

    // sort cities acc to people
//    vector<long long> sortedCities(thisCase.n);
//    for (long long i = 0; i < thisCase.n; ++i) sortedCities[i] = 0;
//    sort(sortedCities.begin(), sortedCities.end(), [&](long long a, long long b){
//        return thisCase.p[a] < thisCase.p[b];
//    });

    // base case
    sumsToTable[0] = true;
    visitTable[0] = 0;
    moneyTable[0] = 0;
    long long R = 0; // rightmost true entry

    // tabulate
    for (long long i = 0; i < thisCase.n; ++i) { // for each city
        for (long long j = R; j >= 0; --j) {
            if(sumsToTable[j]){
                if(sumsToTable[j+thisCase.p[i]]){ // update
                    if(moneyTable[j] + thisCase.c[i] > moneyTable[j+thisCase.p[i]]){ // take the max
                        moneyTable[j+thisCase.p[i]] = moneyTable[j] + thisCase.c[i];
                        visitTable[j+thisCase.p[i]] = visitTable[j] + 1;
                    }
                    else if(moneyTable[j] + thisCase.c[i] == moneyTable[j+thisCase.p[i]]){
                        if(visitTable[j] + 1 > visitTable[j+thisCase.p[i]]){
                            cout << visitTable[j]  << endl,
                            cout << visitTable[j+thisCase.p[i]]  << endl;
                            cout << "wohoo" << endl;
                            moneyTable[j+thisCase.p[i]] = moneyTable[j] + thisCase.c[i];
                            visitTable[j+thisCase.p[i]] = visitTable[j] + 1;
                            cout << visitTable[j+thisCase.p[i]]  << endl;
                        }
                    }
                }else{ // first entry
                    sumsToTable[j+thisCase.p[i]] = true;
                    moneyTable[j+thisCase.p[i]] = moneyTable[j] + thisCase.c[i];
                    visitTable[j+thisCase.p[i]] = visitTable[j] + 1;
                }


            }
        }
        R = min(totalpeople/2, R + thisCase.p[i]);
    }



    Ans answer;
    answer.visits = 0;
    answer.money = 0;
    for (long long i = 0; i <= totalpeople/2; ++i) {
        if(sumsToTable[i]){
            if(visitTable[i] > answer.visits) answer.visits = visitTable[i];
            if(moneyTable[i] > answer.money) answer.money = moneyTable[i];
        }
    }

    answer.visits = thisCase.n - answer.visits;
    answer.money = totalmoney - answer.money;
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