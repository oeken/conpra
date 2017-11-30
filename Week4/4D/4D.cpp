#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <climits>
#include <queue>
#include <ctime>

using namespace std;

struct Case {
    int w; // width
    int h; // height
    int** maze;
    int sx, sy, tx, ty;
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
vector<vector<int>> tiles(Case &thisCase){
    vector<vector<int>> tiles;
    for(int i = 0; i < thisCase.h; i++){ // for each y
        for(int j = 0; j < thisCase.w; j++){ // for each x
            if(!thisCase.maze[i][j]){
                vector<int> v({j, i});
                tiles.push_back(v);
            }
        }
    }
    return tiles;
}

vector<vector<int>> tile2index(vector<vector<int>> &tiles, Case &thisCase){
    vector<vector<int>> indices(thisCase.h);
    for(int i = 0; i < thisCase.h; i++){
        vector<int> v(thisCase.w);
        for(int j = 0; j < thisCase.w; j++){
            v[j] = -1;
        }
        indices[i] = v;
    }

    for(int i = 0; i < tiles.size(); i++){
        int x = tiles[i][0];
        int y = tiles[i][1];
        indices[y][x] = i;
    }

    return indices;
}

vector<vector<int>> constructAdjlist(vector<vector<int>> &tiles, vector<vector<int>> &tile_indices, Case &thisCase){
    int copies = 3;
    int N = tiles.size();
    vector<vector<int>> adjlist(N * (copies+1));

    for(int i = 0; i < copies; i++){
        for(int j = 0; j < N; j++){
            int x = tiles[j][0];
            int y = tiles[j][1];
            int index = tile_indices[y][x] + (i*N);

            // form the base graph
            if(x - 1 >= 0 && !thisCase.maze[y][x-1]){
                int target = tile_indices[y][x-1] + (i*N);
                adjlist[index].push_back(target);
            }
            if(x + 1 < thisCase.w && !thisCase.maze[y][x+1]){
                int target = tile_indices[y][x+1] + (i*N);
                adjlist[index].push_back(target);
            }
            if(y - 1 >= 0 && !thisCase.maze[y-1][x]){
                int target = tile_indices[y-1][x] + (i*N);
                adjlist[index].push_back(target);
            }
            if(y + 1 < thisCase.h && !thisCase.maze[y+1][x]){
                int target = tile_indices[y+1][x] + (i*N);
                adjlist[index].push_back(target);
            }

            // add connections to upper graph
            if(x - 2 >= 0 && !thisCase.maze[y][x-2] && thisCase.maze[y][x-1]){
                int target = tile_indices[y][x-2] + ((i+1)*N);
                adjlist[index].push_back(target);
            }
            if(x + 2 < thisCase.w && !thisCase.maze[y][x+2] && thisCase.maze[y][x+1]){
                int target = tile_indices[y][x+2] + ((i+1)*N);
                adjlist[index].push_back(target);
            }
            if(y - 2 >= 0 && !thisCase.maze[y-2][x] && thisCase.maze[y-1][x]){
                int target = tile_indices[y-2][x] + ((i+1)*N);
                adjlist[index].push_back(target);
            }
            if(y + 2 < thisCase.h && !thisCase.maze[y+2][x] && thisCase.maze[y+1][x]){
                int target = tile_indices[y+2][x] + ((i+1)*N);
                adjlist[index].push_back(target);
            }
        }

    }

    // add the inner connections of the top graph
    int i = 3;
    for(int j = 0; j < N; j++) {
        int x = tiles[j][0];
        int y = tiles[j][1];
        int index = tile_indices[y][x] + (i*N);

        // form the base graph
        if (x - 1 >= 0 && !thisCase.maze[y][x - 1]) {
            int target = tile_indices[y][x - 1] + (i*N);
            adjlist[index].push_back(target);
        }
        if (x + 1 < thisCase.w && !thisCase.maze[y][x + 1]) {
            int target = tile_indices[y][x + 1] + (i*N);
            adjlist[index].push_back(target);
        }
        if (y - 1 >= 0 && !thisCase.maze[y - 1][x]) {
            int target = tile_indices[y - 1][x] + (i*N);
            adjlist[index].push_back(target);
        }
        if (y + 1 < thisCase.h && !thisCase.maze[y + 1][x]) {
            int target = tile_indices[y + 1][x] + (i*N);
            adjlist[index].push_back(target);
        }
    }

    return adjlist;
}

vector<vector<int>> bfsShortestPath(vector<vector<int>> &adjlist, int s){
    int N = adjlist.size();
    vector<bool> visited(N);
    vector<int> distances(N);
    vector<int> parents(N);
    for(int i = 0; i < N; i++) distances[i] = INT_MAX;
    for(int i = 0; i < N; i++) parents[i] = -1;

    deque<int> q;
    distances[s] = 0;
    parents[s] = -2;
    visited[s] = true;
    q.push_back(s);
    while(!q.empty()){
//        for(int i = 0; i < q.size(); i++) cout << q[i] << ", ";

        // get node
        int node = q.front();
        q.pop_front();

        // visit neighbours
        for(int i = 0; i < adjlist[node].size(); i++){
            int target = adjlist[node][i];
            if(!visited[target]) {
                distances[target] = distances[node] + 1;
                parents[target] = node;
                visited[target] = true;
                q.push_back(target);
            }
        }
    }

    vector<vector<int>> rv({distances, parents});
    return rv;

}

vector<vector<int>> solveProblem(vector<Case> &cases){
    vector<vector<int>> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}


vector<int> solveCase(Case &thisCase){
    vector<vector<int>> ts = tiles(thisCase);
    vector<vector<int>> tis = tile2index(ts, thisCase);
    int s = tis[thisCase.sy][thisCase.sx];
    int t = tis[thisCase.ty][thisCase.tx];

    vector<vector<int>> adjlist = constructAdjlist(ts, tis, thisCase);
    vector<vector<int>> distances_parents = bfsShortestPath(adjlist, s);
    vector<int> distances = distances_parents[0];
    vector<int> parents = distances_parents[1];

    int N = ts.size();
    vector<int> targets({t, t+N, t+2*N, t+3*N});
    int minimum = INT_MAX;
    int minimum_index = -1;
    for(int i = 0; i < 4; i++){
        if(distances[targets[i]] < minimum) {minimum = distances[targets[i]]; minimum_index = targets[i]; }
    }

    vector<int> answer(6);
    for(int i = 0; i < answer.size(); i++) answer[i] = -1;

    int node = minimum_index;
    int found = 0;
    int c = 0;
    while(node != s && node != -1){
        c++;
        int next_node = parents[node];
        int group = node / N;
        int next_group = next_node / N;

        if(group != next_group){
            int x1 = ts[node % N][0];
            int y1 = ts[node % N][1];

            int x2 = ts[next_node % N][0];
            int y2 = ts[next_node % N][1];

            answer[found] = (x1 + x2)/2 + 1;
            answer[found+1] = (y1 + y2)/2 + 1;
            found += 2;
        }
        node = next_node;
        if(c == N * 4) break;
    }

    return answer;
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
        scanf("%d", &thisCase.w);
        scanf("%d", &thisCase.h);
        scanf("%c", &dummy); // skip \n

        thisCase.maze = new int*[thisCase.h];

        for(int j = 0; j < thisCase.h; j++){ // read costs
            thisCase.maze[j] = new int[thisCase.w];
            for(int k = 0; k < thisCase.w; k++){
                char what;
                scanf("%c", &what);

                if(what == '@'){ // wall
                    thisCase.maze[j][k] = 1;
                }else if(what == '_'){ // path
                    thisCase.maze[j][k] = 0;
                    if(j == 0 || j == thisCase.h-1 || k == 0 || k == thisCase.w-1){
                        thisCase.tx = k;
                        thisCase.ty = j;
                    }
                }else{ // lea
                    thisCase.maze[j][k] = 0;
                    thisCase.sx = k;
                    thisCase.sy = j;
                }
            }
            scanf("%c", &dummy); // skip \n
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
        fscanf(file, "%d", &thisCase.w);
        fscanf(file, "%d", &thisCase.h);
        fscanf(file, "%c", &dummy); // skip \n

        thisCase.maze = new int*[thisCase.h];

        for(int j = 0; j < thisCase.h; j++){ // read costs
            thisCase.maze[j] = new int[thisCase.w];
            for(int k = 0; k < thisCase.w; k++){
                char what;
                fscanf(file, "%c", &what);

                if(what == '@'){ // wall
                    thisCase.maze[j][k] = 1;
                }else if(what == '_'){ // path
                    thisCase.maze[j][k] = 0;
                    if(j == 0 || j == thisCase.h-1 || k == 0 || k == thisCase.w-1){
                        thisCase.tx = k;
                        thisCase.ty = j;
                    }
                }else{ // lea
                    thisCase.maze[j][k] = 0;
                    thisCase.sx = k;
                    thisCase.sy = j;
                }
            }
            fscanf(file, "%c", &dummy); // skip \n
        }
        cases[i] = thisCase;
    }

    fclose (file);
    return cases;
}


// writers
void writeOutput(vector<vector<int>> &answers){
    for(int i = 0; i < answers.size(); i++){
        printf("Case #%d:\n", i+1);
        for(int j = 0; j < 6; j = j + 2){
            if(answers[i][j] == -1){
                printf("unused\n");
            }else{
                printf("%d %d\n", answers[i][j], answers[i][j+1]);
            }
        }
    }
}

void writeOutputFile(vector<vector<int>> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < answers.size(); i++){
        fprintf(file, "Case #%d:\n", i+1);
        for(int j = 0; j < 6; j = j + 2){
            if(answers[i][j] == -1){
                fprintf(file, "unused\n");
            }else{
                fprintf(file, "%d %d\n", answers[i][j], answers[i][j+1]);
            }
        }
    }

    fclose(file);
}