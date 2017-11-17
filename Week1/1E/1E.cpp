#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

vector<string> readInput();
void writeOutput(vector<long long> &answers);
vector<string> readInputFile();
void writeOutputFile(vector<long long> &answers);

vector<long long> solveProblem(vector<string> &texts);
long long calculate(long long ll1, string &operand, long long ll2);

string inFileName = "in.txt";
string outFileName = "out.txt";

int main() {
    vector<string> texts = readInput();
//    vector<string> texts = readInputFile();

    vector<long long> answers = solveProblem(texts);

//    writeOutputFile(answers);
    writeOutput(answers);
    return 0;
}


vector<long long> solveProblem(vector<string> &texts){
    vector<long long> answers(texts.size());

    for(int i = 0; i < texts.size(); i++){
        string text = texts[i];

        int j = 0;
        int k = 0;

        // first number
        while(k < text.size() && !isalpha(text[k])) k++;
        long long acc = stoll(text.substr(j, k-j));
        j = k;

        while(k < text.size()){
            // read operator
            while(k < text.size() && !isdigit(text[k])) k++;
            string op = text.substr(j, k-j);
            j = k;

            // read num
            while(k < text.size() && !isalpha(text[k])) k++;
            long long num = stoll(text.substr(j, k-j));
            j = k;

            acc = calculate(acc, op, num);
        }

        answers[i] = acc;
    }
    return answers;
}


long long calculate(long long ll1, string &operand, long long ll2){
    if(operand == "plus"){
        return ll1 + ll2;
    }
    else if(operand == "minus"){
        return ll1 - ll2;

    }
    else if(operand == "times"){
        return ll1 * ll2;

    }
    else if(operand == "tothepowerof"){
        return (long long)pow(ll1, ll2);
    }
    else{
        cout << "wtf man";
        return NULL;
    }
}






// STDIN STDOUT Readers and Writers
vector<string> readInput(){
    // read # of cases
    int caseCount;
    scanf("%d", &caseCount);

    // read each
    vector<string> readings(caseCount);
    for(int i = 0; i < caseCount; i++){
        char* text = new char[10000];
        scanf("%s", text);
        readings[i] = string(text);
    }

    return readings;
}

void writeOutput(vector<long long> &answers){
    for(int i = 0; i < answers.size(); i++){
        printf("Case #%d: %lld\n", i+1, answers[i]);
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
        char* text = new char[10000];
        fscanf(file, "%s", text);
        readings[i] = string(text);
    }

    fclose (file);
    return readings;
}

void writeOutputFile(vector<long long> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < answers.size(); i++){
        fprintf(file, "Case #%d: %lld\n", i+1, answers[i]);
    }

    fclose(file);
}