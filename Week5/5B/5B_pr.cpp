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
    long L; // #policemen
    long N; // #nodes
    long M; // #links
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


// graph constrcutors
vector<vector<long>> constructGlist(Case &thisCase){
    vector<vector<long>> Glist(thisCase.N);
    for(long i = 0; i < thisCase.M; i++){
        long v = thisCase.v[i]-1;
        long w = thisCase.w[i]-1;

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

tuple<vector<vector<long>>, long, long, long> constructGmatrix(Case &thisCase){
    vector<vector<long>> Gmatrix(thisCase.N);
    for(long i = 0; i < thisCase.N; i++){
        vector<long> v(thisCase.N);
        Gmatrix[i] = v;
    }

    long directCap = 0;
    long sourceCap = 0;
    long targetCap = 0;

    // read capacities
    for(long i = 0; i < thisCase.M; i++){
        long v = thisCase.v[i]-1;
        long w = thisCase.w[i]-1;
        long c = thisCase.c[i];

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

    tuple<vector<vector<long>>, long, long, long> t(Gmatrix, directCap, sourceCap, targetCap);
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



// standard stuff
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
    long sourceCap;
    long targetCap;

    tuple<vector<vector<long>>, long, long, long> t = constructGmatrix(thisCase);
    tie(Gmatrix, directCap, sourceCap, targetCap) = t;

    if(thisCase.L >= directCap + min(sourceCap, targetCap)) return 1;

    long maxflow = prMaxFlow(Glist, Gmatrix);

    if(thisCase.L >= directCap + maxflow){
        return 1;
    } else{
        return 0;
    }
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
        scanf("%li", &thisCase.L);
        scanf("%li", &thisCase.N);
        scanf("%li", &thisCase.M);

        thisCase.v = new long[thisCase.M];
        thisCase.w = new long[thisCase.M];
        thisCase.c = new long[thisCase.M];

        for(long j = 0; j < thisCase.M; j++){
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
        fscanf(file, "%li", &thisCase.L);
        fscanf(file, "%li", &thisCase.N);
        fscanf(file, "%li", &thisCase.M);

        thisCase.v = new long[thisCase.M];
        thisCase.w = new long[thisCase.M];
        thisCase.c = new long[thisCase.M];

        for(long j = 0; j < thisCase.M; j++){
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
        if(answers[i]){
            printf("Case #%li: yes\n", i+1);
        }else{
            printf("Case #%li: no\n", i+1);
        }
    }
}

void writeOutputFile(vector<long> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(long i = 0; i < answers.size(); i++){
        if(answers[i]){
            fprintf(file, "Case #%li: yes\n", i+1);
        }else{
            fprintf(file, "Case #%li: no\n", i+1);
        }
    }

    fclose(file);
}

