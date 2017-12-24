#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Case {
    int** board; // number of planets
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
vector<vector<int>> getBoard(Case &thisCase){
    vector<vector<int>> board(9);
    for(int i = 0; i < 9; i++){
        vector<int> v(9);
        board[i] = v;
    }

    for(int i = 0; i < 9; i++){ // for each row
        for(int j = 0; j < 9; j++){ // for each col
            board[i][j] = thisCase.board[i][j];
        }
    }
    return board;
}

vector<vector<int>> impossible(int n){
    vector<vector<int>> rv(n);
    for(int i = 0; i < n; i++){
        vector<int> v(n);
        rv[i] = v;
    }
    rv[0][0] = -1;
    return rv;
}

bool fitsRow(vector<vector<int>> &board, int row, int col, int num){
    for(int i = 0; i < 9; i++){
        if(i != col && board[row][i] == num) return false;
    }
    return true;
}

bool fitsCol(vector<vector<int>> &board, int row, int col, int num){
    for(int i = 0; i < 9; i++){
        if(i != row && board[i][col] == num) return false;
    }
    return true;
}

bool fitsBox(vector<vector<int>> &board, int row, int col, int num){
    int boxrow = 3*(row/3);
    int boxcol = 3*(col/3);

    for(int i = boxrow; i < boxrow+3; i++){
        for(int j = boxcol; j < boxcol+3; j++){
            if(!(i == row && j == col) && board[i][j] == num) return false;
        }
    }
    return true;
}

bool nextLocation(vector<vector<int>> &board, int* row, int* col){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(!board[i][j]){
                *row = i;
                *col = j;
                return true;
            }
        }
    }
    return false;
}

bool isSafe(vector<vector<int>> &board, int row, int col, int num){
    return fitsRow(board, row, col, num) &&
           fitsCol(board, row, col, num) &&
           fitsBox(board, row, col, num);
}

bool solveSudoku(vector<vector<int>> &board){
    int row;
    int col;
    if(!nextLocation(board, &row, &col)) return true;

    for (int num = 1; num <= 9; num++){

        if(isSafe(board, row, col, num)){ // good to go
            board[row][col] = num; // assign
            if(solveSudoku(board)){
                return true; // cool
            }else{
                board[row][col] = 0; // take it back
            }
        }
    }
    return false;
}

vector<vector<vector<int>>> solveProblem(vector<Case> &cases){
    vector<vector<vector<int>>> answers(cases.size());
    for(int i = 0; i < cases.size(); i++){
        answers[i] = solveCase(cases[i]);
    }
    return answers;
}

vector<vector<int>> solveCase(Case &thisCase){
    vector<vector<int>> board = getBoard(thisCase);

    bool solvesdk = solveSudoku(board);
    if(solvesdk) return board;
    else return impossible(9);
}


// readers
vector<Case> readInput(){
    // read # of cases
    char dummy;
    int t;
    scanf("%d", &t);
    scanf("%c", &dummy); // skip \n

    // read each case
    vector<Case> cases(t);
    for(int i = 0; i < t; i++){
        Case thisCase;
        thisCase.board = new int*[9];

        for(int j = 0; j < 9; j++){
            thisCase.board[j] = new int[9];
            for(int k = 0; k < 9; k++){
                scanf("%c", &dummy);
                if(dummy == '?'){
                    thisCase.board[j][k] = 0;
                }else{
                    thisCase.board[j][k] = dummy - '0';
                }
            }
            if(j != 8) scanf("%c", &dummy); // skip \n
        }

        if(i != t-1){
            scanf("%c", &dummy); // skip \n
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
    fscanf(file, "%c", &dummy); // skip \n

    // read each case
    vector<Case> cases(t);
    for(int i = 0; i < t; i++){
        Case thisCase;
        thisCase.board = new int*[9];

        for(int j = 0; j < 9; j++){
            thisCase.board[j] = new int[9];
            for(int k = 0; k < 9; k++){
                fscanf(file, "%c", &dummy);
                if(dummy == '?'){
                    thisCase.board[j][k] = 0;
                }else{
                    thisCase.board[j][k] = dummy - '0';
                }
            }
            if(j != 8) fscanf(file, "%c", &dummy); // skip \n
        }

        if(i != t-1){
            fscanf(file, "%c", &dummy); // skip \n
            fscanf(file, "%c", &dummy); // skip \n
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
        if(answers[i][0][0] == -1){
            printf("impossible\n");
        }else{
            for(int j = 0; j < 9; j++){
                for(int k = 0; k < 9; k++){
                    printf("%d", answers[i][j][k]);
                }
                printf("\n");
            }
            printf("\n");
        }
    }
}

void writeOutputFile(vector<vector<vector<int>>> &answers){
    FILE* file;
    file = fopen (outFileName.c_str(),"w");

    for(int i = 0; i < answers.size(); i++){
        fprintf(file, "Case #%d:\n", i+1);
        if(answers[i][0][0] == -1){
            fprintf(file, "impossible\n");
        }else{
            for(int j = 0; j < 9; j++){
                for(int k = 0; k < 9; k++){
                    fprintf(file, "%d", answers[i][j][k]);
                }
                fprintf(file, "\n");
            }
            fprintf(file, "\n");
        }
    }

    fclose(file);
}