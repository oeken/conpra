#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Case {
    int n; // board size
    int** board; // board
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
    vector<vector<int>> board(thisCase.n);
    for(int i = 0; i < thisCase.n; i++){
        vector<int> v(thisCase.n);
        board[i] = v;
    }

    for(int i = 0; i < thisCase.n; i++){ // for each row
        for(int j = 0; j < thisCase.n; j++){ // for each col
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

bool isColSafe(vector<vector<int>> &board, int row, int col){
    // check wrt col
    for (int i = row-1; i >= 0; i--)
        if (board[i][col])
            return false;
    return true;
}

bool isRowSafe(vector<vector<int>> &board, int row, int col){
    // check wrt row
    for (int i = col-1; i >= 0; i--)
        if (board[row][i])
            return false;
    return true;
}

bool isUpDiagSafe(vector<vector<int>> &board, int row, int col){
    // check wrt upper diag
    int i, j;
    for (i=row-1, j=col-1; i>=0 && j>=0; i--, j--)
        if (board[i][j])
            return false;
    return true;
}

bool isDownDiagSafe(vector<vector<int>> &board, int row, int col){
    // check wrt lower diag
    int i, j;
    for (i=row+1, j=col-1; j>=0 && i<board.size(); i++, j--)
        if (board[i][j])
            return false;
    return true;
}

bool isSafe(vector<vector<int>> &board, int row, int col) {
    return isRowSafe(board, row, col) && isUpDiagSafe(board, row, col) && isDownDiagSafe(board, row, col);
}

bool isSafeIncCol(vector<vector<int>> &board, int row, int col) {
    return  isColSafe(board, row, col) &&
            isRowSafe(board, row, col) &&
            isUpDiagSafe(board, row, col) &&
            isDownDiagSafe(board, row, col);
}

bool solveNQueens(vector<vector<int>> &board, int col){
    // base case
    if(col >= board.size()) return true;

    // first case a statically placed queen
    for(int i = 0; i < board.size(); i++){
        if(board[i][col]){
            if(isSafe(board, i, col)){
                return solveNQueens(board, col+1);
            }else{
                return false;
            }
        }
    }

    // second case a dynamically placed queen
    for(int i = 0; i < board.size(); i++){
        if(isSafe(board, i, col)){
            board[i][col] = 1; // place a queen here since it is safe
            if(solveNQueens(board, col+1)){
                return true; // cool, we could solve it
            }else{
                board[i][col] = 0; // take the queen back
            }
        }
    }

    // didnt reach the end, there is not static queen, there is no safe place
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
    // get the board
    vector<vector<int>> board = getBoard(thisCase);

    // if currently it's legal --> return impossible
    for(int i = 0; i < thisCase.n; i++){
        for(int j = 0; j < thisCase.n; j++){
            if(board[i][j] && !isSafeIncCol(board, i, j))
                return impossible(thisCase.n);
        }
    }

    // start from leftmost column
    bool nqueens = solveNQueens(board, 0);
    if(nqueens) return board;
    else return impossible(thisCase.n);

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
        scanf("%d", &thisCase.n);
        scanf("%c", &dummy); // skip |n

        thisCase.board = new int*[thisCase.n];

        for(int j = 0; j < thisCase.n; j++){
            thisCase.board[j] = new int[thisCase.n];
            for(int k = 0; k < thisCase.n; k++){
                scanf("%c", &dummy);
                if(dummy == 'x'){
                    thisCase.board[j][k] = 1;
                }else{
                    thisCase.board[j][k] = 0;
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
        fscanf(file, "%d", &thisCase.n);
        fscanf(file, "%c", &dummy); // skip |n

        thisCase.board = new int*[thisCase.n];

        for(int j = 0; j < thisCase.n; j++){
            thisCase.board[j] = new int[thisCase.n];
            for(int k = 0; k < thisCase.n; k++){
                fscanf(file, "%c", &dummy);
                if(dummy == 'x'){
                    thisCase.board[j][k] = 1;
                }else{
                    thisCase.board[j][k] = 0;
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
void writeOutput(vector<vector<vector<int>>> &answers){
    for(int i = 0; i < answers.size(); i++){
        printf("Case #%d:\n", i+1);
        if(answers[i][0][0] == -1){
            printf("impossible\n");
        }else{
            for(int j = 0; j < answers[i].size(); j++){ // for each row
                for(int k = 0; k < answers[i].size(); k++){ // for each col
                    if(answers[i][j][k]){
                        printf("x");
                    }else{
                        printf(".");
                    }
                }
                printf("\n");
            }
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
            for(int j = 0; j < answers[i].size(); j++){ // for each row
                for(int k = 0; k < answers[i].size(); k++){ // for each col
                    if(answers[i][j][k]){
                        fprintf(file, "x");
                    }else{
                        fprintf(file, ".");
                    }
                }
                fprintf(file, "\n");
            }
        }
    }

    fclose(file);
}