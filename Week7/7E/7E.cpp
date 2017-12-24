#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <numeric>
#include <string.h>

using namespace std;

struct Case {
    int n; // number of human DNAs
    int m; // number of mouse DNAs
    char** DNAhum; // human DNAs
    char** DNAmou; // mouse DNAs
    int seqlen;
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
int pairId(char c1, char c2){
    if(c1 == c2){
        if(c1 == 'A') return 0; // AA
        if(c1 == 'C') return 1; // CC
        if(c1 == 'T') return 2; // TT
        if(c1 == 'G') return 3; // GG
    }else{
        if((c1 == 'A' && c2 == 'C') || (c1 == 'C' && c2 == 'A')) return 4; // AC
        if((c1 == 'A' && c2 == 'T') || (c1 == 'T' && c2 == 'A')) return 5; // AT
        if((c1 == 'A' && c2 == 'G') || (c1 == 'G' && c2 == 'A')) return 6; // AG
        if((c1 == 'C' && c2 == 'T') || (c1 == 'T' && c2 == 'C')) return 7; // CT
        if((c1 == 'C' && c2 == 'G') || (c1 == 'G' && c2 == 'C')) return 8; // CG
        if((c1 == 'T' && c2 == 'G') || (c1 == 'G' && c2 == 'T')) return 9; // TG
        return -1;
    }
}

vector<int> solveProblem(vector<Case> &cases){
    vector<int> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}

int solveCase(Case &thisCase){
    vector<vector<char>> human(thisCase.n);
    vector<vector<char>> mouse(thisCase.m);
    for (int i = 0; i < thisCase.n; ++i) human[i].assign(thisCase.DNAhum[i], thisCase.DNAhum[i] + thisCase.seqlen);
    for (int i = 0; i < thisCase.m; ++i) mouse[i].assign(thisCase.DNAmou[i], thisCase.DNAmou[i] + thisCase.seqlen);

    // count pairs
    vector<long> paircounts(10);
    for (int i = 0; i < thisCase.n; ++i) { // for each human dna
        for (int j = 0; j < thisCase.m; ++j) { // for each mouse dna
            for (int k = 0; k < thisCase.seqlen; ++k) { // for each entry
                char c1 = human[i][k];
                char c2 = mouse[j][k];
                paircounts[pairId(c1,c2)]++;
            }
        }
    }

    // sort pairs
    vector<int> pairs(10);
    for (int i = 0; i < 10; ++i) pairs[i] = i;
    sort(pairs.begin(), pairs.end(), [&](int a, int b){
        int mula = a > 3;
        int mulb = b > 3;
        double ma = (double)(mula+1);
        double mb = (double)(mulb+1);

        return (double)paircounts[a]/ma < (double)paircounts[b]/mb;
    });

    // assign the extreme
    vector<int> pairvals(10);
    int activeCount = 0;
    for (int i = 0; i < 10; ++i) {
        if(paircounts[i]){ pairvals[i] = 10; activeCount++; }
        else if(i > 3){ pairvals[i] = -10; }
    }

    // ignoring unset variables
    int c = 10 - activeCount;

    // compute initial cost
    int valsum = 0;
    for (int i = 0; i < 10; ++i) {
            int mul = i > 3;
            valsum += (mul+1) * pairvals[i];
    }

    bool feasible = valsum <= 0;
    while(c < 10 && !feasible){
        int pair = pairs[c];
        int dec;
        if(pair < 4){
            dec = min(valsum, 10);
            pairvals[pair] -= dec;
        }else {
            dec = min(valsum, 40);
            pairvals[pair] -= dec/2;
        }
        valsum -= dec;
        feasible = valsum <= 0;
        c++;
    }

    int ans = 0;
    for (int i = 0; i < 10; ++i) {
        if(paircounts[i]>0){
            ans += paircounts[i] * pairvals[i];
        }
    }

    return ans;
}


// readers
vector<Case> readInput(){
    // read # of cases
    int t;
    scanf("%d", &t);

    // read each case
    vector<Case> cases(t);
    for(int i = 0; i < t; i++){
        Case thisCase;
        scanf("%d", &thisCase.n);
        scanf("%d", &thisCase.m);

        thisCase.DNAhum = new char*[thisCase.n];
        thisCase.DNAmou = new char*[thisCase.m];

        for(int j = 0; j < thisCase.n; j++){
            char* str = new char[100];
            scanf("%s", str);
            thisCase.DNAhum[j] = str;
            thisCase.seqlen = strlen(str);
        }

        for(int j = 0; j < thisCase.m; j++){
            char* str = new char[100];
            scanf("%s", str);
            thisCase.DNAmou[j] = str;
        }
        cases[i] = thisCase;
    }

    return cases;
}

vector<Case> readInputFile(){
    FILE* file;
    file = fopen (inFileName.c_str(),"r");

    // read # of cases
    int t;
    fscanf(file, "%d", &t);

    // read each case
    vector<Case> cases(t);
    for(int i = 0; i < t; i++){
        Case thisCase;
        fscanf(file, "%d", &thisCase.n);
        fscanf(file, "%d", &thisCase.m);

        thisCase.DNAhum = new char*[thisCase.n];
        thisCase.DNAmou = new char*[thisCase.m];

        for(int j = 0; j < thisCase.n; j++){
            char* str = new char[100];
            fscanf(file, "%s", str);
            thisCase.DNAhum[j] = str;
            thisCase.seqlen = strlen(str);
        }

        for(int j = 0; j < thisCase.m; j++){
            char* str = new char[100];
            fscanf(file, "%s", str);
            thisCase.DNAmou[j] = str;
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