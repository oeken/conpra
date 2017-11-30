#include <iostream>
#include <vector>
#include <float.h>
#include <math.h>
#include <algorithm>
#include <climits>
#include <deque>
#include <set>
#include <tuple>

#define EPSILON 0.00001

using namespace std;

struct Case {
    int N;
    int M;
    int* a;
    int* b;
    double* c;
};



// readers
vector<Case> readInput();
vector<Case> readInputFile();

// writers
void writeOutput(vector<double> &answers);
void writeOutputFile(vector<double> &answers);

// helpers
vector<double> solveProblem(vector<Case> &cases);
double solveCase(Case &thisCase);

// file names
string inFileName = "in.txt";
string outFileName = "out.txt";


int main() {
//    vector<Case> cases = readInputFile();
    vector<Case> cases = readInput();

    vector<double> answers = solveProblem(cases);

    writeOutput(answers);
//    writeOutputFile(answers);

    return 0;
}


// helpers
bool detectNegCycle(vector<vector<int>> &adjlist, vector<vector<double>> &adjmat, vector<double> &distances){
    int N = adjmat.size();
    for(int i = 0; i < N; i++){
        for(int j = 0; j < adjlist[i].size(); j++){
            int target = adjlist[i][j];
            if(fabs(DBL_MAX - distances[i]) > EPSILON  && fabs(DBL_MAX - distances[target]) > EPSILON){
                if(distances[i] + adjmat[i][target] < distances[target]) return true;
            }
        }
    }
    return false;

}
vector<vector<double>> constructAdjmat(Case &thisCase){
    vector<vector<double>> adjmat(thisCase.N);
    for(int i = 0; i < thisCase.N; i++){
        vector<double> v(thisCase.N);
        for(int j = 0; j < thisCase.N; j++){
            v[j] = DBL_MAX;
        }
        adjmat[i] = v;
    }

    for(int i = 0; i < thisCase.M; i++){
        int a = thisCase.a[i]-1;
        int b = thisCase.b[i]-1;
        double c = thisCase.c[i];
        if(log(c) < adjmat[a][b]) adjmat[a][b] = log(c);
    }
    return adjmat;
}

vector<vector<int>> constructAdjlist(Case &thisCase){
    vector<vector<int>> adjlist(thisCase.N);
    for(int i = 0; i < thisCase.M; i++){
        int a = thisCase.a[i]-1;
        int b = thisCase.b[i]-1;
        if(find(begin(adjlist[a]), end(adjlist[a]), b) == end(adjlist[a])) adjlist[a].push_back(b);
    }
    return adjlist;
}

tuple<vector<double>, vector<int>, bool> bellmanfordShortestPath(vector<vector<int>> &adjlist, vector<vector<double>> &adjmat){
    int N = adjlist.size();

    vector<double> distances(N);
    vector<int> parents(N);
    for(int i = 0; i < N; i++) { distances[i] = DBL_MAX; parents[i] = -1; }

    distances[0] = 0;

    for(int i = 1; i < N; i++){
        for(int j = 0; j < N; j++){
            for(int k = 0; k < adjlist[j].size(); k++){
                int target = adjlist[j][k];
                if(fabs(DBL_MAX - distances[j]) > EPSILON && distances[j] + adjmat[j][target] < distances[target]){
                    distances[target] = distances[j] + adjmat[j][target];
                    parents[target] = j;
                }
            }
        }
    }

    // detect negative cycle
    bool hasNegativeCycle = detectNegCycle(adjlist, adjmat, distances);

    tuple<vector<double>, vector<int>, bool> p(distances, parents, hasNegativeCycle);
    return p;
}

vector<double> solveProblem(vector<Case> &cases){
    vector<double> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}


double solveCase(Case &thisCase){
    // construct graph
    vector<vector<int>> adjlist = constructAdjlist(thisCase);
    vector<vector<double>> adjmat = constructAdjmat(thisCase);

    // run bellmann ford
    vector<double> distances;
    vector<int> parents;
    bool hasNegCycle;

    tuple<vector<double>, vector<int>, bool> bfsp = bellmanfordShortestPath(adjlist, adjmat);
    tie(distances, parents, hasNegCycle) = bfsp;

    // if there are cycles
    if(hasNegCycle) return -1; // jackpot
    if(fabs(DBL_MAX - distances[thisCase.N-1]) < EPSILON) return -2; // impossible
    return exp(distances[thisCase.N-1]); // feasible soln

//    if(hasNegCycle){
//        // if 1-N is a part of cycle then -1
//
//        int count = 0;
//        int current = thisCase.N - 1;
//        while(current != -2){
//            current = parents[current];
//            count++;
//            if(count == thisCase.N) return -1;
//        }
//    }

    // if distance is infinity then -2 / impossible
//    if(distances[thisCase.N-1] == DBL_MAX) return -2;

//    return exp(distances[thisCase.N-1]);
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

        thisCase.a = new int[thisCase.M];
        thisCase.b = new int[thisCase.M];
        thisCase.c = new double[thisCase.M];

        for(int j = 0; j < thisCase.M; j++){
            scanf("%d", &thisCase.a[j]);
            scanf("%d", &thisCase.b[j]);
            scanf("%lf", &thisCase.c[j]);
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

        thisCase.a = new int[thisCase.M];
        thisCase.b = new int[thisCase.M];
        thisCase.c = new double[thisCase.M];

        for(int j = 0; j < thisCase.M; j++){
            fscanf(file, "%d", &thisCase.a[j]);
            fscanf(file, "%d", &thisCase.b[j]);
            fscanf(file, "%lf", &thisCase.c[j]);
        }

        cases[i] = thisCase;
    }

    fclose (file);
    return cases;
}


// writers
void writeOutput(vector<double> &answers){
    for(int i = 0; i < answers.size(); i++){
        if(answers[i] == -1){ // jackpot
            printf("Case #%d: Jackpot\n", i+1);
        }else if(answers[i] == -2){ // impossible
            printf("Case #%d: impossible\n", i+1);
        }else{
            printf("Case #%d: %lf\n", i+1, answers[i]);
        }
    }
}

void writeOutputFile(vector<double> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < answers.size(); i++){
        if(answers[i] == -1){ // jackpot
            fprintf(file, "Case #%d: Jackpot\n", i+1);
        }else if(answers[i] == -2){ // impossible
            fprintf(file, "Case #%d: impossible\n", i+1);
        }else{
            fprintf(file, "Case #%d: %lf\n", i+1, answers[i]);
        }
    }

    fclose(file);
}


//tuple<vector<double>, vector<int>, bool> bellmanfordShortestPath(vector<vector<int>> &adjlist, vector<vector<double>> &adjmat){
//    int N = adjlist.size();
//
//    deque<int> queue;
//    deque<int> queue_prime;
//    vector<double> distances(N);
//    vector<int> parents(N);
//
//    for(int i = 0; i < N; i++) { distances[i] = DBL_MAX; parents[i] = -1; }
//    distances[0] = 0;
//    parents[0] = -2;
//    queue.push_back(0);
//
//    for(int i = 1; i < N; i++){
//        while(!queue.empty()){
//            int node = queue.front();
//            queue.pop_front();
//            for(int j = 0; j < adjlist[node].size(); j++){
//                int target = adjlist[node][j];
//                if(distances[node] + adjmat[node][target] < distances[target]){
//                    distances[target] = distances[node] + adjmat[node][target];
//                    parents[target] = node;
//                    if(find(begin(queue_prime), end(queue_prime), node) == end(queue_prime)) { // does not contain
//                        queue_prime.push_back(target);
//                    }
//                }
//            }
//        }
//        swap(queue, queue_prime);
//    }
//
//    // detect negative cycle
//    bool hasNegativeCycle = detectNegCycle(adjlist, adjmat, distances);
//
//    tuple<vector<double>, vector<int>, bool> p(distances, parents, hasNegativeCycle);
//    return p;
//}