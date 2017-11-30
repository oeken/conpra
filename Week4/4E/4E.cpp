#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <climits>
#include <queue>
#include <ctime>

using namespace std;

struct Case {
    int N; // nodes
    int M; // links
    int K; // teams
    int* a; // luxuries
    int* x; // flight sources
    int* y; // flight targets
    int* c; // flight costs
    int* tx; // team sources
    int* ty; // team targets
    int* ta; // team luxuries
};

// readers
vector<Case> readInput();
vector<Case> readInputFile();

// writers
void writeOutput(vector<vector<int>> &answers);
void writeOutputFile(vector<vector<int>> &answers);

// helpers
vector<vector<int>> solveProblem(vector<Case> &cases);
vector<int> solveCase(Case &thisCase);

// file names
string inFileName = "in.txt";
string outFileName = "out.txt";


int main() {
//    vector<Case> cases = readInputFile();
    vector<Case> cases = readInput();

    vector<vector<int>> answers = solveProblem(cases);

    writeOutput(answers);
//    writeOutputFile(answers);
    return 0;
}


// helpers
vector<vector<int>> constructAdjlist(Case &thisCase){
    vector<vector<int>> adjlist(thisCase.N);
    for(int i = 0; i < thisCase.M; i++){
        int src = thisCase.x[i]-1;
        int trg = thisCase.y[i]-1;

        bool check1 = src != trg; // eliminate self links
        bool check2 = find(begin(adjlist[src]), end(adjlist[src]), trg) == end(adjlist[src]); // if does not contain add it

        if(check1 && check2) adjlist[src].push_back(trg);
    }
    return adjlist;
}

vector<vector<int>> constructAdjmat(Case &thisCase){
    vector<vector<int>> adjmat(thisCase.N);
    for(int i = 0; i < thisCase.N; i++){
        vector<int> v(thisCase.N);
        for(int j = 0; j < thisCase.N; j++) v[j] = INT_MAX;
        v[i] = 0;
        adjmat[i] = v;
    }

    for(int i = 0; i < thisCase.M; i++){
        int src = thisCase.x[i]-1;
        int trg = thisCase.y[i]-1;
        int cost = thisCase.c[i];

        bool check1 = src != trg;
        bool check2 = cost < adjmat[src][trg];

        if(check1 && check2) adjmat[src][trg] = cost;
    }
    return adjmat;
}


vector<vector<int>> solveProblem(vector<Case> &cases){
    vector<vector<int>> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}


vector<int> solveCase(Case &thisCase){
    // construct stuff
    vector<vector<int>> adjlist = constructAdjlist(thisCase);
    vector<vector<int>> adjmat = constructAdjmat(thisCase);

    // fetch requests
    vector<vector<int>> request(thisCase.K);
    vector<int> request_indices_ascending(thisCase.K);
    for(int i = 0; i < thisCase.K; i++) vector<int> v({thisCase.tx[i]-1, thisCase.ty[i]-1, thisCase.ta[i], -1});
    for(int i = 0; i < thisCase.K; i++) request_indices_ascending[i] = i;
    sort(request_indices_ascending.begin(), request_indices_ascending.end(), [&](int a, int b){
        return request[a][2] < request[b][2];
    });


    int N = thisCase.N;
    // init with INF and 0s on diagoal
    vector<vector<int>> apsp(N);
    for(int i = 0; i < N; i++){
        vector<int> v1(N);
        apsp[i] = v1;
        for(int j = 0; j < N; j++){
            if(i == j){
                apsp[i][j] = 0;
            }else{
                apsp[i][j]= INT_MAX;
            }
        }
    }

    int d = 0;
    vector<int> cities;
    cities.push_back(d);
    bool enough = false;
    while(!enough){
        d++;
        enough = cities.back()[2] ==
    }

    // add direct connections
    for(int i = 0; i < N; i++){
        for(int j = 0; j < adjlist[i].size(); j++){
            int trg = adjlist[i][j];
            apsp[i][trg] = adjmat[i][trg];
        }
    }

    // exec floyd warshall
    for(int w = 0; w < N; w++){ // through node w
        for(int i = 0; i < N; i++){ // from node i
            for(int j = 0; j < N; j++){ // to node j
                bool check1 = apsp[i][w] != INT_MAX;
                bool check2 = apsp[w][j] != INT_MAX;
                if(check1 && check2 && apsp[i][w] + apsp[w][j] < apsp[i][j]){
                    apsp[i][j] = apsp[i][w] + apsp[w][j];
                }
            }
        }
    }

    bool found_new_level = false;
    int i = 0;
    while(!found_new_level){
        i++;

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
        scanf("%d", &thisCase.N);
        scanf("%d", &thisCase.M);
        scanf("%d", &thisCase.K);

        thisCase.a = new int[thisCase.N]; // luxuries

        thisCase.x = new int[thisCase.M]; // flight sources
        thisCase.y = new int[thisCase.M]; // flight targets
        thisCase.c = new int[thisCase.M]; // flight costs

        thisCase.tx = new int[thisCase.K]; // team sources
        thisCase.ty = new int[thisCase.K]; // team targets
        thisCase.ta = new int[thisCase.K]; // team luxuries

        for(int j = 0; j < thisCase.N; j++) scanf("%d", &thisCase.a[j]); // read luxuries

        for(int j = 0; j < thisCase.M; j++){
            scanf("%d", &thisCase.x[j]);
            scanf("%d", &thisCase.y[j]);
            scanf("%d", &thisCase.c[j]);
        }

        for(int j = 0; j < thisCase.K; j++){
            scanf("%d", &thisCase.tx[j]);
            scanf("%d", &thisCase.ty[j]);
            scanf("%d", &thisCase.ta[j]);
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
        fscanf(file, "%d", &thisCase.M);
        fscanf(file, "%d", &thisCase.K);

        thisCase.a = new int[thisCase.N]; // luxuries

        thisCase.x = new int[thisCase.M]; // flight sources
        thisCase.y = new int[thisCase.M]; // flight targets
        thisCase.c = new int[thisCase.M]; // flight costs

        thisCase.tx = new int[thisCase.K]; // team sources
        thisCase.ty = new int[thisCase.K]; // team targets
        thisCase.ta = new int[thisCase.K]; // team luxuries

        for(int j = 0; j < thisCase.N; j++) fscanf(file, "%d", &thisCase.a[j]); // read luxuries

        for(int j = 0; j < thisCase.M; j++){
            fscanf(file, "%d", &thisCase.x[j]);
            fscanf(file, "%d", &thisCase.y[j]);
            fscanf(file, "%d", &thisCase.c[j]);
        }

        for(int j = 0; j < thisCase.K; j++){
            fscanf(file, "%d", &thisCase.tx[j]);
            fscanf(file, "%d", &thisCase.ty[j]);
            fscanf(file, "%d", &thisCase.ta[j]);
        }

        cases[i] = thisCase;
    }

    fclose (file);
    return cases;
}


// writers
void writeOutput(vector<vector<int>> &answers){
    for(int i = 0; i < answers.size(); i++){
        printf("Case #%d:", i+1);
        for(int j = 0; j < answers[i].size(); j++){
            if(answers[i][j] == -1){
                printf(" impossible");
            }else{
                printf(" %d", answers[i][j]);
            }
        }
        printf("\n");
    }
}

void writeOutputFile(vector<vector<int>> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < answers.size(); i++){
        fprintf(file, "Case #%d:", i+1);
        for(int j = 0; j < answers[i].size(); j++){
            if(answers[i][j] == -1){
                fprintf(file, " impossible");
            }else{
                fprintf(file, " %d", answers[i][j]);
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
}