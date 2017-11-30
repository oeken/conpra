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
    int L; // #policemen
    int N; // #nodes
    int M; // #links
    int* v; // node 1
    int* w; // node 2
    int* c; // capacity
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
vector<vector<int>> constructGlist(Case &thisCase){
    vector<vector<int>> Glist(thisCase.N);
    for(int i = 0; i < thisCase.M; i++){
        int v = thisCase.v[i]-1;
        int w = thisCase.w[i]-1;

        bool check1 = v != w; // add if not a self edge
        bool check2 = find(begin(Glist[v]), end(Glist[v]), w) == end(Glist[v]); // add if not present yet

        bool check3 = v == 0 && w == thisCase.N-1;       // s --> t
        bool check4 = w == 0 && v == thisCase.N-1;       // t --> s

        if(check1 && check2 && !check3 && !check4){
            Glist[v].push_back(w); // undirected graph
            Glist[w].push_back(v); // undirected graph
        }
    }
    return Glist;
}

tuple<vector<vector<int>>, int, int, int> constructGmatrix(Case &thisCase){
    vector<vector<int>> Gmatrix(thisCase.N);
    for(int i = 0; i < thisCase.N; i++){
        vector<int> v(thisCase.N);
        Gmatrix[i] = v;
    }

    int directCap = 0;
    int sourceCap = 0;
    int targetCap = 0;

    // read capacities
    for(int i = 0; i < thisCase.M; i++){
        int v = thisCase.v[i]-1;
        int w = thisCase.w[i]-1;
        int c = thisCase.c[i];

        bool check1 = v == 0 && w == thisCase.N-1; // s --> t
        bool check2 = w == 0 && v == thisCase.N-1; // t --> s

        bool check3 = v != w; // if not a self edge

        if(check1 || check2) {
            directCap += c;
        }else if(check3){
            if(v == 0 || w == 0) sourceCap += c;
            if(v == thisCase.N-1 || w == thisCase.N-1) targetCap += c;

            Gmatrix[v][w] += c;  // add --> merge multiple edges
            Gmatrix[w][v] += c;  // undirected --> add both ways
        }
    }

    tuple<vector<vector<int>>, int, int, int> t(Gmatrix, directCap, sourceCap, targetCap);
    return t;
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
    vector<vector<int>> Glist = constructGlist(thisCase);
    vector<vector<int>> Gmatrix;
    int directCap;
    int sourceCap;
    int targetCap;

    tuple<vector<vector<int>>, int, int, int> t = constructGmatrix(thisCase);
    tie(Gmatrix, directCap, sourceCap, targetCap) = t;

    if(thisCase.L >= directCap + min(sourceCap, targetCap)) return 1;

    int maxflow = dinicsMaxFlow(Glist, Gmatrix);

    if(thisCase.L >= directCap + maxflow){
        return 1;
    } else{
        return 0;
    }
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
        scanf("%d", &thisCase.L);
        scanf("%d", &thisCase.N);
        scanf("%d", &thisCase.M);

        thisCase.v = new int[thisCase.M];
        thisCase.w = new int[thisCase.M];
        thisCase.c = new int[thisCase.M];

        for(int j = 0; j < thisCase.M; j++){
            scanf("%d", &thisCase.v[j]);
            scanf("%d", &thisCase.w[j]);
            scanf("%d", &thisCase.c[j]);
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
        fscanf(file, "%d", &thisCase.L);
        fscanf(file, "%d", &thisCase.N);
        fscanf(file, "%d", &thisCase.M);

        thisCase.v = new int[thisCase.M];
        thisCase.w = new int[thisCase.M];
        thisCase.c = new int[thisCase.M];

        for(int j = 0; j < thisCase.M; j++){
            fscanf(file, "%d", &thisCase.v[j]);
            fscanf(file, "%d", &thisCase.w[j]);
            fscanf(file, "%d", &thisCase.c[j]);
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

