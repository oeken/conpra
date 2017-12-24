#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Case {
    int v; // number of variables
    int c; // number of clauses
    int** f; // formula
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
    vector<Case> cases = readInputFile();
//    vector<Case> cases = readInput();

    vector<int> answers = solveProblem(cases);

//    writeOutput(answers);
    writeOutputFile(answers);
    return 0;
}


// helpers
vector<vector<int>> getFormula(Case &thisCase){
    vector<vector<int>> formula;

    for(int i = 0; i < thisCase.c; i++){
        int count = thisCase.f[i][0];
        if(count){
            vector<int> v(count);
            for(int j = 0; j < count; j++){
                v[j] = thisCase.f[i][j+1];
            }
            formula.push_back(v);
        }
    }
    return formula;
}

// returns 0, 1, -1
int pureVar(int var, vector<vector<int>> &formula){
    int pole = -1;
    for(int j = 0; j < formula.size(); j++){
        for(int k = 0; k < formula[j].size(); k++){
            if(abs(formula[j][k])-1 == var){
                int curr = formula[j][k] > 0;
                if(pole == -1){
                    pole = curr;
                }else if(pole != curr){
                    return -1; // not a pure literal
                }
            }
        }
    }
    if(pole == -1) return 1; // not involved in any clause
    else pole; // is a pure literal
}

void assignPureLiterals(vector<vector<int>> &formula, vector<int> &assignments){
    for(int i = 0; i < assignments.size(); i++){
        int purity = pureVar(i, formula);
        assignments[i] = purity;
    }
}

int getNext(vector<int> &assignments){
    for(int i = 0; i < assignments.size(); i++){
        if(assignments[i] == -1) return i;
    }
    return -1;
}

int getLiteralValue(int literal, vector<int> &assignments){
    int var = abs(literal)-1;
    int varval = assignments[var];
    if(varval == -1) return -1;

    bool neg = literal < 0;
    if(neg) return (varval+1)%2;
    return varval;
}

bool unsatisfiable(vector<int> &assignments, vector<vector<int>> &formula){
    for(int i = 0; i < formula.size(); i++){ // for each clause
        int unset_count = 0;
        int true_count = 0;
        int false_count = 0;
        for(int j = 0; j < formula[i].size(); j++){ // for each literal
            int val = getLiteralValue(formula[i][j], assignments);
            if(val == -1) {unset_count++; break;}
            if(val == 1) {true_count++; break;}
            if(val == 0) {false_count++;}
        }
        if(true_count == 0 && unset_count == 0 && false_count > 0)
            return true;
    }
    return false;
}

bool solveSAT(vector<int> &assignments, vector<vector<int>> &formula){
    if(satisfied(formula)) return true;

    // choose var here
    int var = getNext(assignments);
    if(var == -1) return true;

    // set true
    assignments[var] = 1;
    if(!unsatisfiable(assignments, formula) && solveSAT(assignments, loneness, cardinality, formula)) return true;

    // set false
    assignments[var] = 0;
    if(!unsatisfiable(assignments, formula) && solveSAT(assignments, loneness, cardinality, formula)) return true;

    // set unset, backtrack
    assignments[var] = -1;
    return false;
}

vector<int> solveProblem(vector<Case> &cases){
    vector<int> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}


int solveCase(Case &thisCase){
    vector<vector<int>> formula = getFormula(thisCase);
    if(formula.size() != thisCase.c) return false;

    vector<int> assignments(thisCase.v);
    for(int i = 0; i < thisCase.v; i++) assignments[i] = -1;
    assignPureLiterals(formula, assignments);

    bool sat = solveSAT(assignments, formula);
    return sat;
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
        scanf("%d", &thisCase.v);
        scanf("%d", &thisCase.c);

        thisCase.f = new int*[thisCase.c];

        for(int j = 0; j < thisCase.c; j++){
            int counter = 0;
            thisCase.f[j] = new int[thisCase.v+1];
            int literal;
            scanf("%d", &literal);
            while(literal){
                counter++;
                thisCase.f[j][counter] = literal;
                scanf("%d", &literal);
            }
            thisCase.f[j][0] = counter;
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
        fscanf(file, "%d", &thisCase.v);
        fscanf(file, "%d", &thisCase.c);

        thisCase.f = new int*[thisCase.c];

        for(int j = 0; j < thisCase.c; j++){
            int counter = 0;
            thisCase.f[j] = new int[thisCase.v+1];
            int literal;
            fscanf(file, "%d", &literal);
            while(literal){
                counter++;
                thisCase.f[j][counter] = literal;
                fscanf(file, "%d", &literal);
            }
            thisCase.f[j][0] = counter;
        }
        cases[i] = thisCase;
    }

    fclose (file);
    return cases;
}


// writers
void writeOutput(vector<int> &answers){
    for(int i = 0; i < answers.size(); i++){
        if(answers[i]){
            printf("Case #%d: yes\n", i+1);
        }else{
            printf("Case #%d: no\n", i+1);
        }
    }
}

void writeOutputFile(vector<int> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < answers.size(); i++){
        if(answers[i]){
            fprintf(file, "Case #%d: yes\n", i+1);
        }else{
            fprintf(file, "Case #%d: no\n", i+1);
        }
    }

    fclose(file);
}