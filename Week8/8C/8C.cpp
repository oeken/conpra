#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <unordered_map>
#include <string>



using namespace std;

struct Case {
    int n; // number of contests
    vector<int> a; // start times
    vector<int> b; // end times
    vector<int> p; // difficulties
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
unordered_map<int, int> table;

int findNextIndex(int index, vector<int> &sortedEvents, Case &thisCase){
    int startOfIndex = thisCase.a[sortedEvents[index]];
    for(int i = index-1; i > -1; --i){
        int endOfNewNextIndex = thisCase.b[sortedEvents[i]];
        if(endOfNewNextIndex < startOfIndex) return i;
    }
    return -1;
}

int solveGCPC(int index, vector<int> &sortedEvents, Case &thisCase){
    // base case
    if(index == -1) return 0;

    // table check
    if(table.find(index) != table.end()) return table[index];

    // recursion
    // opt1: item i is not included
    int opt1 = solveGCPC(index-1, sortedEvents, thisCase);

    // opt2: item i is included
    int nextIndex = findNextIndex(index, sortedEvents, thisCase);
    int opt2 = solveGCPC(nextIndex, sortedEvents, thisCase) + thisCase.p[sortedEvents[index]];

    // save and return answer
    int rv = max(opt1, opt2);
    table.emplace(index, rv);
    return rv;
}

vector<int> solveProblem(vector<Case> &cases){
    vector<int> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}

int solveCase(Case &thisCase){
    vector<int> sortedEvents(thisCase.n);
    for (int i = 0; i < thisCase.n; ++i) sortedEvents[i] = i;
    sort(sortedEvents.begin(), sortedEvents.end(), [&](int a, int b){
        return thisCase.b[a] < thisCase.b[b];
    });

    table = unordered_map<int, int>();
    int rv = solveGCPC(thisCase.n-1, sortedEvents, thisCase);
    return rv;
}


// readers
vector<Case> readInput(){
    // read # of cases
    char* dummy = new char[30];
    int t = 1;

    // read each case
    vector<Case> cases(t);
    for(int i = 0; i < t; i++){
        Case thisCase;
        scanf("%d", &thisCase.n);

        vector<int> a(thisCase.n);
        vector<int> b(thisCase.n);
        vector<int> p(thisCase.n);

        for(int j = 0; j < thisCase.n; j++){
            int ae, be, pe;
            scanf("%d", &ae);
            scanf("%d", &be);
            scanf("%d", &pe);
            scanf("%s", dummy); // skip the name

            a[j] = ae;
            b[j] = be;
            p[j] = pe;
        }

        thisCase.a = a;
        thisCase.b = b;
        thisCase.p = p;

        cases[i] = thisCase;
    }

    return cases;
}

vector<Case> readInputFile(){
    FILE* file;
    file = fopen (inFileName.c_str(),"r");

    // read # of cases
    char* dummy = new char[30];
    int t = 1;

    // read each case
    vector<Case> cases(t);
    for(int i = 0; i < t; i++){
        Case thisCase;
        fscanf(file, "%d", &thisCase.n);

        vector<int> a(thisCase.n);
        vector<int> b(thisCase.n);
        vector<int> p(thisCase.n);

        for(int j = 0; j < thisCase.n; j++){
            int ae, be, pe;
            fscanf(file, "%d", &ae);
            fscanf(file, "%d", &be);
            fscanf(file, "%d", &pe);
            fscanf(file, "%s", dummy); // skip the name

            a[j] = ae;
            b[j] = be;
            p[j] = pe;
        }

        thisCase.a = a;
        thisCase.b = b;
        thisCase.p = p;

        cases[i] = thisCase;
    }

    fclose (file);
    return cases;
}


// writers
void writeOutput(vector<int> &answers){
    printf("%d\n", answers[0]);
}

void writeOutputFile(vector<int> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    fprintf(file, "%d\n", answers[0]);

    fclose(file);
}