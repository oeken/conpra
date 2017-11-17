#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <deque>
#include <ctime>

using namespace std;

struct Case {
    int n;   // number of people
    int** adj; // adj mat
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
vector<vector<int>> constructAdjList(Case &thisCase){
    vector<vector<int>> list(thisCase.n);
    for(int i = 0; i < thisCase.n; i++){
        for(int j = 0; j < thisCase.n; j++){
            if(thisCase.adj[j][i]) list[i].push_back(j);
        }
    }
    return list;
}

vector<vector<int>> constructRevAdjList(Case &thisCase){
    vector<vector<int>> list(thisCase.n);
    for(int i = 0; i < thisCase.n; i++){
        for(int j = 0; j < thisCase.n; j++){
            if(thisCase.adj[i][j]) list[i].push_back(j);
        }
    }
    return list;
}

vector<int> solveProblem(vector<Case> &cases){
    vector<int> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}

int solveCase(Case &thisCase){
    // make adj list
    vector<vector<int>> adj_list = constructAdjList(thisCase);
    vector<vector<int>> rev_adj_list = constructRevAdjList(thisCase);

    // run bfs
    deque<int> q;
    vector<int> visited_list;
    vector<bool> visited_table(thisCase.n);

    visited_list.push_back(0);
    visited_table[0] = true;
    q.push_back(0);

    while(!q.empty()){
        int current = q.front();
        for(int i = 0; i < adj_list[current].size(); i++){
            int candidate = adj_list[current][i];
            if(!visited_table[candidate]){
                visited_list.push_back(candidate);
                visited_table[candidate] = true;
                q.push_back(candidate);
            }
        }
        q.pop_front();
    }

    int answer = 0;
    for(int i = 0; i < visited_list.size(); i++){
        int current = visited_list[i];
        if(current != 0 && visited_table[current]){
            int max = INT_MIN;
            for(int j = 0; j < rev_adj_list[current].size(); j++){
                int target = rev_adj_list[current][j];
                if(visited_table[target] && thisCase.adj[current][target] > max) max = thisCase.adj[current][target];
            }
            answer += max;
        }
    }

    return answer;

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

        thisCase.adj = new int*[thisCase.n];

        for(int j = 0; j < thisCase.n; j++){
            thisCase.adj[j] = new int[thisCase.n];
            for(int k = 0; k < thisCase.n; k++){
                scanf("%d", &thisCase.adj[j][k]);
            }
        }

        cases[i] = thisCase;
    }

    return cases;
}

vector<Case> readInputFile(){
    FILE* file;
    file = fopen (inFileName.c_str(),"r");
    int dummy;

    // read # of cases
    int t;
    fscanf(file, "%d", &t);

    // read each case
    vector<Case> cases(t);
    for(int i = 0; i < t; i++){
        Case thisCase;
        fscanf(file, "%d", &thisCase.n);

        thisCase.adj = new int*[thisCase.n];

        for(int j = 0; j < thisCase.n; j++){
            thisCase.adj[j] = new int[thisCase.n];
            for(int k = 0; k < thisCase.n; k++){
                fscanf(file, "%d", &thisCase.adj[j][k]);
            }
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