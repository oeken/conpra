#include <iostream>
#include <vector>

using namespace std;

#define C 299792458

vector<unsigned long> readInput();
void writeOutput(vector<unsigned long> &answers);
vector<unsigned long> readInputFile();
void writeOutputFile(vector<unsigned long> &answers);
vector<unsigned long> solveProblem(vector<unsigned long> &masses);

string inFileName = "in.txt";
string outFileName = "out.txt";

int main() {
    vector<unsigned long> masses = readInput();
//    vector<unsigned long> masses = readInputFile();

    vector<unsigned long> answers = solveProblem(masses);

//    writeOutputFile(answers);
    writeOutput(answers);
    return 0;
}


vector<unsigned long> solveProblem(vector<unsigned long> &masses){
    vector<unsigned long> answers(masses.size());
    for(int i = 0; i < masses.size(); i++){
        answers[i] = masses[i] * C * C;
    }
    return answers;
}



// STDIN STDOUT Readers and Writers
vector<unsigned long> readInput(){
    // read # of cases
    int caseCount;
    scanf("%d", &caseCount);

    // read each name
    vector<unsigned long> masses(caseCount);
    for(int i = 0; i < caseCount; i++){
        unsigned long mass;
        scanf("%lu", &mass);
        masses[i] = mass;
    }

    return masses;
}

void writeOutput(vector<unsigned long> &answers){
    for(int i = 0; i < answers.size(); i++){
        printf("Case #%d: %lu\n", i+1, answers[i]);
    }
}




// FILEIN, FILEOUT Readers and Writers
vector<unsigned long> readInputFile(){
    FILE* file;
    file = fopen (inFileName.c_str(),"r");

    // read # of cases
    int caseCount;
    fscanf(file, "%d", &caseCount);
    // read each name
    vector<unsigned long> masses(caseCount);
    for(int i = 0; i < caseCount; i++){
        unsigned long mass;
        fscanf(file, "%lu", &mass);
        masses[i] = mass;
    }

    fclose (file);
    return masses;
}

void writeOutputFile(vector<unsigned long> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < answers.size(); i++){
        fprintf(file, "Case #%d: %lu\n", i+1, answers[i]);
    }

    fclose(file);
}