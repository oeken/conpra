#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <climits>
#include <queue>
#include <ctime>
#include <tuple>
#include <unordered_map>

using namespace std;

struct Case {
    long N;      // grid size
    long K;      // #riders
    long D;      // #nights
    long** grid; // landscape
    long* rrow;  // rider rows
    long* rcol;  // rider cols
    long* level; // levels through nights
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


// graph constructors
long rowcolday2index_new(long N, long row, long col, long day){
    long local = row + col * N;
    return local + day * (N*N) + 1;
}

//long rowcolday2index(long N, long row, long col, long day, long distilled){
//    long local = row + col * N;
//    if(!day) return local+1;
//    return (local + ((day * 2 - 1) + distilled) * N * N)+1;
//}

//vector<long> index2rowcolday(long N, long index){
//    long day = (index / (N * N) + 1) / 2;
//    long distilled = (index / (N * N) + 1) % 2;
//    long local = index % (N*N);
//    long col = local / N;
//    long row = local % N;
//    vector<long> v({row, col, day, distilled});
//    return v;
//}

long addByIndex(long src, long trg, long N, vector<vector<long>> &edges, unordered_map<long,long> &index2id, unordered_map<long,long> &id2index, unordered_map<long,bool> &active){
    long srcid = index2id[src];
    if (index2id.find(trg) != index2id.end()){ // existing one / already given an id
        long trgid = index2id[trg];
        vector<long> v({srcid, trgid});
        edges.push_back(v);
    }else { // new one
        vector<long> v({srcid, N});
        edges.push_back(v);
        index2id[trg] = N;
        id2index[N] = trg;
        N++;
    }
    active[trg] = true;
    return N;
}

bool frozen(long row, long col, long day, Case &thisCase){
    bool ingrid = row >= 0 && row <= thisCase.N-1 && col >= 0 && col <= thisCase.N-1;
    if(!ingrid) return true;
    else{
        if(day == 0) return false;
        else if(day >= 1 && day <= thisCase.D) return thisCase.grid[row][col] <= thisCase.level[day-1];
        else return true;
    }
}

tuple<vector<vector<long>>, long, unordered_map<long, long>> explore(Case &thisCase, int day1, int day2, unordered_map<long, bool> &active){
    vector<vector<long>> edges;
    unordered_map<long, long> index2id;
    unordered_map<long, long> id2index;


    long N = 1;
    // source --> day1
    for(int j = 0; j < thisCase.N; j++){
        for(int i = 0; i < thisCase.N; i++){
            long trg = rowcolday2index_new(thisCase.N, i, j, day1);
            if(active.find(trg) != active.end()) { // only if it's found before
                N = addByIndex(0, trg, N, edges, index2id, id2index, active);
            }
        }
    }

    // day1 --> day2
    for(int j = 0; j < thisCase.N; j++){
        for(int i = 0; i < thisCase.N; i++){
            long src = rowcolday2index_new(thisCase.N, i, j, day1);
            if(active.find(src) != active.end()) { // only if it's found before
                // go left
                if(!frozen(i, j-1, day2, thisCase)){
                    long trg = rowcolday2index_new(thisCase.N, i, j-1, day2);
                    N = addByIndex(src, trg, N, edges, index2id, id2index, active);
                }

                // go up
                if(!frozen(i-1, j, day2, thisCase)){
                    long trg = rowcolday2index_new(thisCase.N, i-1, j, day2);
                    N = addByIndex(src, trg, N, edges, index2id, id2index, active);
                }

                // stay put
                if(!frozen(i, j, day2, thisCase)){
                    long trg = rowcolday2index_new(thisCase.N, i, j, day2);
                    N = addByIndex(src, trg, N, edges, index2id, id2index, active);
                }

                // go down
                if(!frozen(i+1, j, day2, thisCase)){
                    long trg = rowcolday2index_new(thisCase.N, i+1, j, day2);
                    N = addByIndex(src, trg, N, edges, index2id, id2index, active);
                }

                // go right
                if(!frozen(i, j+1, day2, thisCase)){
                    long trg = rowcolday2index_new(thisCase.N, i, j+1, day2);
                    N = addByIndex(src, trg, N, edges, index2id, id2index, active);
                }
            }
        }
    }


    // day2 --> sink
    for(int j = 0; j < thisCase.N; j++){
        for(int i = 0; i < thisCase.N; i++){
            long src = rowcolday2index_new(thisCase.N, i, j, day2);
            if(active.find(src) != active.end()) { // only if it's found before
                N = addByIndex(src, thisCase.N*thisCase.N*(thisCase.D+1)+1, N, edges, index2id, id2index, active);
            }
        }
    }

    tuple<vector<vector<long>>, long, unordered_map<long, long>> tup(edges, N, id2index);
    return tup;
}

vector<vector<long>> constructGlist(long N, vector<vector<long>> &edges){
    vector<vector<long>> Glist(N);
    for(long i = 0; i < edges.size(); i++){
        Glist[edges[i][0]].push_back(edges[i][1]);
        Glist[edges[i][1]].push_back(edges[i][0]);
    }
    return Glist;
}

vector<vector<long>> constructGmatrix(long N, vector<vector<long>> &edges){
    vector<vector<long>> Gmatrix(N);
    for(long i = 0; i < N; i++){
        vector<long> v(N);
        Gmatrix[i] = v;
    }

    for(long i = 0; i < edges.size(); i++){
        Gmatrix[edges[i][0]][edges[i][1]] = 1;
    }

    return Gmatrix;
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
tuple<vector<vector<long>>, long> prMaxFlow(vector<vector<long>> &Glist, vector<vector<long>> &Gmatrix, unordered_map<long, bool> &active){
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
    tuple<vector<vector<long>>, long> tup(flow, maxflow);
    return tup;
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
    unordered_map<long, bool> active;
    if(!thisCase.K) return 0;

    for(int i = 0; i < thisCase.K; i++){
        long row = thisCase.rrow[i];
        long col = thisCase.rcol[i];
        long day = 0;
        long index = rowcolday2index_new(thisCase.N, row, col, day);
        active[index] = true;
    }

    long totalmaxflow = LONG_MAX;
    for(int i = 0; i < thisCase.D; i++){
        vector<vector<long>> edges;
        long N;
        unordered_map<long, long> id2index;
        tuple<vector<vector<long>>, long, unordered_map<long, long>> tup1 = explore(thisCase, i, i+1, active);
        tie(edges, N, id2index) = tup1;

        vector<vector<long>> Glist = constructGlist(N, edges);
        vector<vector<long>> Gmatrix= constructGmatrix(N, edges);

        vector<vector<long>> flowall;
        long maxflow;
        tuple<vector<vector<long>>, long> tup2 = prMaxFlow(Glist, Gmatrix, active);
        tie(flowall, maxflow) = tup2;

        for(int i = 0; i < Glist[N-1].size(); i++){
            long trgid = Glist[N-1][i];
            long trg = id2index[trgid];
            if(flowall[trgid][N-1] > 0){
                active[trg] = true;
            }else{
                active[trg] = false;
            }
        }

        totalmaxflow = min<long>(totalmaxflow, maxflow);
        if(totalmaxflow == 0) break;
    }
    return totalmaxflow;
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
        scanf("%li", &thisCase.D);

        thisCase.grid = new long*[thisCase.N];
        for(long j = 0; j < thisCase.N; j++){
            thisCase.grid[j] = new long[thisCase.N];
            for(long k = 0; k < thisCase.N; k++){
                scanf("%li", &thisCase.grid[j][k]);
            }
        }

        thisCase.rrow = new long[thisCase.K];
        thisCase.rcol = new long[thisCase.K];
        for(long j = 0; j < thisCase.K; j++){
            scanf("%li", &thisCase.rrow[j]);
            scanf("%li", &thisCase.rcol[j]);
        }

        thisCase.level = new long[thisCase.D];
        for(long j = 0; j< thisCase.D; j++){
            scanf("%li", &thisCase.level[j]);
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
        fscanf(file, "%li", &thisCase.D);

        thisCase.grid = new long*[thisCase.N];
        for(long j = 0; j < thisCase.N; j++){
            thisCase.grid[j] = new long[thisCase.N];
            for(long k = 0; k < thisCase.N; k++){
                fscanf(file, "%li", &thisCase.grid[j][k]);
            }
        }

        thisCase.rrow = new long[thisCase.K];
        thisCase.rcol = new long[thisCase.K];
        for(long j = 0; j < thisCase.K; j++){
            fscanf(file, "%li", &thisCase.rrow[j]);
            fscanf(file, "%li", &thisCase.rcol[j]);
        }

        thisCase.level = new long[thisCase.D];
        for(long j = 0; j< thisCase.D; j++){
            fscanf(file, "%li", &thisCase.level[j]);
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
