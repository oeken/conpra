#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <climits>
#include <queue>
#include <ctime>
#include <tuple>

using namespace std;

struct Case {
    int N; // #friends
    int M; // #presents
    char** p; // preferences
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


// graph constrcutors
void processToken(string &s, vector<int> &ts){
    int pos = 0;
    string dash("-");

    if((pos = s.find(dash)) != string::npos) { // yes dash
        string left = s.substr(0, pos);
        string right = s.substr(pos + 1, s.size());
        int lnum = stoi(left);
        int rnum = stoi(right);
        for (int j = lnum; j <= rnum; j++) ts.push_back(j);
    }else{ // no dash
        ts.push_back(stoi(s));
    }
}

vector<vector<int>> constructTargets(Case &thisCase){
    vector<vector<int>> targets(thisCase.N);
    string comma(",");
    string dash("-");
    int pos = 0;

    for(int i = 0; i < thisCase.N; i++) {
        vector<int> ts;
        string s(thisCase.p[i]);

        if(s.size() > 0) {
            if ((s.find(comma)) == string::npos) { // no comma
                processToken(s, ts);
            } else { // yes comma
                while ((pos = s.find(comma)) != string::npos) { // for each element
                    string token = s.substr(0, pos);
                    processToken(token, ts);
                    s.erase(0, s.find(comma) + comma.length());
                }
                processToken(s, ts);
            }
        }

//        }else{
//            for(int j = 1; j <= thisCase.M; j++) ts.push_back(j);
//        }

        targets[i] = ts;
    }
    return targets;
}

vector<vector<int>> constructGlist(Case &thisCase, vector<vector<int>> &targets){
    int N = thisCase.N + thisCase.M + 2;
    vector<vector<int>> Glist(N);

    for(int i = 0; i < thisCase.N; i++){
        int src = i+1;
        for(int j = 0; j < targets[i].size(); j++){
            int trg = thisCase.N + targets[i][j];
            Glist[src].push_back(trg); // directed graph - but push the reverse direction any way with value 0
            Glist[trg].push_back(src); // todo: may not work
        }
    }

    for(int i = 1; i <= thisCase.N; i++) Glist[0].push_back(i); // source to friends
    for(int i = thisCase.N+1; i <= thisCase.N+thisCase.M; i++) Glist[i].push_back(N-1); // presents to sink

    return Glist;
}

vector<vector<int>> constructGmatrix(Case &thisCase, vector<vector<int>> &targets){
    int N = thisCase.N + thisCase.M + 2;
    vector<vector<int>> Gmatrix(N);
    for(int i = 0; i < N; i++){
        vector<int> v(N);
        Gmatrix[i] = v;
    }

    for(int i = 0; i < thisCase.N; i++){
        int src = i+1;
        for(int j = 0; j < targets[i].size(); j++){
            int trg = thisCase.N + targets[i][j];
            Gmatrix[src][trg] = 1; // directed graph
        }
    }

    for(int i = 1; i <= thisCase.N; i++) Gmatrix[0][i] = 1; // source to friends
    for(int i = thisCase.N+1; i <= thisCase.N+thisCase.M; i++) Gmatrix[i][N-1] = 1; // presents to sink

    return Gmatrix;
}



// maxflow utilities
int residual(vector<vector<int>> &Glist, vector<vector<int>> &Gmatrix, vector<vector<int>> &flow, int src, int trg){
    int r = 0;

    // an edge exists src --> trg
    if(Gmatrix[src][trg] != 0) r += Gmatrix[src][trg] - flow[src][trg];

    // an edge exists trg --> src
    if(Gmatrix[trg][src] != 0) r += flow[trg][src];

    return r;
}

void augmentFlow(int src, int trg, int flw, vector<vector<int>> &Gmatrix, vector<vector<int>> &flow){

    if(Gmatrix[trg][src] != 0){
        int flowbackward = max(0, flow[trg][src] - flw);
        int leftflow = max(0, flw - flow[trg][src]);
        int flowforward = min(Gmatrix[src][trg] ,flow[src][trg] + leftflow);

        flow[src][trg] = flowforward;
        flow[trg][src] = flowbackward;
    }else{
        int flowforward = min(Gmatrix[src][trg] ,flow[src][trg] + flw);
        flow[src][trg] = flowforward;
    }
}



// dinics utilities
vector<int> computeLevels(vector<vector<int>> &Glist, vector<vector<int>> &Gmatrix, vector<vector<int>> &flow){
    int N = Glist.size();
    int s = 0;

    // init bfs
    vector<int> levels(N);
    for(int i = 0; i < N; i++) levels[i] = -1;
    vector<bool> visited(N);
    deque<int> queue;

    // push s
    int node = s;
    visited[node] = true;
    levels[node] = 0;
    queue.push_back(node);

    while(!queue.empty()){
        int node = queue.front();
        queue.pop_front();

        // visit neighbours of node
        for(int i = 0; i < Glist[node].size(); i++){
            int trg = Glist[node][i];
            int res = residual(Glist, Gmatrix, flow, node, trg);
            if(res > 0 && !visited[trg]){
                levels[trg] = levels[node] + 1;
                visited[trg] = true;
                queue.push_back(trg);
            }
        }
    }
    return levels;
}

int sendFlow(int node, int flw, int trg, vector<vector<int>> &Glist, vector<vector<int>> &Gmatrix, vector<vector<int>> &flow, vector<int> &levels){

    if(node == trg) return flw;

    for(int i = 0; i < Glist[node].size(); i++){
        int next = Glist[node][i];
        int res = residual(Glist,Gmatrix, flow, node, next);
        if(res > 0 && levels[node] + 1 == levels[next]){
            int current_flw = min(flw, res);

            int temp_flw = sendFlow(next, current_flw, trg, Glist, Gmatrix, flow, levels);

            if(temp_flw > 0){
                augmentFlow(node, next, temp_flw, Gmatrix, flow);
                return temp_flw;
            }
        }
    }

    return 0;
}



// dinics
int dinicsMaxFlow(vector<vector<int>> &Glist, vector<vector<int>> &Gmatrix){
    int N = Glist.size();
    int s = 0;
    int t = N-1;

    // init all flows with 0s
    vector<vector<int>> flow(N);
    for(int i = 0; i < N; i++){
        vector<int> v(N);
        flow[i] = v;
    }

    // compute levels
    vector<int> levels = computeLevels(Glist, Gmatrix, flow);
    int maxflow = 0;

    // while a blocking flow on lg exists, compute it and augment to original flow
    while(levels[t] != -1){
        while(int flw = sendFlow(s, INT_MAX, t, Glist, Gmatrix, flow, levels)){
            maxflow += flw;
        }
        levels = computeLevels(Glist, Gmatrix, flow);
    }

    return maxflow;
}



// standard stuff
vector<int> solveProblem(vector<Case> &cases){
    vector<int> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}

int solveCase(Case &thisCase){
    if(thisCase.N > thisCase.M) return 0;

    vector<vector<int>> targets = constructTargets(thisCase);
    for(int i = 0; i < thisCase.N; i++) if(!targets[i].size()) return 0;

    vector<vector<int>> Glist = constructGlist(thisCase, targets);
    vector<vector<int>> Gmatrix = constructGmatrix(thisCase, targets);

    int maxflow = dinicsMaxFlow(Glist, Gmatrix);

    if(maxflow == thisCase.N) { return 1; }
    else{ return 0;}

}


// readers
vector<Case> readInput(){
    // read # of cases
    char dummy;
    int t;
    scanf("%d", &t);

    // read each case
    vector<Case> cases(t);
    for(int i = 0; i < t; i++){
        Case thisCase;
        scanf("%d", &thisCase.N);
        scanf("%d", &thisCase.M);
        scanf("%c", &dummy); // skip \n

        thisCase.p = new char*[thisCase.N];

        for(int j = 0; j < thisCase.N; j++){
            char* c = new char[2*thisCase.M];
            scanf("%[^\n]", c);
            scanf("%c", &dummy); // skip \n
            thisCase.p[j] = c;
        }
        cases[i] = thisCase;
    }

    return cases;
}

vector<Case> readInputFile(){
    FILE* file;
    file = fopen (inFileName.c_str(),"r");

    // read # of cases
    char dummy;
    int t;
    fscanf(file, "%d", &t);

    // read each case
    vector<Case> cases(t);
    for(int i = 0; i < t; i++){
        Case thisCase;
        fscanf(file, "%d", &thisCase.N);
        fscanf(file, "%d", &thisCase.M);
        fscanf(file, "%c", &dummy); // skip \n

        thisCase.p = new char*[thisCase.N];

        for(int j = 0; j < thisCase.N; j++){
            char* c = new char[2*thisCase.M];
            fscanf(file, "%[^\n]", c);
            fscanf(file, "%c", &dummy); // skip \n
            thisCase.p[j] = c;
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

