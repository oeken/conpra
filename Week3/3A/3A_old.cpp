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
    int n; // number of planets
    int* x; // x's of planets
    int* y; // y's of planets
    int* z; // z's of planets
};


// readers
vector<Case> readInput();
vector<Case> readInputFile();

// writers
void writeOutput(vector<int> &answers);
void writeOutputFile(vector<int> &answers);

// helpers
void print_edges(vector<vector<int>> &edges);
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
void print_edges(vector<vector<int>> &edges){
    for(int i = 0; i < edges.size(); i++){
        cout << edges[i][0] << ", " << edges[i][1] << ", " << edges[i][2] << endl;
    }
    cout << endl;
}

vector<vector<int>> constructGraph(Case &thisCase){
    int num_edges = thisCase.n * (thisCase.n-1) / 2;
    vector<vector<int>> edges(num_edges);

    int c = 0;
    for(int i = 0; i < thisCase.n; i++){
        for(int j = i+1; j < thisCase.n; j++){
            int dx = abs(thisCase.x[i] - thisCase.x[j]);
            int dy = abs(thisCase.y[i] - thisCase.y[j]);
            int dz = abs(thisCase.z[i] - thisCase.z[j]);

            vector<int> edge({i ,j, dx+dy+dz});
            edges[c] = edge;

            c++;
        }
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
//    int edges_added = 0;
//    int i = 0;
    int total_weight = 0;
//    while(edges_added + 1 < n){
    for(int i = 0; i < edges.size(); i++){
        if(uf.find(edges[i][0]) != uf.find(edges[i][1])){
            uf.merge(edges[i][0], edges[i][1]);
            total_weight += edges[i][2];
//            edges_added++;
        }
//        i++;
    }

    return total_weight;
}

//int distance(int i, int j, Case &thisCase){
//    int dx = abs(thisCase.x[i] - thisCase.x[j]);
//    int dy = abs(thisCase.y[i] - thisCase.y[j]);
//    int dz = abs(thisCase.z[i] - thisCase.z[j]);
//    return dx + dy + dz;
//}

vector<int> solveProblem(vector<Case> &cases){
    vector<int> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}


//bool distComp(vector<int> a, vector<int> b){
//    return a[1] < b[2];
//}

int solveCase(Case &thisCase){
    vector<vector<int>> edges = constructGraph(thisCase);
    return kruskalMST(edges, thisCase.n);

//    vector<bool> inMST(thisCase.n);
//    vector<vector<int>> distances(thisCase.n);
//    for(int i = 0; i < thisCase.n; i++){
//        vector<int> distance({i, INT_MAX});
//        distances[i] = distance;
//    }
//    priority_queue<vector<int>, vector<vector<int>>, function<bool(vector<int>, vector<int>)>> pq(distComp);


//    return -1;
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

        thisCase.x = new int[thisCase.n];
        thisCase.y = new int[thisCase.n];
        thisCase.z = new int[thisCase.n];

        for(int j = 0; j < thisCase.n; j++){
            scanf("%d", &thisCase.x[j]);
            scanf("%d", &thisCase.y[j]);
            scanf("%d", &thisCase.z[j]);
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
        fscanf(file, "%d", &thisCase.n);

        thisCase.x = new int[thisCase.n];
        thisCase.y = new int[thisCase.n];
        thisCase.z = new int[thisCase.n];

        for(int j = 0; j < thisCase.n; j++){
            fscanf(file, "%d", &thisCase.x[j]);
            fscanf(file, "%d", &thisCase.y[j]);
            fscanf(file, "%d", &thisCase.z[j]);
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