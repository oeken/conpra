#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <climits>
#include <stack>

using namespace std;

//class UnionFind {
//
//private:
//    int countElem; // # of elems
//    int countSet; // # of sets
//    int* ids; // sets/ids of each element
//    int* sizes; // size of each set/id, notice: there are redundant values (after merge operations)
//
//public:
//
//    // Create an empty union find data structure with N isolated sets.
//    UnionFind(int N) {
//        countElem = N;
//        countSet = N;
//        ids = new int[N];
//        sizes = new int[N];
//        for (int i = 0; i<N; i++) {
//            ids[i] = i, sizes[i] = 1;
//        }
//    }
//
//    ~UnionFind() {
//        delete[] ids;
//        delete[] sizes;
//    }
//
//    // Return the id of component corresponding to object p.
//    int find(int p) {
//        int element = p;
//
//        // find the root
//        while (element != ids[element]) element = ids[element];
//
//        // now element is root, compress the path
//        while (p != element) {
//            int newp = ids[p];
//            ids[p] = element;
//            p = newp;
//        }
//
//        return element;
//    }
//
//    // Replace sets containing x and y with their union.
//    void merge(int x, int y) {
//        int i = find(x);
//        int j = find(y);
//        if (i == j) return;
//
//        // make smaller root point to larger one
//        if (sizes[i] < sizes[j]) {
//            ids[i] = j;
//            sizes[j] += sizes[i];
//        }
//        else {
//            ids[j] = i;
//            sizes[i] += sizes[j];
//        }
//        countSet--;
//    }
//
//    // Are objects x and y in the same set?
//    bool connected(int x, int y) { return find(x) == find(y); }
//
//    // Return the number of disjoint sets.
//    int getCountSet() { return countSet; }
//
//    int getMaxSize(){
//        int rv = INT_MIN;
//        for(int i = 0; i < countElem; i++){
//            if(sizes[i] > rv) rv = sizes[i];
//        }
//        return rv;
//    }
//
//    int getSize(int x){
//        return sizes[x];
//    }
//};

struct Relation {
    int c1;
    int c2;
    char type;
};

struct Case {
    int countryCount;
    int relationCount;

    Relation* relations;
};

struct Stance {
    int country;
    char type;
};


// readers
vector<Case> readInput();
vector<Case> readInputFile();

// writers
void writeOutput(vector<bool> &answers);
void writeOutputFile(vector<bool> &answers);

// helpers
vector<bool> solveProblem(vector<Case> &cases);
bool solveCase(Case &thisCase);
string relationToString(Relation &relation);
void addRelation(Relation r, vector<Relation> &rList, unordered_map<string, bool> &rTable, unordered_map<int, vector<Stance>> &cbList);

string inFileName = "in.txt";
string outFileName = "out.txt";


int main() {
//    vector<Case> cases = readInputFile();
    vector<Case> cases = readInput();

    vector<bool> answers = solveProblem(cases);

    writeOutput(answers);
//    writeOutputFile(answers);

    return 0;
}



// helpers
vector<bool> solveProblem(vector<Case> &cases){
    vector<bool> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}


bool solveCase(Case &thisCase){
//    todo implement

    // form the graph
    vector<vector<Stance>> graph(thisCase.countryCount);
    for(int i = 0; i < thisCase.relationCount; i++){
        Relation r = thisCase.relations[i];

        Stance s1;
        s1.type = r.type;
        s1.country = r.c2;
        graph[r.c1-1].push_back(s1);

        Stance s2;
        s2.type = r.type;
        s2.country = r.c1;
        graph[r.c2-1].push_back(s2);
    }

    // create DFS utilities
    stack<int> s1; // what node i am on
    stack<char> s2; // what path i followed
    stack<bool> s3; // is the node a friend

    vector<bool> visited(thisCase.countryCount);
    int coalitionSize = 0;
    bool backtrack = false;
    bool isFriend;

    s1.push(1);
    s2.push('F');
    s3.push(true);

    while(!s1.empty()){
        int current = s1.top();

        if(!backtrack){ // going forward
            isFriend = s3.top() && s2.top() == 'F' || !s3.top() && s2.top() == 'A';

            if(!visited[current-1] && isFriend) coalitionSize++;
            if(!visited[current-1]) visited[current-1]= true;

            backtrack = true; // assume we wont find a new guy to go, we must backtrack
            for(int i = 0; i < graph[current-1].size(); i++){ // search for a new guy to go
                Stance s = graph[current-1][i];
                if(!visited[s.country-1]){
                    s1.push(s.country); // what a luck we found it!
                    s2.push(s.type);
                    s3.push(isFriend);
                    backtrack = false;
                    break;
                }
            }

        }else{ // make a step back
            s1.pop();
            s2.pop();
            s3.pop();
            backtrack = false;
        }
    }

    return coalitionSize> ((double)thisCase.countryCount/2);

}


void addRelation(Relation r, vector<Relation> &rList, unordered_map<string, bool> &rTable, unordered_map<int, vector<Stance>> &cbList){
    rList.push_back(r); // 1 to easily iterate over
    rTable[relationToString(r)] = true; // 2 to easily check existence

    Stance s1;
    s1.type = r.type;
    s1.country = r.c2;
    cbList[r.c1].push_back(s1); // 3 to easily find triangles

    Stance s2;
    s2.type = r.type;
    s2.country = r.c1;
    cbList[r.c2].push_back(s2); // 3 to easily find triangles
}

string relationToString(Relation &relation){
    string rv("");
    rv += relation.type;
    rv += ",";
    if(relation.c1 < relation.c2){
        rv += to_string(relation.c1) + "," + to_string(relation.c2);
    }else{
        rv += to_string(relation.c2) + "," + to_string(relation.c1);
    }
    return rv;
}


// readers
vector<Case> readInput(){
    // read # of cases
    int caseCount;
    scanf("%d", &caseCount);

    // read each case
    char dummy;
    vector<Case> cases(caseCount);
    for(int i = 0; i < caseCount; i++){
        Case thisCase;

        scanf("%d", &thisCase.countryCount);
        scanf("%d", &thisCase.relationCount);


        thisCase.relations = new Relation[thisCase.relationCount];
        for(int j = 0; j < thisCase.relationCount; j++){
            Relation relation;

            scanf("%c", &dummy); // proceed to next line
            scanf("%c", &relation.type);
            scanf("%d", &relation.c1);
            scanf("%d", &relation.c2);
            thisCase.relations[j] = relation;
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
    char dummy;
    vector<Case> cases(caseCount);
    for(int i = 0; i < caseCount; i++){
        Case thisCase;

        fscanf(file, "%d", &thisCase.countryCount);
        fscanf(file, "%d", &thisCase.relationCount);


        thisCase.relations = new Relation[thisCase.relationCount];
        for(int j = 0; j < thisCase.relationCount; j++){
            Relation relation;

            fscanf(file, "%c", &dummy); // proceed to next line
            fscanf(file, "%c", &relation.type);
            fscanf(file, "%d", &relation.c1);
            fscanf(file, "%d", &relation.c2);
            thisCase.relations[j] = relation;
        }
        cases[i] = thisCase;
    }

    fclose (file);
    return cases;
}



// writers
void writeOutput(vector<bool> &answers){
    for(int i = 0; i < answers.size(); i++){
        if(answers[i]){
            printf("Case #%d: %s\n", i+1, "yes");
        }else{
            printf("Case #%d: %s\n", i+1, "no");
        }
    }
}

void writeOutputFile(vector<bool> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < answers.size(); i++){
        if(answers[i]){
            fprintf(file, "Case #%d: %s\n", i+1, "yes");
        }else{
            fprintf(file, "Case #%d: %s\n", i+1, "no");
        }
    }

    fclose(file);
}