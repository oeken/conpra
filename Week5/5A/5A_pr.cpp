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
    long N; // #fountains
    long K; // #dist. stations
    long M; // #wrappers
    long L; // #belts
    long* v; // node 1
    long* w; // node 2
    long* c; // capacity
};

// readers
vector<Case> readInput();
vector<Case> readInputFile();

// writers
void writeOutput(vector<long> &answers);
void writeOutputFile(vector<long> &answers);

// helpers
vector<long> solveProblem(vector<Case> &cases);
long solveCase(Case &thisCase);

// file names
string inFileName = "in.txt";
string outFileName = "out.txt";


int main() {
    vector<Case> cases = readInputFile();
//    vector<Case> cases = readInput();

    vector<long> answers = solveProblem(cases);

//    writeOutput(answers);
    writeOutputFile(answers);

    return 0;
}


// helpers
vector<vector<long>> constructGlist(Case &thisCase){
    long nodeCount = thisCase.N + thisCase.K + thisCase.M + 2; // +2 for super nodes
    vector<vector<long>> Glist(nodeCount);
    for(long i = 0; i < thisCase.L; i++){
        long v = thisCase.v[i];
        long w = thisCase.w[i];

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
    for(long i = 1; i <= thisCase.N; i++) {
        Glist[0].push_back(i);
        Glist[i].push_back(0);
    }

    // add: wrapper --> super sink
    for(long i = thisCase.N + thisCase.K + 1; i <= nodeCount - 2; i++) {
        Glist[i].push_back(nodeCount-1);
        Glist[nodeCount-1].push_back(i);
    }

    return Glist;
}

tuple<vector<vector<long>>, long> constructGmatrix(Case &thisCase){
    long nodeCount = thisCase.N + thisCase.K + thisCase.M + 2;

    // init with -1s indicating edge is not present
    vector<vector<long>> Gmatrix(nodeCount);
    for(long i = 0; i < nodeCount; i++){
        vector<long> v(nodeCount);
        Gmatrix[i] = v;
    }

    long directCap = 0;
    // read capacities
    for(long i = 0; i < thisCase.L; i++){
        long v = thisCase.v[i];
        long w = thisCase.w[i];
        long c = thisCase.c[i];

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
    for(long i = 1; i <= thisCase.N; i++) Gmatrix[0][i] = INT_MAX;

    // capacity of wrapper --> super sink is INF
    for(long i = thisCase.N + thisCase.K + 1; i <= nodeCount - 2; i++) Gmatrix[i][nodeCount-1] = INT_MAX;

    tuple<vector<vector<long>>, long> t(Gmatrix, directCap);
    return t;
}

// maxflow utilities
long residual(long src, long trg, vector<vector<long>> &Gmatrix, vector<vector<long>> &flow){
    long r = 0;

    // an edge exists src --> trg
    if(Gmatrix[src][trg] > 0) r += Gmatrix[src][trg] - flow[src][trg];

    // an edge exists trg --> src
    if(Gmatrix[trg][src] > 0) r += flow[trg][src];

    return r;
}

void augmentFlow(long src, long trg, long flw, vector<vector<long>> &Gmatrix, vector<vector<long>> &flow){
//    if(Gmatrix[src][trg] > 0){
    if(Gmatrix[trg][src] > 0){ // two sided
        long flowbackward = max<long>(flow[trg][src] - flw, 0);
        long leftflow = max<long>(flw - flow[trg][src], 0);
        long flowforward = min(Gmatrix[src][trg] ,flow[src][trg] + leftflow);

        flow[src][trg] = flowforward;
        flow[trg][src] = flowbackward;
    }else{ // one sided
        long flowforward = min(Gmatrix[src][trg] ,flow[src][trg] + flw);
        flow[src][trg] = flowforward;
    }
//    }
}


// pr utilities
long excess(long node, vector<vector<long>> &Glist, vector<vector<long>> &Gmatrix, vector<vector<long>> &flow){
    long excess = 0;
    for(long i = 0; i < Glist[node].size(); i++){
        long trg = Glist[node][i];
        excess -= flow[node][trg]; // outgoing
        excess += flow[trg][node]; // incoming
    }
    return excess;
}

long overflowNode(vector<vector<long>> &Glist, vector<vector<long>> &Gmatrix, vector<vector<long>> &flow){
    for(long i = 0; i < Glist.size()-1; i++){ // never return last index (i.e. sink)
        if(excess(i, Glist, Gmatrix, flow) > 0) return i;
    }
    return  -1;
}

void relabel(long node, vector<vector<long>> &Glist, vector<vector<long>> &Gmatrix, vector<vector<long>> &flow, vector<long> &height){
    // Find the adjacent with minimum height
    long mh = LONG_MAX;
    for(long i = 0; i < Glist[node].size(); i++) {
        long trg = Glist[node][i];
        if(height[trg] < mh && residual(node, trg, Gmatrix, flow) > 0){
            mh = height[trg];
        }
    }
    height[node] = mh+1;
}

bool push(long node, vector<vector<long>> &Glist, vector<vector<long>> &Gmatrix, vector<vector<long>> &flow, vector<long> &height){
    // Traverse through all edges to find an adjacent (of u)
    // to which flow can be pushed
    for(long i = 0; i < Glist[node].size(); i++) {
        long trg = Glist[node][i];
        // flow is possible if: there are some residual left && target height is smaller
        if(height[node] > height[trg]){
            long res = residual(node, trg, Gmatrix, flow);
            if(res > 0){
                long flw = min(excess(node, Glist, Gmatrix, flow), res);
                augmentFlow(node, trg, flw, Gmatrix, flow);
                return true;
            }
        }
    }
    return false;
}

tuple<vector<vector<long>>, vector<long>> preflow(vector<vector<long>> &Glist, vector<vector<long>> &Gmatrix){
    long N = Glist.size();
    long s = 0;
    long t = N-1;

    // init height and excess with 0s
    vector<vector<long>> flow(N);
    vector<long> height(N);

    // init height of source
    height[s] = N;

    // init flow with 0s
    for(long i = 0; i < N; i++){
        vector<long> v(N);
        flow[i] = v;
    }

    // push the first flow
    for(long i = 0; i < Glist[s].size(); i++){
        long trg = Glist[s][i];
        augmentFlow(s, trg, Gmatrix[s][trg], Gmatrix, flow);
    }

    tuple<vector<vector<long>>, vector<long>> fh(flow, height);
    return fh;
}


// push relabel
long prMaxFlow(vector<vector<long>> &Glist, vector<vector<long>> &Gmatrix){
    long N = Glist.size();
    long s = 0;
    long t = N-1;

    vector<vector<long>> flow;
    vector<long> height;
    tuple<vector<vector<long>>, vector<long>> fh = preflow(Glist, Gmatrix);
    tie(flow, height) = fh;

    long node = overflowNode(Glist, Gmatrix, flow);
    while(node != -1){
        if(!push(node, Glist, Gmatrix, flow, height)) {
            relabel(node, Glist, Gmatrix, flow, height);
        }
        node = overflowNode(Glist, Gmatrix, flow);
    }

    long maxflow = excess(N-1, Glist, Gmatrix, flow);
    return maxflow;
}


vector<long> solveProblem(vector<Case> &cases){
    vector<long> answers(cases.size());
    for(long i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}


long solveCase(Case &thisCase){
    vector<vector<long>> Glist = constructGlist(thisCase);
    vector<vector<long>> Gmatrix;
    long directCap;
    tuple<vector<vector<long>>, long> t = constructGmatrix(thisCase);
    tie(Gmatrix, directCap) = t;

    long maxflow = prMaxFlow(Glist, Gmatrix);

    return maxflow + directCap;
}


// readers
vector<Case> readInput(){
    // read # of cases
    long t;
    scanf("%li", &t);

    // read each case
    vector<Case> cases(t);
    for(long i = 0; i < t; i++){
        Case thisCase;
        scanf("%li", &thisCase.N);
        scanf("%li", &thisCase.K);
        scanf("%li", &thisCase.M);
        scanf("%li", &thisCase.L);

        thisCase.v = new long[thisCase.L];
        thisCase.w = new long[thisCase.L];
        thisCase.c = new long[thisCase.L];

        for(long j = 0; j < thisCase.L; j++){
            scanf("%li", &thisCase.v[j]);
            scanf("%li", &thisCase.w[j]);
            scanf("%li", &thisCase.c[j]);
        }

        cases[i] = thisCase;
    }

    return cases;
}

vector<Case> readInputFile(){
    FILE* file;
    file = fopen (inFileName.c_str(),"r");

    // read # of cases
    long t;
    fscanf(file, "%li", &t);

    // read each case
    vector<Case> cases(t);
    for(long i = 0; i < t; i++){
        Case thisCase;
        fscanf(file, "%li", &thisCase.N);
        fscanf(file, "%li", &thisCase.K);
        fscanf(file, "%li", &thisCase.M);
        fscanf(file, "%li", &thisCase.L);

        thisCase.v = new long[thisCase.L];
        thisCase.w = new long[thisCase.L];
        thisCase.c = new long[thisCase.L];

        for(long j = 0; j < thisCase.L; j++){
            fscanf(file, "%li", &thisCase.v[j]);
            fscanf(file, "%li", &thisCase.w[j]);
            fscanf(file, "%li", &thisCase.c[j]);
        }

        cases[i] = thisCase;
    }

    fclose (file);
    return cases;
}


// writers
void writeOutput(vector<long> &answers){
    for(long i = 0; i < answers.size(); i++){
        printf("Case #%li: %li\n", i+1, answers[i]);
    }
}

void writeOutputFile(vector<long> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(long i = 0; i < answers.size(); i++){
        fprintf(file, "Case #%li: %li\n", i+1, answers[i]);
    }

    fclose(file);
}
