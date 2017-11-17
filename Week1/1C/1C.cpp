#include <iostream>
#include <vector>

using namespace std;

vector<string> readInput();
void writeOutput(vector<string> &answers);
vector<string> readInputFile();
void writeOutputFile(vector<string> &answers);
vector<string> solveProblem(vector<string> &texts);

string inFileName = "in.txt";
string outFileName = "out.txt";

int main() {
    vector<string> texts = readInput();
//    vector<string> texts = readInputFile();

    vector<string> answers = solveProblem(texts);

//    writeOutputFile(answers);
    writeOutput(answers);
    return 0;
}


vector<string> solveProblem(vector<string> &texts){
    vector<string> answers(texts.size());
    for(int i = 0; i < texts.size(); i++){
        string answer;

        int j = 0;
        while(j < texts[i].size()){
            if(texts[i].size() - j >= 5){
                if(texts[i].substr(j,5).compare(string("enten")) == 0){
                    answer.append(string("ierende"));
                    j = j + 5;
                }
                else if(texts[i].substr(j,5).compare(string("entin")) == 0){
                    answer.append(string("ierende"));
                    j = j + 5;
                }
                else if(texts[i].substr(j,3).compare(string("ent")) == 0){
                    answer.append(string("ierender"));
                    j = j + 3;
                }
                else{
                    answer.append(texts[i].substr(j,1));
                    j++;
                }
            }
            else if(texts[i].size() - j >= 3){
                if(texts[i].substr(j,3).compare(string("ent")) == 0){
                    answer.append(string("ierender"));
                    j = j + 3;
                }else{
                    answer.append(texts[i].substr(j,1));
                    j++;
                }
            }
            else{
                answer.append(texts[i].substr(j, texts[i].size() - j));
                j = texts[i].size();
            }
        }
        answers[i] = answer;
    }
    return answers;
}




// STDIN STDOUT Readers and Writers
vector<string> readInput(){
    // read # of cases
    int caseCount;
    scanf("%d", &caseCount);

    // read each
    vector<string> readings(caseCount);
    for(int i = 0; i < caseCount; i++){

        int numOfLines;
        char dummy;
        scanf("%d", &numOfLines);
        scanf("%c", &dummy); // read the \n and proceed to next line

        string text;
        for(int j = 0; j < numOfLines; j++){
            char lineText[10000] = {0};
            scanf("%10000[^\n]", lineText);
            scanf("%c", &dummy); // read the \n and proceed to next line

            text.append(lineText);
            if(j != numOfLines - 1) text.append("\n");
        }
        readings[i] = text;
    }

    return readings;
}

void writeOutput(vector<string> &answers){
    for(int i = 0; i < answers.size(); i++){
        printf("Case #%d:\n%s\n", i+1, answers[i].c_str());
    }
}




// FILEIN, FILEOUT Readers and Writers
vector<string> readInputFile(){
    FILE* file;
    file = fopen (inFileName.c_str(),"r");

    // read # of cases
    int caseCount;
    fscanf(file, "%d", &caseCount);

    // read each
    vector<string> readings(caseCount);
    for(int i = 0; i < caseCount; i++){

        int numOfLines;
        char dummy;
        fscanf(file, "%d", &numOfLines);
        fscanf(file, "%c", &dummy); // read the \n and proceed to next line

        string text;
        for(int j = 0; j < numOfLines; j++){
            char lineText[10000] = {0};
            fscanf(file, "%10000[^\n]", lineText);
            fscanf(file, "%c", &dummy); // read the \n and proceed to next line

            text.append(lineText);
            if(j != numOfLines - 1) text.append("\n");
        }
        readings[i] = text;
    }

    fclose (file);
    return readings;
}

void writeOutputFile(vector<string> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < answers.size(); i++){
        fprintf(file, "Case #%d:\n%s\n", i+1, answers[i].c_str());
    }

    fclose(file);
}