#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <queue>
#include <ctime>

using namespace std;

class UnionFind {

private:
    int count; // # of sets
    int* ids; // parents of each element
    int* sizes; // size of each set/id, notice: there are redundant values (after merge operations)

public:

    // Create an empty union find data structure with N isolated sets.
    UnionFind(int N) {
        count = N;
        ids = new int[N];
        sizes = new int[N];
        for (int i = 0; i<N; i++) {
            ids[i] = i, sizes[i] = 1;
        }
    }

    ~UnionFind() {
        delete[] ids;
        delete[] sizes;
    }

    // Return the id of component corresponding to object p.
    int find(int p) {
        int element = p;

        // find the root
        while (element != ids[element]) element = ids[element];

        // now element is root, compress the path
        while (p != element) {
            int newp = ids[p];
            ids[p] = element;
            p = newp;
        }

        return element;
    }

    // Replace sets containing x and y with their union.
    void merge(int x, int y) {
        int i = find(x);
        int j = find(y);
        if (i == j) return;

        // make smaller root point to larger one
        if (sizes[i] < sizes[j]) {
            ids[i] = j;
            sizes[j] += sizes[i];
        }
        else {
            ids[j] = i;
            sizes[i] += sizes[j];
        }
        count--;
    }

    // Are objects x and y in the same set?
    bool connected(int x, int y) { return find(x) == find(y); }

    // Return the number of disjoint sets.
    int getCount() { return count; }
};

struct Case {
    int n;   // number of nodes
    int m; // number of edges
    int* a; // edge source
    int* b; // edge target
    int* c;  // edge weight
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
vector<vector<int>> constructGraph(Case &thisCase){
    int num_edges = thisCase.m;
    vector<vector<int>> edges(num_edges);

    for(int i = 0; i < num_edges; i++){
        vector<int> edge({thisCase.a[i]-1 ,thisCase.b[i]-1, -1*thisCase.c[i]});
        edges[i] = edge;
    }
    return edges;
}


int kruskalMST(vector<vector<int>> &edges, int n){
    // sort
    sort(begin(edges), end(edges), [](vector<int> a, vector<int> b) {
        return a[2] < b[2];
    });

    // add greedily
    UnionFind uf(n);
    int total_weight = 0;
    for(int i = 0; i < edges.size(); i++){
        if(uf.find(edges[i][0]) != uf.find(edges[i][1])){
            uf.merge(edges[i][0], edges[i][1]);
            total_weight += edges[i][2];
        }
    }
    return total_weight;
}

vector<int> solveProblem(vector<Case> &cases){
    vector<int> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}

int solveCase(Case &thisCase){
    // construct negative edge list
    vector<vector<int>> edges = constructGraph(thisCase);

    // sum of the edges (positive)
    int sum = 0;
    for(int i = 0; i < edges.size(); i++){
        sum += edges[i][2];
    }
    sum *= -1;

    // maximum spanning tree
    int max_spanning_tree = -1 * kruskalMST(edges, thisCase.n);
    int answer = sum - max_spanning_tree;
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
        scanf("%d", &thisCase.n);
        scanf("%d", &thisCase.m);

        thisCase.a = new int[thisCase.m];
        thisCase.b = new int[thisCase.m];
        thisCase.c = new int[thisCase.m];

        for(int j = 0; j < thisCase.m; j++){
            scanf("%d", &thisCase.a[j]);
            scanf("%d", &thisCase.b[j]);
            scanf("%d", &thisCase.c[j]);
        }
        cases[i] = thisCase;
    }

    return cases;
}

vector<Case> readInputFile(){
    FILE* file;
    file = fopen (inFileName.c_str(),"r");
    int dummy;

    // read # of cases
    int t;
    fscanf(file, "%d", &t);

    // read each case
    vector<Case> cases(t);
    for(int i = 0; i < t; i++){
        Case thisCase;
        fscanf(file, "%d", &thisCase.n);
        fscanf(file, "%d", &thisCase.m);

        thisCase.a = new int[thisCase.m];
        thisCase.b = new int[thisCase.m];
        thisCase.c = new int[thisCase.m];

        for(int j = 0; j < thisCase.m; j++){
            fscanf(file, "%d", &thisCase.a[j]);
            fscanf(file, "%d", &thisCase.b[j]);
            fscanf(file, "%d", &thisCase.c[j]);
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