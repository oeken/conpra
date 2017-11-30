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
    int N; // #fountains
    int K; // #dist. stations
    int M; // #wrappers
    int L; // #belts
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
int dinicsMaxFlow(vector<vector<int>> &Glist, vector<vector<int>> &Gmatrix);
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

//    int V = 700;
//    int E = 50000;
//
//    vector<vector<int>> Glist(V);
//    vector<vector<int>> Gmatrix(V);
//    for(int i = 0; i < V; i++){
//        vector<int> v(V);
//        Gmatrix[i] = v;
//    }
//
//    for(int i = 0; i < E; i++){
//        int src = 0 + (rand() % static_cast<int>(V));
//        int trg = 0 + (rand() % static_cast<int>(V));
//        Gmatrix[src][trg] += 1;
//        Gmatrix[trg][src] += 1;
//    }
//
//    for(int i = 0; i < V; i++){
//        for(int j = i+1; j < V; j++){
//            if(Gmatrix[i][j]){
//                Glist[i].push_back(j);
//                Glist[j].push_back(i);
//            }
//        }
//    }
//
//    int maxflow = dinicsMaxFlow(Glist, Gmatrix);
//    cout << maxflow;

    return 0;
}


// graph constrcutors
vector<vector<int>> constructGlist(Case &thisCase){
    int nodeCount = thisCase.N + thisCase.K + thisCase.M + 2; // +2 for super nodes
    vector<vector<int>> Glist(nodeCount);
    for(int i = 0; i < thisCase.L; i++){
        int v = thisCase.v[i];
        int w = thisCase.w[i];

        bool check1 = v != w; // add if not a self edge
        bool check2 = find(begin(Glist[v]), end(Glist[v]), w) == end(Glist[v]); // add if not present yet

        bool check3 = v <= thisCase.N && w > thisCase.N + thisCase.K; // fountain --> wrapper
        bool check4 = w <= thisCase.N && v > thisCase.N + thisCase.K; // wrapper --> fountain

        bool check5 = v <= thisCase.N && w <= thisCase.N; // fountain --> fountain
        bool check6 = v > thisCase.N + thisCase.K && w > thisCase.N + thisCase.K; // wrapper --> wrapper

        if(check1 && check2 && !check3 && !check4 && !check5 && !check6){
            Glist[v].push_back(w); // undirected graph
            Glist[w].push_back(v); // undirected graph
        }
    }

    // add: super source --> fountain
    for(int i = 1; i <= thisCase.N; i++) Glist[0].push_back(i);

    // add: wrapper --> super sink
    for(int i = thisCase.N + thisCase.K + 1; i <= nodeCount - 2; i++) Glist[i].push_back(nodeCount-1);

    return Glist;
}

tuple<vector<vector<int>>, int> constructGmatrix(Case &thisCase){
    int nodeCount = thisCase.N + thisCase.K + thisCase.M + 2;

    // init with -1s indicating edge is not present
    vector<vector<int>> Gmatrix(nodeCount);
    for(int i = 0; i < nodeCount; i++){
        vector<int> v(nodeCount);
        Gmatrix[i] = v;
    }

    // todo: f-f && w-w çıkar
    // todo: index'lere dikkat et

    int directCap = 0;
    // read capacities
    for(int i = 0; i < thisCase.L; i++){
        int v = thisCase.v[i];
        int w = thisCase.w[i];
        int c = thisCase.c[i];

        bool check1 = v <= thisCase.N && w > thisCase.N + thisCase.K; // fountain --> wrapper
        bool check2 = w <= thisCase.N && v > thisCase.N + thisCase.K; // wrapper --> fountain

        bool check3 = v != w; // if not a self edge

        bool check4 = v <= thisCase.N && w <= thisCase.N; // fountain --> fountain
        bool check5 = v > thisCase.N + thisCase.K && w > thisCase.N + thisCase.K; // wrapper --> wrapper

        if(check1 || check2) {
            directCap += c;
        }else if(check3 && !check4 && !check5){
            Gmatrix[v][w] += c;  // add --> merge multiple edges
            Gmatrix[w][v] += c;  // undirected --> add both ways
        }
    }

    // capacity of super source --> fountain is INF
    for(int i = 1; i <= thisCase.N; i++) Gmatrix[0][i] = INT_MAX;

    // capacity of wrapper --> super sink is INF
    for(int i = thisCase.N + thisCase.K + 1; i <= nodeCount - 2; i++) Gmatrix[i][nodeCount-1] = INT_MAX;

    tuple<vector<vector<int>>, int> t(Gmatrix, directCap);
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

void augmentFlow(int src,
                 int trg,
                 int flw,
                 vector<vector<int>> &Gmatrix,
                 vector<vector<int>> &flow){

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

int sendFlow(int node,
               int flw,
               int trg,
              vector<vector<int>> &Glist,
              vector<vector<int>> &Gmatrix,
               vector<vector<int>> &flow,
               vector<int> &levels){

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


// drivers
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
    tuple<vector<vector<int>>, int> t = constructGmatrix(thisCase);
    tie(Gmatrix, directCap) = t;

     int maxflow = dinicsMaxFlow(Glist, Gmatrix);

    return maxflow + directCap;
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
        scanf("%d", &thisCase.N);
        scanf("%d", &thisCase.K);
        scanf("%d", &thisCase.M);
        scanf("%d", &thisCase.L);

        thisCase.v = new int[thisCase.L];
        thisCase.w = new int[thisCase.L];
        thisCase.c = new int[thisCase.L];

        for(int j = 0; j < thisCase.L; j++){
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
        fscanf(file, "%d", &thisCase.N);
        fscanf(file, "%d", &thisCase.K);
        fscanf(file, "%d", &thisCase.M);
        fscanf(file, "%d", &thisCase.L);

        thisCase.v = new int[thisCase.L];
        thisCase.w = new int[thisCase.L];
        thisCase.c = new int[thisCase.L];

        for(int j = 0; j < thisCase.L; j++){
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

