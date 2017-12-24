#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <unordered_map>


using namespace std;

struct Case {
    int n; // number of clocks
    int* d1; // digit 1
    int* d2; // digit 2
    int* d3; // digit 3
    int* d4; // digit 4
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
    vector<Case> cases = readInputFile();
//    vector<Case> cases = readInput();

    vector<vector<int>> answers = solveProblem(cases);

//    writeOutput(answers);
    writeOutputFile(answers);

    return 0;
}


// helpers
vector<vector<int>> see2real(10);
vector<vector<vector<int>>> relaxDiscovery(10);
unordered_map<int, unordered_map<int, int>> intersectTable(3);
void initSee2Real(){
    for(int i = 0; i < 10; i++){
        vector<int> v(10);
        see2real[i] = v;
    }

    see2real[0][0]= 1;
    see2real[0][8]= 1;

    see2real[1][0]= 1;
    see2real[1][1]= 1;
    see2real[1][3]= 1;
    see2real[1][4]= 1;
    see2real[1][7]= 1;
    see2real[1][8]= 1;
    see2real[1][9]= 1;

    see2real[2][2]= 1;
    see2real[2][8]= 1;

    see2real[3][3]= 1;
    see2real[3][8]= 1;
    see2real[3][9]= 1;

    see2real[4][4]= 1;
    see2real[4][8]= 1;
    see2real[4][9]= 1;

    see2real[5][5]= 1;
    see2real[5][6]= 1;
    see2real[5][8]= 1;
    see2real[5][9]= 1;

    see2real[6][6]= 1;
    see2real[6][8]= 1;

    see2real[7][0]= 1;
    see2real[7][3]= 1;
    see2real[7][7]= 1;
    see2real[7][8]= 1;
    see2real[7][9]= 1;

    see2real[8][8]= 1;

    see2real[9][8]= 1;
    see2real[9][9]= 1;

//    see2real[0].push_back(0);
//    see2real[0].push_back(8);
//
//    see2real[1].push_back(0);
//    see2real[1].push_back(1);
//    see2real[1].push_back(3);
//    see2real[1].push_back(4);
//    see2real[1].push_back(7);
//    see2real[1].push_back(8);
//    see2real[1].push_back(9);
//
//    see2real[2].push_back(2);
//    see2real[2].push_back(8);
//
//    see2real[3].push_back(3);
//    see2real[3].push_back(8);
//    see2real[3].push_back(9);
//
//    see2real[4].push_back(4);
//    see2real[4].push_back(8);
//    see2real[4].push_back(9);
//
//    see2real[5].push_back(5);
//    see2real[5].push_back(6);
//    see2real[5].push_back(8);
//    see2real[5].push_back(9);
//
//    see2real[6].push_back(6);
//    see2real[6].push_back(8);
//
//    see2real[7].push_back(0);
//    see2real[7].push_back(3);
//    see2real[7].push_back(7);
//    see2real[7].push_back(8);
//    see2real[7].push_back(9);
//
//    see2real[8].push_back(8);
//
//    see2real[9].push_back(8);
//    see2real[9].push_back(9);
}

void initRelaxedDiscovery(){
    for(int i = 0; i < 10; i++){
        vector<vector<int>> v(10);
        relaxDiscovery[i] = v;
    }

    // int see, int real
    // 0
    relaxDiscovery[0][0] = {1, 1, 1, -1, 1, 1, 1};
    relaxDiscovery[0][8] = {1, 1, 1, 0, 1, 1, 1};

    // 1
    relaxDiscovery[1][0]={0,0,1,-1,0,1,0};
    relaxDiscovery[1][1]={-1,-1,1,-1,-1,1,-1};
    relaxDiscovery[1][3]={0,-1,1,0,-1,1,0};
    relaxDiscovery[1][4]={-1,0,1,0,-1,1,-1};
    relaxDiscovery[1][7]={0,-1,1,-1,-1,1,-1};
    relaxDiscovery[1][8]={0,0,1,0,0,1,0};
    relaxDiscovery[1][9]={0,0,1,0,-1,1,0};

    //,2
    relaxDiscovery[2][2]={1,-1,1,1,1,-1,1};
    relaxDiscovery[2][8]={0,0,1,0,1,0,0};

    //,3
    relaxDiscovery[3][3]={1,-1,1,1,-1,1,1};
    relaxDiscovery[3][8]={1,0,1,1,0,1,1};
    relaxDiscovery[3][9]={1,0,1,1,-1,1,1};

    //,4
    relaxDiscovery[4][4]={-1,1,1,1,-1,1,-1};
    relaxDiscovery[4][8]={0,1,1,1,0,1,0};
    relaxDiscovery[4][9]={0,1,1,1,-1,1,0};

    //,5
    relaxDiscovery[5][5]={1,1,-1,1,-1,1,1};
    relaxDiscovery[5][6]={1,1,-1,1,0,1,1};
    relaxDiscovery[5][8]={1,1,0,1,0,1,1};
    relaxDiscovery[5][9]={1,1,0,1,-1,1,1};

    //,6
    relaxDiscovery[6][6]={1,1,-1,1,1,1,1};
    relaxDiscovery[6][8]={1,1,0,1,1,1,1};

    //,7
    relaxDiscovery[7][0]={1,0,1,-1,0,1,0};
    relaxDiscovery[7][3]={1,-1,1,0,-1,1,0};
    relaxDiscovery[7][7]={1,-1,1,-1,-1,1,-1};
    relaxDiscovery[7][8]={1,0,1,0,0,1,0};
    relaxDiscovery[7][9]={1,0,1,0,-1,1,0};

    //,8
    relaxDiscovery[8][8]={1,1,1,1,1,1,1};

    //,9
    relaxDiscovery[9][8]={1,1,1,1,0,1,1};
    relaxDiscovery[9][9]={1,1,1,1,-1,1,1};
}

void initIntersectTable(){

    // so far - new info
    intersectTable[0][0] = 0;
    intersectTable[0][1] = -11;
    intersectTable[0][-1] = 0;

    intersectTable[1][0] = -11;
    intersectTable[1][1] = 1;
    intersectTable[1][-1] = 1;

    intersectTable[-1][0] = 0;
    intersectTable[-1][1] = 1;
    intersectTable[-1][-1] = -1;
}


void intersect(vector<int> &discovery, vector<int> &relaxPart, int index){
    for(int i = 0; i < 7; i++){
        int i1 = discovery[index*7+i];
        int i2 = relaxPart[i];
        discovery[index*7+i] = intersectTable[i1][i2];
    }
}

vector<int> updateDiscovery(int d1, int d2, int d3, int d4,
                     int o1, int o2, int o3, int o4,
                     vector<int> discovery){

    intersect(discovery, relaxDiscovery[d1][o1], 0);
    intersect(discovery, relaxDiscovery[d2][o2], 1);
    intersect(discovery, relaxDiscovery[d3][o3], 2);
    intersect(discovery, relaxDiscovery[d4][o4], 3);

    return discovery;
}

vector<int> oneMinuteLater(int d1, int d2, int d3, int d4){
    int left = d1 * 10 + d2;
    int right = d3 * 10 + d4;
    right = (right+1) % 60;
    if(!right) left = (left+1) % 24;

    int new1 = left / 10;
    int new2 = left % 10;
    int new3 = right / 10;
    int new4 = right % 10;

    vector<int> rv({new1, new2, new3, new4});
    return rv;
}

bool timesContain(vector<int> &times, int d1, int d2, int d3, int d4){
    for(int i = 0; i < times.size()/4; i++){
        if(times[i*4] == d1 && times[i*4+1] == d2 && times[i*4+2] == d3 && times[i*4+3] == d4) return true;
    }
    return false;
}

vector<int> fetchPossibleTimes(int d1, int d2, int d3, int d4, int errordigit){
    vector<int> rv;

    switch(errordigit){
        case 1:
            for(int i = 0; i <= 2; i++){
                if(d1 != i && see2real[d1][i]){
                    rv.push_back(i);
                    rv.push_back(d2);
                    rv.push_back(d3);
                    rv.push_back(d4);
                }
            }
            break;

        case 2:
            for(int i = 0; i <= 9; i++){
                if(d2 != i && see2real[d2][i]){
                    rv.push_back(d1);
                    rv.push_back(i);
                    rv.push_back(d3);
                    rv.push_back(d4);
                }
            }
            break;

        case 3:
            for(int i = 0; i <= 5; i++){
                if(d3 != i && see2real[d3][i]){
                    rv.push_back(d1);
                    rv.push_back(d2);
                    rv.push_back(i);
                    rv.push_back(d4);
                }
            }
            break;

        case 4:
            for(int i = 0; i <= 9; i++){
                if(d4 != i && see2real[d4][i]){
                    rv.push_back(d1);
                    rv.push_back(d2);
                    rv.push_back(d3);
                    rv.push_back(i);
                }
            }
            break;
    }

    return rv;
    // todo fetchpossibletimes should only output valid times, changed idea, not anymore
}

bool timeIsValid(int d1, int d2, int d3, int d4){
    int left = 10 * d1 + d2;
    int right = 10 * d3 + d4;
    bool leftgood = left >= 0 && left <= 23;
    bool rightgood = right >= 0 && right <= 59;
    return leftgood && rightgood;
}

vector<int> inferTimes(int index, vector<int> discovery, Case &thisCase){
    vector<int> rv;

    // fetch current time
    int d1 = thisCase.d1[index];
    int d2 = thisCase.d2[index];
    int d3 = thisCase.d3[index];
    int d4 = thisCase.d4[index];

    if(!timeIsValid(d1,d2,d3,d4)) return rv;

    // assume nothing is broken as the first case, i.e. what it shows is correct
    vector<int> dc1 = updateDiscovery(d1, d2, d3, d4, d1, d2, d3, d4, discovery);
    if(find(dc1.begin(), dc1.end(), -11) == dc1.end()){ // if hypothesis works
        if(index == thisCase.n-1){ // base case
            rv.push_back(d1);
            rv.push_back(d2);
            rv.push_back(d3);
            rv.push_back(d4);
        }else{ // there are more to explore
            vector<int> futuretimes = inferTimes(index+1, dc1, thisCase);

            vector<int> oneminlater = oneMinuteLater(d1,d2,d3,d4);
            int l1 = oneminlater[0];
            int l2 = oneminlater[1];
            int l3 = oneminlater[2];
            int l4 = oneminlater[3];

            if(timesContain(futuretimes, l1, l2, l3, l4)){
                rv.push_back(d1);
                rv.push_back(d2);
                rv.push_back(d3);
                rv.push_back(d4);
            }
        }
    }

    // assume other possible malfunctions
    for(int i = 1; i <= 4; i++){
        vector<int> possibilities = fetchPossibleTimes(d1,d2,d3,d4,i);
        for(int j = 0; j < possibilities.size()/4; j++){
            int c1 = possibilities[j*4];
            int c2 = possibilities[j*4+1];
            int c3 = possibilities[j*4+2];
            int c4 = possibilities[j*4+3];

            if(timeIsValid(c1, c2, c3, c4)){
                vector<int> dc2 = updateDiscovery(d1, d2, d3, d4, c1, c2, c3, c4, discovery);
                if(find(dc2.begin(), dc2.end(), -11) == dc2.end()){ // if hypothesis works
                    if(index == thisCase.n-1){ // base case
                        rv.push_back(c1);
                        rv.push_back(c2);
                        rv.push_back(c3);
                        rv.push_back(c4);
                    }else{ // there are more to explore
//                    updateDiscovery(d1, d2, d3, d4, c1, c2, c3, c4, discovery);
                        vector<int> futuretimes = inferTimes(index+1, discovery, thisCase);

                        vector<int> oneminlater = oneMinuteLater(c1,c2,c3,c4);
                        int l1 = oneminlater[0];
                        int l2 = oneminlater[1];
                        int l3 = oneminlater[2];
                        int l4 = oneminlater[3];

                        if(timesContain(futuretimes, l1, l2, l3, l4)){
                            rv.push_back(c1);
                            rv.push_back(c2);
                            rv.push_back(c3);
                            rv.push_back(c4);
                        }
                    }
                }
            }
        }
    }

    return rv;
}

vector<vector<int>> solveProblem(vector<Case> &cases){
    initSee2Real();
    initRelaxedDiscovery();
    initIntersectTable();
    vector<vector<int>> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}

vector<int> solveCase(Case &thisCase){
    int index = 0; // start from 0th time given
    vector<int> discovery(28);
    for(int i = 0; i<28; i++) discovery[i] = -1; // initially both case is possible for all lights

    vector<int> fits = inferTimes(index, discovery, thisCase);
    return fits;
}


// readers
vector<Case> readInput(){
    // read # of cases
    char dummy = '-'; // dummy shit
    int t;
    scanf("%d", &t);

    // read each case
    vector<Case> cases(t);
    for(int i = 0; i < t; i++){
        Case thisCase;
        scanf("%d", &thisCase.n);

        thisCase.d1 = new int[thisCase.n];
        thisCase.d2 = new int[thisCase.n];
        thisCase.d3 = new int[thisCase.n];
        thisCase.d4 = new int[thisCase.n];

        for(int j = 0; j < thisCase.n; j++){
            int left;
            int right;
            scanf("%d", &left);
            scanf("%c", &dummy); // remove ':'
            scanf("%d", &right);
            thisCase.d1[j] = left / 10;
            thisCase.d2[j] = left % 10;

            thisCase.d3[j] = right / 10;
            thisCase.d4[j] = right % 10;
        }
        cases[i] = thisCase;
    }

    return cases;
}

vector<Case> readInputFile(){
    FILE* file;
    file = fopen (inFileName.c_str(),"r");

    // read # of cases
    char dummy = '-'; // dummy shit
    int t;
    fscanf(file, "%d", &t);

    // read each case
    vector<Case> cases(t);
    for(int i = 0; i < t; i++){
        Case thisCase;
        fscanf(file, "%d", &thisCase.n);

        thisCase.d1 = new int[thisCase.n];
        thisCase.d2 = new int[thisCase.n];
        thisCase.d3 = new int[thisCase.n];
        thisCase.d4 = new int[thisCase.n];

        for(int j = 0; j < thisCase.n; j++){
            int left;
            int right;
            fscanf(file, "%d", &left);
            fscanf(file, "%c", &dummy); // remove ':'
            fscanf(file, "%d", &right);
            thisCase.d1[j] = left / 10;
            thisCase.d2[j] = left % 10;

            thisCase.d3[j] = right / 10;
            thisCase.d4[j] = right % 10;
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
        if(answers[i].empty()){
            printf("none\n");
        }else{
            for(int j = 0; j < answers[i].size()/4; j++){ // for each clock
                printf("%d%d:%d%d\n", answers[i][j*4], answers[i][j*4+1], answers[i][j*4+2], answers[i][j*4+3]);
            }
        }
    }
}

void writeOutputFile(vector<vector<int>> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < answers.size(); i++){
        fprintf(file, "Case #%d:\n", i+1);
        if(answers[i].empty()){
            fprintf(file, "none\n");
        }else{
            for(int j = 0; j < answers[i].size()/4; j++){ // for each clock
                fprintf(file, "%d%d:%d%d\n", answers[i][j*4], answers[i][j*4+1], answers[i][j*4+2], answers[i][j*4+3]);
            }
        }
    }

    fclose(file);
}