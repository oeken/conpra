#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <climits>

using namespace std;

class UnionFind {

    private:
        int countElem; // # of elems
        int countSet; // # of sets
        int* ids; // sets/ids of each element
        int* sizes; // size of each set/id, notice: there are redundant values (after merge operations)

    public:

        // Create an empty union find data structure with N isolated sets.
        UnionFind(int N) {
            countElem = N;
            countSet = N;
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
            countSet--;
        }

        // Are objects x and y in the same set?
        bool connected(int x, int y) { return find(x) == find(y); }

        // Return the number of disjoint sets.
        int getCountSet() { return countSet; }

        int getMaxSize(){
            int rv = INT_MIN;
            for(int i = 0; i < countElem; i++){
                if(sizes[i] > rv) rv = sizes[i];
            }
            return rv;
        }

        int getSize(int x){
            return sizes[x];
        }
};


struct Case {
    int satelliteCount; // a
    int parallelCount; // b

    int* parallels; // x1, y1, x2, y2, ..., x(b), y(b)
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
vector<int> solveProblem(vector<Case> &cases){
    vector<int> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}


int solveCase(Case &thisCase){

    UnionFind uf(thisCase.satelliteCount);

    for(int i = 0; i < thisCase.parallelCount; i++){
        int s1 = thisCase.parallels[i*2] - 1;
        int s2 = thisCase.parallels[i*2+1] - 1;
        uf.merge(s1, s2);
    }

    int maxSize = uf.getMaxSize();
    for(int i = 0; i < thisCase.satelliteCount; i++){
        int currSize = uf.getSize(uf.find(i));
        if(maxSize == currSize) return i+1;
    }
    return -1;

}




// readers
vector<Case> readInput(){
    // read # of cases
    int caseCount;
    scanf("%d", &caseCount);

    // read each case
    vector<Case> cases(caseCount);
    for(int i = 0; i < caseCount; i++){
        Case thisCase;

        scanf("%d", &thisCase.satelliteCount);
        scanf("%d", &thisCase.parallelCount);

        thisCase.parallels = new int[thisCase.parallelCount*2];
        for(int j = 0; j < thisCase.parallelCount*2; j++) {
            scanf("%d", &thisCase.parallels[j]);
        }

        cases[i] = thisCase;
    }

    return cases;
}

vector<Case> readInputFile(){
    FILE* file;
    file = fopen (inFileName.c_str(),"r");

    // read # of cases
    int caseCount;
    fscanf(file, "%d", &caseCount);

    // read each case
    vector<Case> cases(caseCount);
    for(int i = 0; i < caseCount; i++){
        Case thisCase;

        fscanf(file, "%d", &thisCase.satelliteCount);
        fscanf(file, "%d", &thisCase.parallelCount);

        thisCase.parallels = new int[thisCase.parallelCount*2];
        for(int j = 0; j < thisCase.parallelCount*2; j++) {
            fscanf(file, "%d", &thisCase.parallels[j]);
        }

        cases[i] = thisCase;
    }

    fclose (file);
    return cases;
}



// writers
void writeOutput(vector<int> &answers){
    for(int i = 0; i < answers.size(); i++){
        if(answers[i] != -1){
            printf("Case #%d: %d\n", i+1, answers[i]);
        }else{
            printf("Case #%d: impossible\n", i+1);
        }
    }
}

void writeOutputFile(vector<int> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < answers.size(); i++){
        if(answers[i] != -1){
            fprintf(file, "Case #%d: %d\n", i+1, answers[i]);
        }else{
            fprintf(file, "Case #%d: impossible\n", i+1);
        }
    }

    fclose(file);
}