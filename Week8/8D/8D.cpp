#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Case {
    long h; // height of tower
    long n; // types of boxes
    vector<long> x; // x's of boxes
    vector<long> y; // y's of boxes
    vector<long> z; // z's of boxes
};

// readers
vector<Case> readInput();
vector<Case> readInputFile();

// writers
void writeOutput(vector<bool> &answers);
void writeOutputFile(vector<bool> &answers);

// helpers
vector<bool> solveProblem(vector<Case> &cases);
bool solveCase(Case &thisCase);

// file names
string inFileName = "in.txt";
string outFileName = "out.txt";


int main() {
//    vector<Case> cases = readInputFile();
    vector<Case> cases = readInput();

    vector<bool> answers = solveProblem(cases);

    writeOutput(answers);
//    writeOutputFile(answers);
    return 0;
}


// helpers
vector<long> solveMSH(vector<long> &sortedBoxes, vector<long> &x, vector<long> &y, vector<long> &z){
    // init table
    long N = sortedBoxes.size();
    vector<long> MSH(N);

    // base case
    for (long i = 0; i < N; ++i) {
        MSH[i] = z[sortedBoxes[i]];
    }

    // tabulate (bottom --> up)
    for (long i = 1; i < N ; ++i) {
        long trg = sortedBoxes[i];
        for (long j = 0; j < i; ++j) {
            long src = sortedBoxes[j];
            if ( x[trg] < x[src] && y[trg] < y[src] && MSH[i] < MSH[j] + z[trg]) {
                MSH[i] = MSH[j] + z[trg];
            }
        }
    }
    return MSH;
}

void generateRotations(Case &thisCase, vector<long> &xrot, vector<long> &yrot, vector<long> &zrot){
    for (long i = 0; i < thisCase.n; ++i) {
        // combination 1 (original)
        xrot[3*i] = max(thisCase.x[i], thisCase.y[i]);
        yrot[3*i] = min(thisCase.x[i], thisCase.y[i]);
        zrot[3*i] = thisCase.z[i];
        // combination 2
        xrot[3*i+1] = max(thisCase.x[i], thisCase.z[i]);
        yrot[3*i+1] = min(thisCase.x[i], thisCase.z[i]);;
        zrot[3*i+1] = thisCase.y[i];
        // combination 3
        xrot[3*i+2] = max(thisCase.y[i], thisCase.z[i]);
        yrot[3*i+2] = min(thisCase.y[i], thisCase.z[i]);;
        zrot[3*i+2] = thisCase.x[i];
    }
}

long boxStacking(Case &thisCase){
    // generate all rotations
    vector<long> xrot(thisCase.n*3);
    vector<long> yrot(thisCase.n*3);
    vector<long> zrot(thisCase.n*3);
    generateRotations(thisCase, xrot, yrot, zrot);

    // sort boxes
    vector<long> sortedBoxes(thisCase.n*3);
    for (long i = 0; i < thisCase.n * 3; ++i) sortedBoxes[i] = i;
    sort(sortedBoxes.begin(), sortedBoxes.end(), [&](long a, long b){
        return xrot[a]*yrot[a] > xrot[b]*yrot[b];
    });

    // maximum stack height
    vector<long> msh = solveMSH(sortedBoxes, xrot, yrot, zrot);

    // find the maximum increasing subseq
    long rv = *max_element(msh.begin(), msh.end());
    return rv;

}

vector<bool> solveProblem(vector<Case> &cases){
    vector<bool> answers(cases.size());
    for(long i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}

bool solveCase(Case &thisCase){

    long height = boxStacking(thisCase);
//    cout << height << endl;
    bool answer = thisCase.h <= height;

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
        scanf("%li", &thisCase.h);
        scanf("%li", &thisCase.n);

        vector<long> x(thisCase.n);
        vector<long> y(thisCase.n);
        vector<long> z(thisCase.n);

        for(long j = 0; j < thisCase.n; j++){
            long xe,ye,ze;
            scanf("%li", &xe);
            scanf("%li", &ye);
            scanf("%li", &ze);
            x[j] = xe;
            y[j] = ye;
            z[j] = ze;
        }

        thisCase.x = x;
        thisCase.y = y;
        thisCase.z = z;

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
        fscanf(file, "%li", &thisCase.h);
        fscanf(file, "%li", &thisCase.n);

        vector<long> x(thisCase.n);
        vector<long> y(thisCase.n);
        vector<long> z(thisCase.n);

        for(long j = 0; j < thisCase.n; j++){
            long xe,ye,ze;
            fscanf(file, "%li", &xe);
            fscanf(file, "%li", &ye);
            fscanf(file, "%li", &ze);
            x[j] = xe;
            y[j] = ye;
            z[j] = ze;
        }

        thisCase.x = x;
        thisCase.y = y;
        thisCase.z = z;

        cases[i] = thisCase;
    }

    fclose (file);
    return cases;
}


// writers
void writeOutput(vector<bool> &answers){
    for(long i = 0; i < answers.size(); i++){
        if(answers[i]){
            printf("Case #%li: yes\n", i+1);
        }else{
            printf("Case #%li: no\n", i+1);
        }
    }
}

void writeOutputFile(vector<bool> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(long i = 0; i < answers.size(); i++){
        if(answers[i]){
            fprintf(file, "Case #%li: yes\n", i+1);
        }else{
            fprintf(file, "Case #%li: no\n", i+1);
        }
    }

    fclose(file);
}