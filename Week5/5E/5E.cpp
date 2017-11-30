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
long rowcolday2index(long N, long row, long col, long day, long distilled){
    long local = row + col * N;
    if(!day) return local;
    return local + ((day * 2 - 1) + distilled) * N * N;
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

vector<vector<long>> constructGlist(Case &thisCase){
    long N = thisCase.N * thisCase.N * (thisCase.D * 2 + 1) + 2;
    vector<long> indegrees(N);
    vector<vector<long>> Glist(N);

    // layer: source --> day0
    for(long i = 0; i < thisCase.K; i++){
        long row = thisCase.rrow[i];
        long col = thisCase.rcol[i];
        long rider = rowcolday2index(thisCase.N, row, col, 0, 0)+1;
        Glist[0].push_back(rider);
        Glist[rider].push_back(0); // reverse for dinics
        indegrees[rider]++;
    }

    // layers: day0 --> day1, day1 --> day1d, day1d --> day2, ...
    for(long i = 0; i <= thisCase.D; i++){ // for each day/night
        for(long l = 0; l < 2; l++){ // for raw and distilled
            if(i == thisCase.D && l == 1) continue;
            if(i == 0) l++;
            for(long j = 0; j < thisCase.N; j++){  // for each row
                for(long k = 0; k < thisCase.N; k++){ // for each col
                    long day = i;
                    long row = j;
                    long col = k;
                    long distill = l;
                    long src = rowcolday2index(thisCase.N, row, col, day, distill)+1;
                    if(indegrees[src] && !frozen(row, col, day, thisCase)){ // if it's possible to reach here and it's not frozen

                        long trg;
                        if(distill){ // distilled, check 5 actions on the next grid
                            // stay put
                            if(!frozen(row, col, day+1, thisCase)){
                                trg = rowcolday2index(thisCase.N, row, col, day+1, 0)+1;
                                Glist[src].push_back(trg);
                                Glist[trg].push_back(src); // reverse for dinics
                                indegrees[trg]++;
                            }
                            // go up
                            if(!frozen(row-1, col, day+1, thisCase)){
                                trg = rowcolday2index(thisCase.N, row-1, col, day+1, 0)+1;
                                Glist[src].push_back(trg);
                                Glist[trg].push_back(src); // reverse for dinics
                                indegrees[trg]++;
                            }

                            // go down
                            if(!frozen(row+1, col, day+1, thisCase)){
                                trg = rowcolday2index(thisCase.N, row+1, col, day+1, 0)+1;
                                Glist[src].push_back(trg);
                                Glist[trg].push_back(src); // reverse for dinics
                                indegrees[trg]++;
                            }

                            // go left
                            if(!frozen(row, col-1, day+1, thisCase)){
                                trg = rowcolday2index(thisCase.N, row, col-1, day+1, 0)+1;
                                Glist[src].push_back(trg);
                                Glist[trg].push_back(src); // reverse for dinics
                                indegrees[trg]++;
                            }

                            // go right
                            if(!frozen(row, col+1, day+1, thisCase)){
                                trg = rowcolday2index(thisCase.N, row, col+1, day+1, 0)+1;
                                Glist[src].push_back(trg);
                                Glist[trg].push_back(src); // reverse for dinics
                                indegrees[trg]++;
                            }
                        }else{ // not distilled yet, only stay put
                            // stay put
                            trg = rowcolday2index(thisCase.N, row, col, day, 1)+1;
                            Glist[src].push_back(trg);
                            Glist[trg].push_back(src); // reverse for dinics
                            indegrees[trg]++;
                        }
                    }
                }
            }
        }
    }

    // layer: dayDd --> sink
    for(long i = 0; i < thisCase.N; i++){
        for(long j = 0; j < thisCase.N; j++){
            long src = rowcolday2index(thisCase.N, i, j, thisCase.D, 1)+1;
            if(indegrees[src] && !frozen(i, j, thisCase.D, thisCase)){
                Glist[src].push_back(N-1);
                Glist[N-1].push_back(src); // reverse for dinics
                indegrees[N-1]++;
            }
        }
    }

    return Glist;


}

vector<vector<long>> constructGmatrix(Case &thisCase){
    long N = thisCase.N * thisCase.N * (thisCase.D * 2 + 1) + 2;


    // init with 0s
    vector<long> indegrees(N);
    vector<vector<long>> Gmatrix(N);
    for(long i = 0; i < N; i++){
        vector<long> v(N);
        Gmatrix[i] = v;
    }

    // layer: source --> day0
    for(long i = 0; i < thisCase.K; i++){
        long row = thisCase.rrow[i];
        long col = thisCase.rcol[i];
        long rider = rowcolday2index(thisCase.N, row, col, 0, 0)+1;
        Gmatrix[0][rider] = 1;
        indegrees[rider]++;
    }

    // layers: day0 --> day1, day1 --> day1d, day1d --> day2, ...
    for(long i = 0; i <= thisCase.D; i++){ // for each day/night
        for(long l = 0; l < 2; l++){ // for raw and distilled
            if(i == thisCase.D && l == 1) continue;
            if(i == 0) l++;
            for(long j = 0; j < thisCase.N; j++){  // for each row
                for(long k = 0; k < thisCase.N; k++){ // for each col
                    long day = i;
                    long row = j;
                    long col = k;
                    long distill = l;
                    long src = rowcolday2index(thisCase.N, row, col, day, distill)+1;
                    if(indegrees[src] && !frozen(row, col, day, thisCase)){ // if it's possible to reach here and it's not frozen

                        long trg;
                        if(distill){ // distilled, check 5 actions on the next grid
                            // stay put
                            if(!frozen(row, col, day+1, thisCase)){
                                trg = rowcolday2index(thisCase.N, row, col, day+1, 0)+1;
                                Gmatrix[src][trg] = 1;
                                indegrees[trg]++;
                            }
                            // go up
                            if(!frozen(row-1, col, day+1, thisCase)){
                                trg = rowcolday2index(thisCase.N, row-1, col, day+1, 0)+1;
                                Gmatrix[src][trg] = 1;
                                indegrees[trg]++;
                            }

                            // go down
                            if(!frozen(row+1, col, day+1, thisCase)){
                                trg = rowcolday2index(thisCase.N, row+1, col, day+1, 0)+1;
                                Gmatrix[src][trg] = 1;
                                indegrees[trg]++;
                            }

                            // go left
                            if(!frozen(row, col-1, day+1, thisCase)){
                                trg = rowcolday2index(thisCase.N, row, col-1, day+1, 0)+1;
                                Gmatrix[src][trg] = 1;
                                indegrees[trg]++;
                            }

                            // go right
                            if(!frozen(row, col+1, day+1, thisCase)){
                                trg = rowcolday2index(thisCase.N, row, col+1, day+1, 0)+1;
                                Gmatrix[src][trg] = 1;
                                indegrees[trg]++;
                            }
                        }else{ // not distilled yet, only stay put
                            // stay put
                            trg = rowcolday2index(thisCase.N, row, col, day, 1)+1;
                            Gmatrix[src][trg] = 1;
                            indegrees[trg]++;
                        }
                    }
                }
            }
        }
    }

    // layer: dayDd --> sink
    for(long i = 0; i < thisCase.N; i++){
        for(long j = 0; j < thisCase.N; j++){
            long src = rowcolday2index(thisCase.N, i, j, thisCase.D, 1)+1;
            if(indegrees[src] && !frozen(i, j, thisCase.D, thisCase)){
                Gmatrix[src][N-1] = 1;
                indegrees[N-1]++;
            }
        }
    }

    return Gmatrix;
}



// maxflow utilities
long residual(vector<vector<long>> &Glist, vector<vector<long>> &Gmatrix, vector<vector<long>> &flow, long src, long trg){
    long r = 0;

    // an edge exists src --> trg
    if(Gmatrix[src][trg] != 0) r += Gmatrix[src][trg] - flow[src][trg];

    // an edge exists trg --> src
    if(Gmatrix[trg][src] != 0) r += flow[trg][src];

    return r;
}

void augmentFlow(long src, long trg, long flw, vector<vector<long>> &Gmatrix, vector<vector<long>> &flow){

    if(Gmatrix[trg][src] != 0){
        long flowbackward = max<long>(flow[trg][src] - flw, 0);
        long leftflow = max<long>(flw - flow[trg][src], 0);
        long flowforward = min(Gmatrix[src][trg] ,flow[src][trg] + leftflow);

        flow[src][trg] = flowforward;
        flow[trg][src] = flowbackward;
    }else{
        long flowforward = min(Gmatrix[src][trg] ,flow[src][trg] + flw);
        flow[src][trg] = flowforward;
    }
}



// dinics utilities
vector<long> computeLevels(vector<vector<long>> &Glist, vector<vector<long>> &Gmatrix, vector<vector<long>> &flow){
    long N = Glist.size();
    long s = 0;

    // init bfs
    vector<long> levels(N);
    for(long i = 0; i < N; i++) levels[i] = -1;
    vector<bool> visited(N);
    deque<long> queue;

    // push s
    long node = s;
    visited[node] = true;
    levels[node] = 0;
    queue.push_back(node);

    while(!queue.empty()){
        long node = queue.front();
        queue.pop_front();

        // visit neighbours of node
        for(long i = 0; i < Glist[node].size(); i++){
            long trg = Glist[node][i];
            long res = residual(Glist, Gmatrix, flow, node, trg);
            if(res > 0 && !visited[trg]){
                levels[trg] = levels[node] + 1;
                visited[trg] = true;
                queue.push_back(trg);
            }
        }
    }
    return levels;
}

long sendFlow(long node, long flw, long trg, vector<vector<long>> &Glist, vector<vector<long>> &Gmatrix, vector<vector<long>> &flow, vector<long> &levels){

    if(node == trg) return flw;

    for(long i = 0; i < Glist[node].size(); i++){
        long next = Glist[node][i];
        long res = residual(Glist,Gmatrix, flow, node, next);
        if(res > 0 && levels[node] + 1 == levels[next]){
            long current_flw = min(flw, res);

            long temp_flw = sendFlow(next, current_flw, trg, Glist, Gmatrix, flow, levels);

            if(temp_flw > 0){
                augmentFlow(node, next, temp_flw, Gmatrix, flow);
                return temp_flw;
            }
        }
    }

    return 0;
}



// dinics
long dinicsMaxFlow(vector<vector<long>> &Glist, vector<vector<long>> &Gmatrix){
    long N = Glist.size();
    long s = 0;
    long t = N-1;

    // init all flows with 0s
    vector<vector<long>> flow(N);
    for(long i = 0; i < N; i++){
        vector<long> v(N);
        flow[i] = v;
    }

    // compute levels
    vector<long> levels = computeLevels(Glist, Gmatrix, flow);
    long maxflow = 0;

    // while a blocking flow on lg exists, compute it and augment to original flow
    while(levels[t] != -1){
        while(long flw = sendFlow(s, LONG_MAX, t, Glist, Gmatrix, flow, levels)){
            maxflow += flw;
        }
        levels = computeLevels(Glist, Gmatrix, flow);
    }

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
    vector<vector<long>> Gmatrix = constructGmatrix(thisCase);

    if(!thisCase.K) return 0;
    long maxflow = dinicsMaxFlow(Glist, Gmatrix);
    return maxflow;
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
