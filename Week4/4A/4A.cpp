#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <climits>
#include <queue>
#include <ctime>

using namespace std;

struct Case {
    int N; // number of intersections
    int M; // number of roads
    int S; // number of stations
    int* a; // source intersection
    int* b; // target intersection
    int* c; // distance
    int* s; // stations
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
//int pop(vector<int> &queue, vector<int> &distances){
//    int node = -1;
//    int node_index = -1;
//    int dist = INT_MAX;
//    for(int j = 0; j < queue.size(); j++){
//        if(distances[queue[j]] < dist) {dist = distances[queue[j]]; node = queue[j]; node_index = j;}
//    }
//    queue.erase(begin(queue)+node_index); // remove from queue
//    return node;
//}
//
//void push(int node, vector<int> &queue){
//    for(int j = 0; j < queue.size(); j++){
//        if(queue[j] == node) return;
//    }
//    queue.push_back(node);
//}
int getNext(vector<int> &distances, vector<bool> &visited){
    int node = -1;
    int dist = INT_MAX;
    for(int i = 0; i < distances.size(); i++){
        if(!visited[i] && distances[i] < dist) {dist = distances[i]; node = i; }
    }
    return node;
}

vector<vector<int>> constructAdjmat(Case &thisCase){
    vector<vector<int>> adjmat(thisCase.N);
    for(int i = 0; i < thisCase.N; i++){
        vector<int> v(thisCase.N);
        for(int j = 0; j < thisCase.N; j++){
            v[j] = INT_MAX;
        }
        adjmat[i] = v;
    }

    for(int i = 0; i < thisCase.M; i++){
        int a = thisCase.a[i]-1;
        int b = thisCase.b[i]-1;
        int c = thisCase.c[i];
        if(c < adjmat[a][b]){ // between two nodes choose the smallest distance possible
            adjmat[a][b] = c;
            adjmat[b][a] = c;
        }
    }
    return adjmat;
}

vector<vector<int>> constructAdjlist(Case &thisCase){
    vector<vector<int>> adjlist(thisCase.N);
    for(int i = 0; i < thisCase.M; i++){
        int a = thisCase.a[i]-1;
        int b = thisCase.b[i]-1;
        if(find(begin(adjlist[a]), end(adjlist[a]), b) == end(adjlist[a])) adjlist[a].push_back(b); // insert if doesnt contain
        if(find(begin(adjlist[b]), end(adjlist[b]), a) == end(adjlist[b])) adjlist[b].push_back(a); // insert if doesnt contain
    }
    return adjlist;
}

vector<vector<int>> dijkstraShortestPath(int s, vector<vector<int>> adjlist, vector<vector<int>> adjmat){
    int N = adjlist.size();
//    vector<int> queue;

    vector<bool> visited(N);
    vector<int> distances(N);
    vector<int> parents(N);
    for(int i = 1; i < N; i++){
        distances[i] = INT_MAX;
        parents[i] = -1;
    }

//    push(0, queue);
    int node = 0;
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

vector<vector<int>> solveProblem(vector<Case> &cases){
    vector<vector<int>> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}


vector<int> solveCase(Case &thisCase){
    vector<vector<int>> adjlist = constructAdjlist(thisCase);
    vector<vector<int>> adjmat = constructAdjmat(thisCase);
    vector<vector<int>> dist_and_parents = dijkstraShortestPath(0, adjlist, adjmat);

    vector<int> distances = dist_and_parents[0];

    vector<int> station_indices;
    for(int i = 0; i < thisCase.S; i++){ station_indices.push_back(i); }

    sort(begin(station_indices), end(station_indices), [&](int a, int b){
        int sta_a = thisCase.s[a]-1;
        int sta_b = thisCase.s[b]-1;
        if(distances[sta_a] < distances[sta_b]){
            return true;
        }else if(distances[sta_a] == distances[sta_b]){
            return sta_a < sta_b;
        }
        return false;
    });

    vector<int> answer;
    int min_dist =  distances[thisCase.s[station_indices[0]]-1];
    answer.push_back(min_dist);

    for(int i = 0; i < station_indices.size(); i++){
        int index = station_indices[i];
        int station = thisCase.s[index]-1;
        int distance = distances[station];

        if(distance == min_dist){
            answer.push_back(station+1);
        }else{
            break;
        }
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
        scanf("%d", &thisCase.S);

        thisCase.a = new int[thisCase.M];
        thisCase.b = new int[thisCase.M];
        thisCase.c = new int[thisCase.M];
        thisCase.s = new int[thisCase.S];

        for(int j = 0; j < thisCase.M; j++){
            scanf("%d", &thisCase.a[j]);
            scanf("%d", &thisCase.b[j]);
            scanf("%d", &thisCase.c[j]);
        }

        for(int j = 0; j < thisCase.S; j++){
            scanf("%d", &thisCase.s[j]);
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
        fscanf(file, "%d", &thisCase.S);

        thisCase.a = new int[thisCase.M];
        thisCase.b = new int[thisCase.M];
        thisCase.c = new int[thisCase.M];
        thisCase.s = new int[thisCase.S];

        for(int j = 0; j < thisCase.M; j++){
            fscanf(file, "%d", &thisCase.a[j]);
            fscanf(file, "%d", &thisCase.b[j]);
            fscanf(file, "%d", &thisCase.c[j]);
        }

        for(int j = 0; j < thisCase.S; j++){
            fscanf(file, "%d", &thisCase.s[j]);
        }

        cases[i] = thisCase;
    }

    fclose (file);
    return cases;
}


// writers
void writeOutput(vector<vector<int>> &answers){
    for(int i = 0; i < answers.size(); i++){
        printf("Case #%d: %d", i+1, answers[i][0]);
        for(int j = 1;  j < answers[i].size(); j++){
            printf(" %d", answers[i][j]);
        }
        printf("\n");
    }
}

void writeOutputFile(vector<vector<int>> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < answers.size(); i++){
        fprintf(file, "Case #%d: %d", i+1, answers[i][0]);
        for(int j = 1;  j < answers[i].size(); j++){
            fprintf(file, " %d", answers[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}


//
//
////    while(node != -1){
//// find next node and pop from queue
//int node = pop(queue, distances);
//
//// update neighbours
//for(int j = 0; j < adjlist[node].size(); j++){
//int target = adjlist[node][j];
//if(!visited[target]){
//// add to queue here
//push(target, queue);
//// update
//if(distances[node] + adjmat[node][target] < distances[target]){
//distances[target] = distances[node] + adjmat[node][target];
//parents[target] = node;
//}
//}
//}
//
//// mark this node visited
//visited[node] = true;
////    }
