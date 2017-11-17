#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

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
    int peopleCount; // a
    int relationCount; // b
    int marriageCount; // c

    long* wealths; // m1, m2, ..., m(a-1)
    int* relations; // d1, e1, d2, e2, ..., d(b), e(b)
    int* marriages; // f1, g1, f2, g2, ..., f(c), g(c)
};


// readers
vector<Case> readInput();
vector<Case> readInputFile();

// writers
void writeOutput(vector<long> &answers);
void writeOutputFile(vector<long> &answers);

// helpers
vector<long> solveProblem(vector<Case> &cases);
long solveCase(Case &thisCase);




string inFileName = "in.txt";
string outFileName = "out.txt";



int main() {
//    vector<Case> cases = readInputFile();
    vector<Case> cases = readInput();
    vector<long> answers = solveProblem(cases);
    writeOutput(answers);
//    writeOutputFile(answers);
    return 0;
}



// helpers
vector<long> solveProblem(vector<Case> &cases){
    vector<long> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}


long solveCase(Case &thisCase){
    int jakobfugger = thisCase.peopleCount-1;

    vector<int> people(thisCase.peopleCount-1);
    for(int i = 0; i < thisCase.peopleCount-1; i++) { people[i] = i; }

    sort(begin(people), end(people), [&](int a, int b) {
        return thisCase.wealths[a] > thisCase.wealths[b];
    });


    UnionFind uf(thisCase.peopleCount);
    unordered_map<int, bool> married;

    for(int i = 0; i < thisCase.relationCount; i++){
        int p1 = thisCase.relations[i*2] - 1;
        int p2 = thisCase.relations[i*2+1] - 1;
        uf.merge(p1, p2);
    }

    for(int i = 0; i < thisCase.marriageCount; i++){
        int p1 = thisCase.marriages[i*2] - 1;
        int p2 = thisCase.marriages[i*2+1] - 1;

        uf.merge(p1, p2);
        married[p1] = true;
        married[p2] = true;
    }


    for(int i = 0; i < thisCase.peopleCount-1; i++){
        int candidate = people[i];

        bool b1 = !uf.connected(jakobfugger, candidate);
        bool b2 = married.find(jakobfugger) == married.end();
        bool b3 = married.find(candidate) == married.end();

        bool isgood = b1 && b2 && b3;

        if(isgood){
            return thisCase.wealths[candidate];
        }
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

        scanf("%d", &thisCase.peopleCount);
        scanf("%d", &thisCase.relationCount);
        scanf("%d", &thisCase.marriageCount);

        thisCase.wealths = new long[thisCase.peopleCount-1];
        for(int j = 0; j < thisCase.peopleCount-1; j++){
            scanf("%li", &thisCase.wealths[j]);
        }

        thisCase.relations = new int[thisCase.relationCount*2];
        for(int j = 0; j < thisCase.relationCount*2; j++){
            scanf("%d", &thisCase.relations[j]);
        }

        thisCase.marriages= new int[thisCase.marriageCount*2];
        for(int j = 0; j < thisCase.marriageCount*2; j++){
            scanf("%d", &thisCase.marriages[j]);
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

        fscanf(file, "%d", &thisCase.peopleCount);
        fscanf(file, "%d", &thisCase.relationCount);
        fscanf(file, "%d", &thisCase.marriageCount);

        thisCase.wealths = new long[thisCase.peopleCount-1];
        for(int j = 0; j < thisCase.peopleCount-1; j++){
            fscanf(file, "%li", &thisCase.wealths[j]);
        }

        thisCase.relations = new int[thisCase.relationCount*2];
        for(int j = 0; j < thisCase.relationCount*2; j++){
            fscanf(file, "%d", &thisCase.relations[j]);
        }

        thisCase.marriages= new int[thisCase.marriageCount*2];
        for(int j = 0; j < thisCase.marriageCount*2; j++){
            fscanf(file, "%d", &thisCase.marriages[j]);
        }

        cases[i] = thisCase;
    }

    fclose (file);
    return cases;
}



// writers
void writeOutput(vector<long> &answers){
    for(int i = 0; i < answers.size(); i++){
        if(answers[i] != -1){
            printf("Case #%d: %li\n", i+1, answers[i]);
        }else{
            printf("Case #%d: impossible\n", i+1);
        }
    }
}

void writeOutputFile(vector<long> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < answers.size(); i++){
        if(answers[i] != -1){
            fprintf(file, "Case #%d: %li\n", i+1, answers[i]);
        }else{
            fprintf(file, "Case #%d: impossible\n", i+1);
        }
    }

    fclose(file);
}