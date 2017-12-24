#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Case {
    int l; // length of main street
    int n; // # of lights
    int d; // radius of each light
    int* p; // light positions
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
int findLight(int lastpt, vector<int> &lights, vector<int> &positions, int radius){
    int maxlight = -1;
    int maxpos = INT_MIN;

    for (int i = 0; i < lights.size(); ++i) {
        int light = lights[i];
        int lightpos = positions[light];
        int rend = lightpos + radius;
        int lend = lightpos - radius;
        if(rend <= lastpt) continue;
        if(lend > lastpt) break;
        if(lastpt <= rend && lastpt >= lend && lightpos > maxpos){
            maxpos = lightpos;
            maxlight = light;
        }
    }
    return maxlight;
}

vector<int> solveProblem(vector<Case> &cases){
    vector<int> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}

int solveCase(Case &thisCase){
    vector<int> lights(thisCase.n);
    for (int i = 0; i < thisCase.n; ++i) lights[i] = i;
    vector<int> positions(thisCase.n);
    positions.assign(thisCase.p, thisCase.p + thisCase.n);

    sort(lights.begin(), lights.end(), [&](int a, int b){
        return positions[a] < positions[b];
    });

    int count = 0;
    int lastpt = 0;
    while(lastpt < thisCase.l){
        int light = findLight(lastpt, lights, positions, thisCase.d);
        if(light != -1){
            lastpt = positions[light] + thisCase.d;
            count++;
        }else{
            return -1;
        }
    }
    return count;
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

        scanf("%d", &thisCase.l);
        scanf("%d", &thisCase.n);
        scanf("%d", &thisCase.d);

        thisCase.p = new int[thisCase.n];

        for(int j = 0; j < thisCase.n; j++){
            scanf("%d", &thisCase.p[j]);
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

        fscanf(file, "%d", &thisCase.l);
        fscanf(file, "%d", &thisCase.n);
        fscanf(file, "%d", &thisCase.d);

        thisCase.p = new int[thisCase.n];

        for(int j = 0; j < thisCase.n; j++){
            fscanf(file, "%d", &thisCase.p[j]);
        }
        cases[i] = thisCase;
    }

    fclose (file);
    return cases;
}


// writers
void writeOutput(vector<int> &answers){
    for(int i = 0; i < answers.size(); i++){
        if(answers[i] == -1){
            printf("Case #%d: impossible\n", i+1);
        }else{
            printf("Case #%d: %d\n", i+1, answers[i]);
        }
    }
}

void writeOutputFile(vector<int> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < answers.size(); i++){
        if(answers[i] == -1){
            fprintf(file, "Case #%d: impossible\n", i+1);
        }else{
            fprintf(file, "Case #%d: %d\n", i+1, answers[i]);
        }
    }

    fclose(file);
}