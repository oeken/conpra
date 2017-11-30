#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <climits>
#include <queue>
#include <ctime>

using namespace std;

struct Case {
    int N; // # of nodes
    int X; // target node
    int M; // # of op executed by lea
    int* a; // node costs (we have N of them)
    int** b; // connections (we have N of them)
    int* x; // lea's operations (we have M of them)
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


// helpers
vector<vector<int>> constructAdjmat(Case &thisCase, deque<int> leapath){
    vector<vector<int>> adjmat(thisCase.N);
    for(int i = 0; i < thisCase.N; i++){
        vector<int> v(thisCase.N);
        for(int j = 0; j < thisCase.N; j++) v[j] = INT_MAX;
        adjmat[i] = v;
    }

    for(int i = 0; i < thisCase.N; i++){
        for(int j = 1; j <= thisCase.b[i][0]; j++){
            int target = thisCase.b[i][j]-1;
            if(i != target && thisCase.a[i] < adjmat[i][target]) adjmat[i][target] = thisCase.a[i];
        }
    }

    if(leapath.size() >= 2){
        for(int i = leapath.size()-1; i > 0; i--){
            int source = leapath[i]-1;
            int target = leapath[i-1]-1;
            adjmat[source][target] = 0;
        }
    }

    return adjmat;
}

vector<vector<int>> constructAdjlist(Case &thisCase, deque<int> leapath){
    vector<vector<int>> adjlist(thisCase.N);
    for(int i = 0; i < thisCase.N; i++){
        for(int j = 1; j <= thisCase.b[i][0]; j++){
            int target = thisCase.b[i][j]-1;
            if(i != target){ // eliminate self links
                // if does not contain add it
                if(find(begin(adjlist[i]), end(adjlist[i]), target) == end(adjlist[i])) adjlist[i].push_back(target);
            }
        }
    }

    if(leapath.size() >= 2){
        for(int i = leapath.size()-1; i > 0; i--){
            int source = leapath[i]-1;
            int target = leapath[i-1]-1;
            if(find(begin(adjlist[source]), end(adjlist[source]), target) == end(adjlist[source])) adjlist[source].push_back(target);
        }
    }

    return adjlist;
}

int getNext(vector<int> &distances, vector<bool> &visited){
    int node = -1;
    int dist = INT_MAX;
    for(int i = 0; i < distances.size(); i++){
        if(!visited[i] && distances[i] != INT_MAX && distances[i] < dist) {dist = distances[i]; node = i; }
    }
    return node;
}

vector<vector<int>> dijkstraShortestPath(int s, vector<vector<int>> adjlist, vector<vector<int>> adjmat){
    int N = adjlist.size();

    vector<bool> visited(N);
    vector<int> distances(N);
    vector<int> parents(N);
    for(int i = 0; i < N; i++){
        distances[i] = INT_MAX;
        parents[i] = -1;
    }

    int node = s;
    distances[node] = 0;
    parents[node] = -2;

    do{
        // update neighbours
        for(int j = 0; j < adjlist[node].size(); j++){
            int target = adjlist[node][j];
            if(!visited[target] && distances[node] + adjmat[node][target] < distances[target]){
                distances[target] = distances[node] + adjmat[node][target];
                parents[target] = node;
            }
        }

        // mark this node visited
        visited[node] = true;

        // get next node
        node = getNext(distances, visited);

    } while(node != -1);

    vector<vector<int>> djk({distances, parents});
    return djk;
}

deque<int> findlea(Case &thisCase){
    int lea = 1;
    deque<int> q;
    q.push_back(lea);
    for(int i = 0; i < thisCase.M; i++){
        int target = thisCase.x[i];
        if(!target){
            if(q.size() > 1) q.pop_back();
        }else{
            q.push_back(target);
        }
    }
    return q;
}

vector<int> solveProblem(vector<Case> &cases){
    vector<int> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}


int solveCase(Case &thisCase){
    deque<int> leapath = findlea(thisCase);
    vector<vector<int>> adjlist = constructAdjlist(thisCase, leapath);
    vector<vector<int>> adjmat = constructAdjmat(thisCase, leapath);
    vector<vector<int>> dist_and_parents = dijkstraShortestPath(leapath.back()-1, adjlist, adjmat);

    int shortest_path = dist_and_parents[0][thisCase.X-1];
    int answer = -1;
    if(shortest_path != INT_MAX) answer = shortest_path + thisCase.a[thisCase.X-1];
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
        scanf("%d", &thisCase.X);
        scanf("%d", &thisCase.M);

        thisCase.a = new int[thisCase.N];
        thisCase.b = new int*[thisCase.N];
        thisCase.x = new int[thisCase.M];

        for(int j = 0; j < thisCase.N; j++){ // read costs
            scanf("%d", &thisCase.a[j]);
        }

        for(int j = 0; j < thisCase.N; j++){ // for each line
            int bj = -1;
            scanf("%d", &bj); // read the number of connections
            int* b = new int[bj+1]; // create the array
            b[0] = bj; // make the count the first element
            for(int k = 1; k < bj+1; k++){
                scanf("%d", &b[k]); // read and write the remaining connections
            }
            thisCase.b[j] = b; // save it
        }

        for(int j = 0; j < thisCase.M; j++){
            scanf("%d", &thisCase.x[j]);
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
        fscanf(file, "%d", &thisCase.X);
        fscanf(file, "%d", &thisCase.M);

        thisCase.a = new int[thisCase.N];
        thisCase.b = new int*[thisCase.N];
        thisCase.x = new int[thisCase.M];

        for(int j = 0; j < thisCase.N; j++){ // read costs
            fscanf(file, "%d", &thisCase.a[j]);
        }

        for(int j = 0; j < thisCase.N; j++){ // for each line
            int bj = -1;
            fscanf(file, "%d", &bj); // read the number of connections
            int* b = new int[bj+1]; // create the array
            b[0] = bj; // make the count the first element
            for(int k = 1; k < bj+1; k++){
                fscanf(file, "%d", &b[k]); // read and write the remaining connections
            }
            thisCase.b[j] = b; // save it
        }

        for(int j = 0; j < thisCase.M; j++){
            fscanf(file, "%d", &thisCase.x[j]);
        }

        cases[i] = thisCase;
    }

    fclose (file);
    return cases;
}


// writers
void writeOutput(vector<int> &answers){
    for(int i = 0; i < answers.size(); i++){
        if(answers[i] == -1){
            printf("Case #%d: impossible\n", i+1);
        }else{
            printf("Case #%d: %d\n", i+1, answers[i]);
        }
    }
}

void writeOutputFile(vector<int> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < answers.size(); i++){
        if(answers[i] == -1){
            fprintf(file, "Case #%d: impossible\n", i+1);
        }else{
            fprintf(file, "Case #%d: %d\n", i+1, answers[i]);
        }
    }

    fclose(file);
}