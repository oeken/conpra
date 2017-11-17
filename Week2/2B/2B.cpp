#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <climits>

using namespace std;

struct Case {
    long loan;      // l
    long rate;      // r
    long payment;   // p
    int years;      // y
};

// readers
vector<Case> readInput();
vector<Case> readInputFile();

// writers
void writeOutput(vector<long*> &answers);
void writeOutputFile(vector<long*> &answers);

// helpers
vector<long*> solveProblem(vector<Case> &cases);
long* solveCase(Case &thisCase);

long solveLoan(Case &thisCase);
long solveRate(Case &thisCase);
long solvePayment(Case &thisCase);

bool payable(long l, double r, long p, int m);

string inFileName = "in.txt";
string outFileName = "out.txt";


int main() {
//    vector<Case> cases = readInputFile();
    vector<Case> cases = readInput();

    vector<long*> answers = solveProblem(cases);

    writeOutput(answers);
//    writeOutputFile(answers);
    return 0;
}



// helpers
vector<long*> solveProblem(vector<Case> &cases){
    vector<long*> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}


long* solveCase(Case &thisCase){

    long max_loan = solveLoan(thisCase);
    long max_rate = solveRate(thisCase);
    long min_payment = solvePayment(thisCase);

    long* rv = new long[3];
    rv[0] = max_loan;
    rv[1] = max_rate;
    rv[2] = min_payment;
    return rv;
}

long solveLoan(Case &thisCase){
    double r = 1 + double(thisCase.rate) / 100;
    int m = thisCase.years * 12;

    long low = 0;
    long high = LONG_MAX;

    while(true){
        long mid = (high + low) / 2;
        if(mid == low) {
            return mid;
        }
        if(payable(mid, r, thisCase.payment, m)){
            low = mid;
        }else{
            high = mid;
        }
    }
}

long solveRate(Case &thisCase){
    int m = thisCase.years * 12;

    long low = 0;
    long high = LONG_MAX;

    if(!payable(thisCase.loan, 1+double(low)/100, thisCase.payment, m)){
        return -1; // impossible
    }

    if(payable(thisCase.loan, 1+double(high)/100, thisCase.payment, m)){
        return -2; // infinity;
    }

    while(true){
        long mid = (high + low) / 2;
        if(mid == low) return mid;
        if(payable(thisCase.loan, 1+double(mid)/100, thisCase.payment, m)){
            low = mid;
        }else{
            high = mid;
        }
    }
}

long solvePayment(Case &thisCase){
    double r = 1 + double(thisCase.rate) / 100;
    int m = thisCase.years * 12;

    long low = 0;
    long high = LONG_MAX;

    while(true){
        long mid = (high + low) / 2;
        if(mid == low) return mid+1;
        if(payable(thisCase.loan, r, mid, m)){
            high = mid;
        }else{
            low = mid;
        }
    }
}



bool payable(long l, double r, long p, int m){
    for(int i = 0; i < m; i++){
        l -= p;
        if(l <= 0) return true;
        l *= r;
    }
    return false;
}




// readers
vector<Case> readInput(){
    // read # of cases
    int caseCount;
    scanf("%d", &caseCount);

    // read each case
    vector<Case> cases(caseCount);
    for(int i = 0; i < caseCount; i++){
        Case thisCase;

        scanf("%li", &thisCase.loan);
        scanf("%li", &thisCase.rate);
        scanf("%li", &thisCase.payment);
        scanf("%d", &thisCase.years);

        cases[i] = thisCase;
    }

    return cases;
}

vector<Case> readInputFile(){
    FILE* file;
    file = fopen (inFileName.c_str(),"r");

    // read # of cases
    int caseCount;
    fscanf(file, "%d", &caseCount);

    // read each case
    vector<Case> cases(caseCount);
    for(int i = 0; i < caseCount; i++){
        Case thisCase;

        fscanf(file, "%d", &thisCase.loan);
        fscanf(file, "%d", &thisCase.rate);
        fscanf(file, "%d", &thisCase.payment);
        fscanf(file, "%d", &thisCase.years);

        cases[i] = thisCase;
    }

    fclose (file);
    return cases;
}



// writers
void writeOutput(vector<long*> &answers){
    for(int i = 0; i < answers.size(); i++){
        printf("Case #%d: %li ", i+1, answers[i][0]);

        if(answers[i][1] == -1){ printf("impossible "); }
        else if(answers[i][1] == -2){ printf("infinity "); }
        else{ printf("%li ", answers[i][1]); }

        printf("%li\n", answers[i][2]);
    }
}


void writeOutputFile(vector<long*> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < answers.size(); i++){
        fprintf(file, "Case #%d: %li ", i+1, answers[i][0]);

        if(answers[i][1] == -1){ fprintf(file, "impossible "); }
        else if(answers[i][1] == -2){ fprintf(file, "infinity "); }
        else{ fprintf(file, "%li ", answers[i][1]); }

        fprintf(file, "%li\n", answers[i][2]);
    }

    fclose(file);
}