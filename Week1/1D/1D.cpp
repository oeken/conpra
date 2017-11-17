#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


vector<vector<vector<int>>> readInput();
vector<vector<vector<int>>> readInputFile();

void writeOutput(vector<vector<vector<int>>> &cases);
void writeOutputFile(vector<vector<vector<int>>> &cases);

void solveCases(vector<vector<vector<int>>> &cases);
void solveCase(vector<vector<int>> &currentCase);

string inFileName = "in.txt";
string outFileName = "out.txt";

// sort using a custom function object
struct {
    bool operator()(vector<int> a, vector<int> b) const{
        for(int i = 0; i < 5; i++){
            if(a[i] > b[i]){
                return true;
            }
            else if(a[i] == b[i]){
                continue;
            }else{
                return false;
            }
        }
        return true;
    }
} comparator;

int main() {
    vector<vector<vector<int>>> cases = readInput();
//    vector<vector<vector<int>>> cases = readInputFile();

    solveCases(cases);

//    writeOutputFile(cases);
    writeOutput(cases);
    return 0;
}


void solveCases(vector<vector<vector<int>>> &cases){
    for(int i = 0; i < cases.size(); i++){
        solveCase(cases[i]);
    }
}

void solveCase(vector<vector<int>> &currentCase){
    for(int i = 0; i < currentCase.size(); i++){
        sort(currentCase[i].begin(), currentCase[i].end(), [](int a, int b) {
            return b < a;
        });
    }
    sort(currentCase.begin(), currentCase.end(), comparator);
}



vector<vector<vector<int>>> readInput(){
    // read # of cases
    int caseCount;
    scanf("%d", &caseCount);
    vector<vector<vector<int>>> cases(caseCount);

    for(int i = 0; i < caseCount; i++){ // for each competition/case
        int schoolCount;
        scanf("%d", &schoolCount);

        vector<vector<int>> schools(schoolCount);
        for(int j = 0; j < schoolCount; j++){ // for each school

            vector<int> skills(5);
            for(int k = 0; k < 5; k++){ // for each student
                int skill;
                scanf("%d", &skill);
                skills[k] = skill;
            }
            schools[j] = skills;
        }
        cases[i] = schools;
    }

    return cases;
}


vector<vector<vector<int>>> readInputFile(){
    FILE* file;
    file = fopen (inFileName.c_str(),"r");

    // read # of cases
    int caseCount;
    fscanf(file, "%d", &caseCount);
    vector<vector<vector<int>>> cases(caseCount);

    for(int i = 0; i < caseCount; i++){ // for each competition/case
        int schoolCount;
        fscanf(file, "%d", &schoolCount);

        vector<vector<int>> schools(schoolCount);
        for(int j = 0; j < schoolCount; j++){ // for each school

            vector<int> skills(5);
            for(int k = 0; k < 5; k++){ // for each student
                int skill;
                fscanf(file, "%d", &skill);
                skills[k] = skill;
            }
            schools[j] = skills;
        }
        cases[i] = schools;
    }

    fclose (file);
    return cases;
}


void writeOutput(vector<vector<vector<int>>> &cases){
    for(int i = 0; i < cases.size(); i++){
        printf("Case #%d:\n", i+1);

        for(int j = 0; j < cases[i].size(); j++){
            for(int k = 0; k < cases[i][j].size(); k++){
                printf("%d ", cases[i][j][k]);
            }
            printf("\n");
        }
    }
}


void writeOutputFile(vector<vector<vector<int>>> &cases){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < cases.size(); i++){
        fprintf(file, "Case #%d:\n", i+1);

        for(int j = 0; j < cases[i].size(); j++){
            for(int k = 0; k < cases[i][j].size(); k++){
                fprintf(file, "%d ", cases[i][j][k]);
            }
            fprintf(file, "\n");
        }
    }

    fclose(file);
}

