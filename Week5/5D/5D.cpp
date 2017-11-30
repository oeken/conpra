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
    int W; // #warriors
    int M; // #mages
    int A; // #artifacts
    int S; // #spells

    int*  w_b; // warrior: capacity
    int*  w_c; // warrior: receiveable spell count
    int** w_s; // warrior: receiveable spells

    int*  m_d; // mage: castable spell count
    int** m_t; // mage: castable spells

    int* a_e; // artifact: available count
    int* a_t; // artifact: type (0 normal, 1 epic)

    int*  m_f; // mage: available artifact count
    int** m_a; // mage: available artifacts
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


// graph constructors
vector<vector<int>> constructGlist(Case &thisCase){
    int N = thisCase.A + thisCase.M + thisCase.S + thisCase.W * 2 + 2;
    vector<vector<int>> Glist(N);

    // 0 layer "Sa": source --> artifact
    for(int i = 1; i <= thisCase.A; i++){
        int amount = 0;
        if(thisCase.a_t[i-1]){ // this is epic
            amount = thisCase.a_e[i-1];
        }else{ // this is normal
            amount = thisCase.a_e[i-1] / 3;
        }
        if(amount){ // i.e. add an edge if more than 0 available
            Glist[0].push_back(i);
            Glist[i].push_back(0); // reverse direction with 0 (for dinics)
            // todo: we are not doing this to INF edges, check otherwise if not working
        }
    }

    // 1 layer "am": artifact --> mage
    for(int i = 0; i < thisCase.M; i++){
        int mage = i+1+thisCase.A;
        for(int j = 0; j < thisCase.m_f[i]; j++){
            int artifact = thisCase.m_a[i][j];
            Glist[artifact].push_back(mage);
            Glist[mage].push_back(artifact); // reverse direction with 0 (for dinics)
            // todo: we are not doing this to INF edges, check otherwise if not working
        }
    }

    // 2 layer "ms": mage --> spell
    for(int i = 0; i < thisCase.M; i++){
        int mage = i+1+thisCase.A;
        for(int j = 0; j < thisCase.m_d[i]; j++){
            int spell = thisCase.A + thisCase.M + thisCase.m_t[i][j];
            Glist[mage].push_back(spell);
            Glist[spell].push_back(mage); // reverse direction with 0 (for dinics)
        }
    }

    // 3 layer "sw1": spell --> warrior1
    for(int i = 0; i < thisCase.W; i++){
        int warrior1 = i+1+thisCase.A+thisCase.M+thisCase.S;
        for(int j = 0; j < thisCase.w_c[i]; j++){
            int spell = thisCase.A + thisCase.M + thisCase.w_s[i][j];
            Glist[spell].push_back(warrior1);
            Glist[warrior1].push_back(spell); // reverse direction with 0 (for dinics)
        }
    }

    // 4 layer "w1w2": warrior1 --> warrior2
    for(int i = 0; i < thisCase.W; i++){
        int warrior1 = i+1+thisCase.A+thisCase.M+thisCase.S;
        int warrior2 = warrior1 + thisCase.W;
        Glist[warrior1].push_back(warrior2);
        Glist[warrior2].push_back(warrior1); // reverse direction with 0 (for dinics)
    }

    // 5 layer "w2T": warrior2 --> sink
    for(int i = 0; i < thisCase.W; i++){
        int warrior2 = i+1+thisCase.A+thisCase.M+thisCase.S+thisCase.W;
        Glist[warrior2].push_back(N-1);
        Glist[N-1].push_back(warrior2); // reverse direction with 0 (for dinics)
    }

    return Glist;
}

vector<vector<int>> constructGmatrix(Case &thisCase){
    int N = thisCase.A + thisCase.M + thisCase.S + thisCase.W * 2 + 2;

    vector<vector<int>> Gmatrix(N); // init with 0s
    for(int i = 0; i < N; i++){ vector<int> v(N); Gmatrix[i] = v; }

    // 0 layer "Sa": source --> artifact
    for(int i = 1; i <= thisCase.A; i++){
        if(thisCase.a_t[i-1]){ // this is an epic artifact
            Gmatrix[0][i] = thisCase.a_e[i-1];
        }else{ // this is a normal artifact
            Gmatrix[0][i] = thisCase.a_e[i-1] / 3;
        }
    }

    // 1 layer "am": artifact --> mage
    for(int i = 0; i < thisCase.M; i++){
        int mage = i+1+thisCase.A;
        for(int j = 0; j < thisCase.m_f[i]; j++){
            int artifact = thisCase.m_a[i][j];
            Gmatrix[artifact][mage] = INT_MAX;
        }
    }

    // 2 layer "ms": mage --> spell
    for(int i = 0; i < thisCase.M; i++){
        int mage = i+1+thisCase.A;
        for(int j = 0; j < thisCase.m_d[i]; j++){
            int spell = thisCase.A + thisCase.M + thisCase.m_t[i][j];
            Gmatrix[mage][spell] = INT_MAX;
        }
    }

    // 3 layer "sw1": spell --> warrior1
    for(int i = 0; i < thisCase.W; i++){
        int warrior1 = i+1+thisCase.A+thisCase.M+thisCase.S;
        for(int j = 0; j < thisCase.w_c[i]; j++){
            int spell = thisCase.A + thisCase.M + thisCase.w_s[i][j];
            Gmatrix[spell][warrior1] = INT_MAX;
        }
    }

    // 4 layer "w1w2": warrior1 --> warrior2
    for(int i = 0; i < thisCase.W; i++){
        int warrior1 = i+1+thisCase.A+thisCase.M+thisCase.S;
        int warrior2 = warrior1 + thisCase.W;
        Gmatrix[warrior1][warrior2] = thisCase.w_b[i];
    }

    // 5 layer "w2T": warrior2 --> sink
    for(int i = 0; i < thisCase.W; i++){
        int warrior2 = i+1+thisCase.A+thisCase.M+thisCase.S+thisCase.W;
        Gmatrix[warrior2][N-1] = INT_MAX;
    }

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
    vector<vector<int>> Glist = constructGlist(thisCase);
    vector<vector<int>> Gmatrix = constructGmatrix(thisCase);

    int maxflow = dinicsMaxFlow(Glist, Gmatrix);
    return maxflow;
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
        scanf("%d", &thisCase.W); // warriors
        scanf("%d", &thisCase.M); // mages
        scanf("%d", &thisCase.A); // artifacts
        scanf("%d", &thisCase.S); // spells

        thisCase.w_b = new int[thisCase.W];
        thisCase.w_c = new int[thisCase.W];
        thisCase.w_s = new int*[thisCase.W];

        thisCase.m_d = new int[thisCase.M];
        thisCase.m_t = new int*[thisCase.M];

        thisCase.a_e = new int[thisCase.A];
        thisCase.a_t = new int[thisCase.A];

        thisCase.m_f = new int[thisCase.M];
        thisCase.m_a = new int*[thisCase.M];

        for(int j = 0; j < thisCase.W; j++){
            scanf("%d", &thisCase.w_b[j]);
            scanf("%d", &thisCase.w_c[j]);

            thisCase.w_s[j] = new int[thisCase.w_c[j]];
            for(int k = 0; k < thisCase.w_c[j]; k++){ scanf("%d", &thisCase.w_s[j][k]); }
        }

        for(int j = 0; j < thisCase.M; j++){
            scanf("%d", &thisCase.m_d[j]);

            thisCase.m_t[j] = new int[thisCase.m_d[j]];
            for(int k = 0; k < thisCase.m_d[j]; k++){ scanf("%d", &thisCase.m_t[j][k]); }
        }

        for(int j = 0; j < thisCase.A; j++){
            scanf("%d", &thisCase.a_e[j]);

            char* c = new char[10];
            scanf("%s", c);
            string s(c);

            if(s == "epic"){
                thisCase.a_t[j] = 1;
            }else{
                thisCase.a_t[j] = 0;
            }
        }

        for(int j = 0; j < thisCase.M; j++){
            scanf("%d", &thisCase.m_f[j]);

            thisCase.m_a[j] = new int[thisCase.m_f[j]];
            for(int k = 0; k < thisCase.m_f[j]; k++){ scanf("%d", &thisCase.m_a[j][k]); }
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
        fscanf(file, "%d", &thisCase.W); // warriors
        fscanf(file, "%d", &thisCase.M); // mages
        fscanf(file, "%d", &thisCase.A); // artifacts
        fscanf(file, "%d", &thisCase.S); // spells

        thisCase.w_b = new int[thisCase.W];
        thisCase.w_c = new int[thisCase.W];
        thisCase.w_s = new int*[thisCase.W];

        thisCase.m_d = new int[thisCase.M];
        thisCase.m_t = new int*[thisCase.M];

        thisCase.a_e = new int[thisCase.A];
        thisCase.a_t = new int[thisCase.A];

        thisCase.m_f = new int[thisCase.M];
        thisCase.m_a = new int*[thisCase.M];

        for(int j = 0; j < thisCase.W; j++){
            fscanf(file, "%d", &thisCase.w_b[j]);
            fscanf(file, "%d", &thisCase.w_c[j]);

            thisCase.w_s[j] = new int[thisCase.w_c[j]];
            for(int k = 0; k < thisCase.w_c[j]; k++){ fscanf(file, "%d", &thisCase.w_s[j][k]); }
        }

        for(int j = 0; j < thisCase.M; j++){
            fscanf(file, "%d", &thisCase.m_d[j]);

            thisCase.m_t[j] = new int[thisCase.m_d[j]];
            for(int k = 0; k < thisCase.m_d[j]; k++){ fscanf(file, "%d", &thisCase.m_t[j][k]); }
        }

        for(int j = 0; j < thisCase.A; j++){
            fscanf(file, "%d", &thisCase.a_e[j]);

            char* c = new char[10];
            fscanf(file, "%s", c);
            string s(c);

            if(s == "epic"){
                thisCase.a_t[j] = 1;
            }else{
                thisCase.a_t[j] = 0;
            }
        }

        for(int j = 0; j < thisCase.M; j++){
            fscanf(file, "%d", &thisCase.m_f[j]);

            thisCase.m_a[j] = new int[thisCase.m_f[j]];
            for(int k = 0; k < thisCase.m_f[j]; k++){ fscanf(file, "%d", &thisCase.m_a[j][k]); }
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



//cout << cases[1].W << endl;
//cout << cases[1].M << endl;
//cout << cases[1].A << endl;
//cout << cases[1].S << endl << endl;
//
//cout << cases[1].w_b[0] << endl;
//cout << cases[1].w_c[0] << endl;
//cout << cases[1].w_s[0][0] << endl << endl;
//
//cout << cases[1].m_d[0] << endl;
//cout << cases[1].m_t[0][0] << endl << endl;
//
//cout << cases[1].a_e[0] << endl;
//cout << cases[1].a_t[0] << endl << endl;
//
//cout << cases[1].m_f[0] << endl;
//cout << cases[1].m_a[0][0] << endl << endl;