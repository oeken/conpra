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
    int n;   // number of people
    int* v1; // source person
    int* v2; // target person
    int* d;  // distance
};


// readers
vector<Case> readInput();
vector<Case> readInputFile();

// writers
void writeOutput(vector<vector<vector<int>>> &answers);
void writeOutputFile(vector<vector<vector<int>>> &answers);

// helpers
vector<vector<vector<int>>> solveProblem(vector<Case> &cases);
vector<vector<int>> solveCase(Case &thisCase);

// file names
string inFileName = "in.txt";
string outFileName = "out.txt";


int main() {
//    vector<Case> cases = readInputFile();
    vector<Case> cases = readInput();


    vector<vector<vector<int>>> answers = solveProblem(cases);

    writeOutput(answers);
//    writeOutputFile(answers);
    return 0;
}


// helpers
vector<vector<int>> constructGraph(Case &thisCase){
    int num_edges = thisCase.n * (thisCase.n-1) / 2;
    vector<vector<int>> edges(num_edges);

    for(int i = 0; i < num_edges; i++){
        vector<int> edge({thisCase.v1[i] ,thisCase.v2[i], thisCase.d[i]});
        edges[i] = edge;
    }
    return edges;
}

vector<vector<int>> kruskalMST(vector<vector<int>> &edges, int n){
    // sort
    sort(begin(edges), end(edges), [](vector<int> a, vector<int> b) {
        return a[2] < b[2];
    });

    // add greedily
    UnionFind uf(n);
    int edges_added = 0;
    int i = 0;
    int total_weight = 0;
    vector<vector<int>> greetings(n-1);
    while(edges_added + 1 < n){
        if(uf.find(edges[i][0]) != uf.find(edges[i][1])){
            uf.merge(edges[i][0], edges[i][1]);
            total_weight += edges[i][2];
            vector<int> edge({edges[i][0], edges[i][1]});
            greetings[edges_added] = edge;
            edges_added++;
        }
        i++;
    }

    sort(begin(greetings), end(greetings), [](vector<int> a, vector<int> b){
        if(a[0] < b[0]){
            return true;
        }
        if(a[0] == b[0]){
            return a[1] < b[1];
        }
        return false;
    });
    return greetings;
}

vector<vector<vector<int>>> solveProblem(vector<Case> &cases){
    vector<vector<vector<int>>> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}

vector<vector<int>> solveCase(Case &thisCase){
    vector<int> v1(2);
    vector<vector<int>> v2;
    v2.push_back(v1);

    vector<vector<int>> edges = constructGraph(thisCase);
    return kruskalMST(edges, thisCase.n);
//    return v2;
}


// readers
vector<Case> readInput(){
    int dummy;
    // read # of cases
    int t;
    scanf("%d", &t);

    // read each case
    vector<Case> cases(t);
    for(int i = 0; i < t; i++){
        Case thisCase;
        scanf("%d", &thisCase.n);

        thisCase.v1 = new int[thisCase.n * (thisCase.n-1) / 2];
        thisCase.v2 = new int[thisCase.n * (thisCase.n-1) / 2];
        thisCase.d = new int[thisCase.n * (thisCase.n-1) / 2];

        int c = 0;
        for(int j = 0; j < thisCase.n; j++){
            for(int k = 0; k < thisCase.n; k++){
                if(j < k){
                    thisCase.v1[c] = j;
                    thisCase.v2[c] = k;
                    scanf("%d", &thisCase.d[c]);
                    c++;
                }else{
                    scanf("%d", &dummy); // skip it
                }
            }
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

        thisCase.v1 = new int[thisCase.n * (thisCase.n-1) / 2];
        thisCase.v2 = new int[thisCase.n * (thisCase.n-1) / 2];
        thisCase.d = new int[thisCase.n * (thisCase.n-1) / 2];

        int c = 0;
        for(int j = 0; j < thisCase.n; j++){
            for(int k = 0; k < thisCase.n; k++){
                if(j < k){
                    thisCase.v1[c] = j;
                    thisCase.v2[c] = k;
                    fscanf(file, "%d", &thisCase.d[c]);
                    c++;
                }else{
                    fscanf(file, "%d", &dummy); // skip it
                }
            }
        }
        cases[i] = thisCase;
    }

    fclose (file);
    return cases;
}


// writers
void writeOutput(vector<vector<vector<int>>> &answers){
    for(int i = 0; i < answers.size(); i++){
        printf("Case #%d:\n", i+1);
        for(int j = 0; j < answers[i].size(); j++){
            printf("%d %d\n", answers[i][j][0]+1, answers[i][j][1]+1);
        }
    }
}

void writeOutputFile(vector<vector<vector<int>>> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < answers.size(); i++){
        fprintf(file, "Case #%d:\n", i+1);
        for(int j = 0; j < answers[i].size(); j++){
            fprintf(file, "%d %d\n", answers[i][j][0]+1, answers[i][j][1]+1);
        }
    }

    fclose(file);
}