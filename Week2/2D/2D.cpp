#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <climits>

using namespace std;

struct Case {
    long n;
    long p;
    long q;
    long r;
    long s;
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
long* generateCookies(Case &thisCase);

long sumCookies(long* cookies, long count);
void printCookies(long* cookies, long count);


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
vector<long> solveProblem(vector<Case> &cases){
    vector<long> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}


long solveCase(Case &thisCase){
    long* cookies = generateCookies(thisCase);
//    printCookies(cookies, thisCase.n);
    long totalCookies = sumCookies(cookies, thisCase.n);
    double threshold = double(totalCookies) / 3;

    if(thisCase.n == 1) return 0;
    if(thisCase.n == 2) return min(cookies[0], cookies[1]);

    long a_indices[] = {-1, -1, -1}; // 0 low, 1 eq, 2 high
    long a_values[] = {0, 0, 0}; // 0 low, 1 eq, 2 high

    long b_indices[] = {-1, -1, -1}; // 0 low, 1 eq, 2 high
    long b_values[] = {0, 0, 0}; // 0 low, 1 eq, 2 high

    long a = 0;
    long tilla = 0;

    long b = thisCase.n-1;
    long afterb = 0;

    while(true){
        tilla += cookies[a];
        a++;
        if(tilla < threshold) {a_values[0] = tilla; a_indices[0] = a;}
        else if(tilla == threshold) {a_values[1] = tilla; a_indices[1] = a;}
        else {a_values[2] = tilla; a_indices[2] = a; break;}
    }

    while(true){
        afterb += cookies[b];
        b--;
        if(afterb < threshold) {b_values[0] = afterb; b_indices[0] = b;}
        else if(afterb == threshold) {b_values[1] = afterb; b_indices[1] = b;}
        else {b_values[2] = afterb; b_indices[2] = b; break;}
    }


    long lea = LONG_MIN;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(a_indices[i] != -1 && b_indices[j] != -1){
                if(a_indices[i] <= b_indices[j]){
                    long middle = totalCookies - a_values[i] - b_values[j];
                    long tom = max(max(a_values[i], b_values[j]), middle);
                    long remaining = totalCookies - tom;
                    if(lea < remaining) lea = remaining;
                }
            }
        }
    }

//    for(int i = 0; i < 2; i++){
//        for(int j = 0; j < 2; j++){
//            long left = tilla;
//            long right = afterb;
//            long lefti = a;
//            long righti = b;
//
//            if(i && a != 0) {lefti--; left -= cookies[lefti]; }
//            if(j && b != thisCase.n-1) {righti++; right -= cookies[righti]; }
//
//            long mid = totalCookies - left - right;
//
//            long tom = max(max(left, right), mid);
//            long remaining = totalCookies - tom;
//            if(lea < remaining) lea = remaining;
//        }
//    }

    return lea;
}

long* generateCookies(Case &thisCase){
    long* cookies = new long[thisCase.n];
    for(int i = 0; i < thisCase.n; i++){
        cookies[i] = ((i+1) * thisCase.p + thisCase.q) % thisCase.r + thisCase.s;
    }
    return cookies;
}

long sumCookies(long* cookies, long count){
    long sum = 0;
    for(int i = 0; i < count; i++){
        sum += cookies[i];
    }
    return sum;
}

void printCookies(long* cookies, long count){
    cout << "Cookies:" << endl;
    for(int i = 0; i < count; i++){
        cout << cookies[i] << ", ";
    }
    cout << endl;
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

        scanf("%li", &thisCase.n);
        scanf("%li", &thisCase.p);
        scanf("%li", &thisCase.q);
        scanf("%li", &thisCase.r);
        scanf("%li", &thisCase.s);

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

        fscanf(file, "%li", &thisCase.n);
        fscanf(file, "%li", &thisCase.p);
        fscanf(file, "%li", &thisCase.q);
        fscanf(file, "%li", &thisCase.r);
        fscanf(file, "%li", &thisCase.s);

        cases[i] = thisCase;
    }

    fclose (file);
    return cases;
}



// writers
void writeOutput(vector<long> &answers){
    for(int i = 0; i < answers.size(); i++){
        printf("Case #%d: %li\n", i+1, answers[i]);
    }
}

void writeOutputFile(vector<long> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < answers.size(); i++){
            fprintf(file, "Case #%d: %li\n", i+1, answers[i]);
    }

    fclose(file);
}