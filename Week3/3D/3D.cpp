#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <deque>

using namespace std;

struct Case {
    int n;      // number of sticks
    int m;      // number of connections
    int* p;     // pts of sticks
    int* a;     // connections: top
    int* b;     // connections: bottom
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
    for(int i = 0; i < thisCase.m; i++){
        int a = thisCase.a[i]-1;
        int b = thisCase.b[i]-1;
        list[a].push_back(b);
    }
    return list;
}

vector<int> constructIndegreeTable(Case &thisCase){
    vector<int> indegrees(thisCase.n);
    for(int i = 0; i < thisCase.m; i++){
        int b = thisCase.b[i]-1;
        indegrees[b]++;
    }
    return indegrees;
}

vector<int> solveProblem(vector<Case> &cases){
    vector<int> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}

int solveCase(Case &thisCase){
    vector<vector<int>> adjList = constructAdjList(thisCase);
    vector<int> indegrees = constructIndegreeTable(thisCase);

    // init queue with 0 indegrees
    deque<int> q;
    for(int i = 0; i < indegrees.size(); i++){
        if(!indegrees[i]) q.push_back(i);
    }

    int answer = 0;
    while(!q.empty()){
        int node = q.front();
        answer += thisCase.p[node];
        q.pop_front();
        for(int i = 0; i < adjList[node].size(); i++){
            int target_node = adjList[node][i];
            indegrees[target_node]--;
            if(!indegrees[target_node]) q.push_back(target_node);
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
        scanf("%d", &thisCase.m);

        thisCase.p = new int[thisCase.n];
        thisCase.a = new int[thisCase.m];
        thisCase.b = new int[thisCase.m];

        for(int j = 0; j < thisCase.n; j++){
            scanf("%d", &thisCase.p[j]);
        }

        for(int j = 0; j < thisCase.m; j++){
            scanf("%d", &thisCase.a[j]);
            scanf("%d", &thisCase.b[j]);
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
        fscanf(file, "%d", &thisCase.m);

        thisCase.p = new int[thisCase.n];
        thisCase.a = new int[thisCase.m];
        thisCase.b = new int[thisCase.m];

        for(int j = 0; j < thisCase.n; j++){
            fscanf(file, "%d", &thisCase.p[j]);
        }

        for(int j = 0; j < thisCase.m; j++){
            fscanf(file, "%d", &thisCase.a[j]);
            fscanf(file, "%d", &thisCase.b[j]);
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