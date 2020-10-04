#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <string.h>

const int _size = 14;
const char _charArray[4] = { 'R', 'G', 'B', 'Y' };
int _stepsCounter = 0;
int _maxSteps = 22;

using namespace std;

//cell data structure (piece)
struct Cell {
    bool toBeFlooded;
    char color;
    Cell* right;
    Cell* left;
    Cell* up;
    Cell* down;
};

//2d array to hold game board
Cell _board[_size][_size];

//array to hold all cells to be flooded (color changing)
Cell *_toFlood[_size*_size];

//populate board with memory addresses
/*
void allocateBoardMemory(){
    for (int i=0; i <_size; i++) {
        _board[i] = (Cell**)malloc(_size*sizeof(Cell));
        for (int j=0; j<_size; j++){
            _board[i][j] = new (Cell);
            _board[i][j]->toBeFlooded = false;
            _board[i][j]->color = '\0';
            _board[i][j]->right = nullptr;
            _board[i][j]->left = nullptr;
            _board[i][j]->up = nullptr;
            _board[i][j]->down = nullptr;
        }
    }
}
 */

void addToFloodArray(Cell *c){
    for(int i=0; i<_size*_size; i++){
        if(!_toFlood[i]){
            _toFlood[i] = c;
            break;
        }
    }
}

bool isInFloodArray(Cell *c){
    for(int i=0; i<_size*_size; i++){
        if(_toFlood[i] == c){
            return true;
        }
    }
    return false;
}

//Return a random char from list
char getRandChar() {
    return _charArray[rand() % 4];
}

//print board
void printBoard() {
    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _size; j++) {
            char c = _board[i][j].color;
            switch (c) {
                case ('R'):
                    cout << "\033[1;31m" << c << "\033[0m" << " ";
                    break;
                case ('G'):
                    cout << "\033[1;32m" << c << "\033[0m" << " ";
                    break;
                case ('B'):
                    cout << "\033[1;34m" << c << "\033[0m" << " ";
                    break;
                case ('Y'):
                    cout << "\033[1;33m" << c << "\033[0m" << " ";
                    break;
                default:
                    cout << c << " ";
                    break;
            }
        }
        cout << "\n";
    }
}

//populate the board and assign neighbors
void populateBoard() {
    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _size; j++) {
            
            //fill color
            _board[i][j].color = getRandChar();
            
            //check up
            if(i-1>=0){
                _board[i][j].up = &_board[i-1][j];
            }
            
            //check down
            if(i+1<_size){
                _board[i][j].down = &_board[i+1][j];
            }
            
            //check right
            if(j+1<_size){
                _board[i][j].right = &_board[i][j+1];
            }
            
            //check left
            if(j-1>=0){
                _board[i][j].left = &_board[i][j-1];
            }
        }
    }
}

char checkCapitalCase(char c){
    //if small letter change to caps
    if(int(c) >= 97 && int(c) <= 122){
        c = '\0' + (int(c)-32);
    }
    return c;
}

bool charInArray(char c){
    for(int i=0; i<strlen(_charArray); i++){
        if(_charArray[i] == c){
            return true;
        }
    }
    return false;
}

char getInput(){
    cout << "Color RGBY: ";
    char c = '\0';
    cin >> c;
    c = checkCapitalCase(c);
    _stepsCounter++;
    return c;
}

void flood(char c){
    for(int i=0; i<_size*_size; i++){
        if(_toFlood[i]){
            _toFlood[i]->color = c;
            _toFlood[i]->toBeFlooded = false;
        }
    }
}

void checkNeighbordsToFlood(Cell *cell, char c){
    if(isInFloodArray(cell)){
        //check cell is marked for flooding or not
        if(!cell->toBeFlooded){
            
            //mark cell ready for flooding
            cell->toBeFlooded = true;
            
            //check neighbords
            if(cell->up){
                checkNeighbordsToFlood(cell->up, c);
            }
            
            if(cell->down){
                checkNeighbordsToFlood(cell->down, c);
            }
            
            if(cell->right){
                checkNeighbordsToFlood(cell->right, c);
            }
            
            if(cell->left){
                checkNeighbordsToFlood(cell->left, c);
            }
        }
    }else{
        if(cell->color == c){
            //add cell to toFloor array
            addToFloodArray(cell);
            
            //mark as ready for flooding
            cell->toBeFlooded = true;
            
            //check neighbords
            if(cell->up){
                checkNeighbordsToFlood(cell->up, c);
            }
            
            if(cell->down){
                checkNeighbordsToFlood(cell->down, c);
            }
            
            if(cell->right){
                checkNeighbordsToFlood(cell->right, c);
            }
            
            if(cell->left){
                checkNeighbordsToFlood(cell->left, c);
            }
        }
    }
}

bool checkGameWinenr(){
    for(int i=0; i<_size; i++){
        for(int j=0; j<_size; j++){
            if(_board[0][0].color != _board[i][j].color){
                return false;
            }
        }
    }
    return true;
}

int main(int argc, const char * argv[]) {
    //get some memory space for gameboard and toFlood arrays
    //allocateBoardMemory();
    //*_toFlood = (Cell*)malloc(_size*_size*sizeof(Cell));
    
    //populate board then print
    populateBoard();
    printBoard();
    
    char c = getInput();
    
    while(charInArray(c) && _stepsCounter < _maxSteps){
        //trigger user action (flood neighbors)
        checkNeighbordsToFlood(&_board[0][0], _board[0][0].color);
        
        //change color for flooded cells
        flood(c);
        
        //print board
        cout <<"\nStep " << _stepsCounter << "/" << _maxSteps << "\n";
        printBoard();
        
        //check board state - later
        if(checkGameWinenr()){
            cout << "Congrats! You've Finished The Game in " << _stepsCounter << " Steps!\n";
            break;
        }
        
        //get new input (increments _stepsCounter)
        c = getInput();
    }
    
    //End Of Game
    printf("Game Over in %d steps\n", _stepsCounter);
    
    return 0;
}

