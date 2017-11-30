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
    long N; // #friends
    long M; // #presents
    char** p; // preferences
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
void processToken(string &s, vector<long> &ts){
    long pos = 0;
    string dash("-");

    if((pos = s.find(dash)) != string::npos) { // yes dash
        string left = s.substr(0, pos);
        string right = s.substr(pos + 1, s.size());
        long lnum = stoi(left);
        long rnum = stoi(right);
        for (long j = lnum; j <= rnum; j++) ts.push_back(j);
    }else{ // no dash
        ts.push_back(stoi(s));
    }
}

vector<vector<long>> constructTargets(Case &thisCase){
    vector<vector<long>> targets(thisCase.N);
    string comma(",");
    string dash("-");
    long pos = 0;

    for(long i = 0; i < thisCase.N; i++) {
        vector<long> ts;
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
//            for(long j = 1; j <= thisCase.M; j++) ts.push_back(j);
//        }

        targets[i] = ts;
    }
    return targets;
}

vector<vector<long>> constructGlist(Case &thisCase, vector<vector<long>> &targets){
    long N = thisCase.N + thisCase.M + 2;
    vector<vector<long>> Glist(N);

    for(long i = 0; i < thisCase.N; i++){
        long src = i+1;
        for(long j = 0; j < targets[i].size(); j++){
            long trg = thisCase.N + targets[i][j];
            Glist[src].push_back(trg); // directed graph - but push the reverse direction any way with value 0
            Glist[trg].push_back(src); // todo: may not work
        }
    }

    for(long i = 1; i <= thisCase.N; i++) { // source to friends
        Glist[0].push_back(i);
        Glist[i].push_back(0);
    }

    for(long i = thisCase.N+1; i <= thisCase.N+thisCase.M; i++){ // presents to sink
        Glist[i].push_back(N-1);
        Glist[N-1].push_back(i);
    }

    return Glist;
}

vector<vector<long>> constructGmatrix(Case &thisCase, vector<vector<long>> &targets){
    long N = thisCase.N + thisCase.M + 2;
    vector<vector<long>> Gmatrix(N);
    for(long i = 0; i < N; i++){
        vector<long> v(N);
        Gmatrix[i] = v;
    }

    for(long i = 0; i < thisCase.N; i++){
        long src = i+1;
        for(long j = 0; j < targets[i].size(); j++){
            long trg = thisCase.N + targets[i][j];
            Gmatrix[src][trg] = 1; // directed graph
        }
    }

    for(long i = 1; i <= thisCase.N; i++) Gmatrix[0][i] = 1; // source to friends
    for(long i = thisCase.N+1; i <= thisCase.N+thisCase.M; i++) Gmatrix[i][N-1] = 1; // presents to sink

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
    if(thisCase.N > thisCase.M) return 0;

    vector<vector<long>> targets = constructTargets(thisCase);
    for(long i = 0; i < thisCase.N; i++) if(!targets[i].size()) return 0;

    vector<vector<long>> Glist = constructGlist(thisCase, targets);
    vector<vector<long>> Gmatrix = constructGmatrix(thisCase, targets);

    long maxflow = prMaxFlow(Glist, Gmatrix);

    if(maxflow == thisCase.N) { return 1; }
    else{ return 0;}

}


// readers
vector<Case> readInput(){
    // read # of cases
    char dummy;
    long t;
    scanf("%li", &t);

    // read each case
    vector<Case> cases(t);
    for(long i = 0; i < t; i++){
        Case thisCase;
        scanf("%li", &thisCase.N);
        scanf("%li", &thisCase.M);
        scanf("%c", &dummy); // skip \n

        thisCase.p = new char*[thisCase.N];

        for(long j = 0; j < thisCase.N; j++){
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
    long t;
    fscanf(file, "%li", &t);

    // read each case
    vector<Case> cases(t);
    for(long i = 0; i < t; i++){
        Case thisCase;
        fscanf(file, "%li", &thisCase.N);
        fscanf(file, "%li", &thisCase.M);
        fscanf(file, "%c", &dummy); // skip \n

        thisCase.p = new char*[thisCase.N];

        for(long j = 0; j < thisCase.N; j++){
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

