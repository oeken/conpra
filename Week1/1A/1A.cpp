#include <iostream>
#include <vector>

using namespace std;


vector<string> readInput();
void writeOutput(vector<string> &answers);
vector<string> readInputFile();
void writeOutputFile(vector<string> &answers);
vector<string> solveProblem(vector<string> &names);

string inFileName = "in.txt";
string outFileName = "out.txt";

int main() {
//    vector<string> names = readInput();
//    writeOutput(answers);

    vector<string> names = readInputFile();
    vector<string> answers = solveProblem(names);
    writeOutputFile(answers);
    return 0;
}


vector<string> solveProblem(vector<string> &names){
    vector<string> answers(names.size());
    for(int i = 0; i < names.size(); i++){
        answers[i] = string("Case #") + to_string(i+1) + string(": Hello ") + names[i] + string("!");
    }
    return answers;
}



// STDIN STDOUT Readers and Writers
vector<string> readInput(){
    // read # of cases
    int caseCount;
    scanf("%d", &caseCount);

    // read each name
    vector<string> names(caseCount);
    for(int i = 0; i < caseCount; i++){
        char* name = new char[100];
        scanf("%s", name);
        names[i] = string(name);
    }

    return names;
}

void writeOutput(vector<string> &answers){
    for(int i = 0; i < answers.size(); i++){
        printf(answers[i].c_str());
        printf("\n");
    }
}




// FILEIN, FILEOUT Readers and Writers
vector<string> readInputFile(){
    FILE* file;
    file = fopen (inFileName.c_str(),"r");

    // read # of cases
    int caseCount;
    fscanf(file, "%d", &caseCount);
    // read each name
    vector<string> names(caseCount);
    for(int i = 0; i < caseCount; i++){
        char* name = new char[100];
        fscanf(file, "%s", name);
        names[i] = string(name);
    }

    fclose (file);
    return names;
}

void writeOutputFile(vector<string> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < answers.size(); i++){
        fprintf(file, "%s", answers[i].c_str());
        fprintf(file, "\n");
    }

    fclose(file);
}