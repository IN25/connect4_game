//Create a Connect 4 Game
#include <iostream>
#include <string>

using namespace std;

//Global constants for the game pieces
const char EMP = '-';
const char P1 = 'X';
const char P2 = 'O';

//global variables/constants for the game board
const int COLS = 7;
const int ROWS = 6;
char board[COLS][ROWS]; //2D array of 7x6 connect 4 board

//Initialize the board to be empty
void initializeBoard();

//Display the board
void displayBoard();

//Get column for current move
int getColumn(bool isPlayer1);

//Get row to check for horizontal win
int getRow(char col[], char currTile);

//Add one piece to the Board
bool addPiece(int c, bool isPlayer1);

//Check if a given player has won
bool isWinner(bool isPlayer1, int lastCol);

//if a program doesnt find X or O at a position, this function is executed, 
//because we need to start looping from rows = 5 again
bool diagonalWinner1(int row, int counter, int column, char currTile, bool alter_c);

//if a program doesnt find X or O at a position, this function is executed, 
//because we need to start looping from rows = 5 again
bool diagonalWinner2(int row, int counter, int column, char currTile, bool alter_c);

int main() {
  //Initialize game
  //TO DO: Add code to randomize first player
  int totalMoves = 0; //number of moves by both players
  const int MAX_MOVES = COLS * ROWS; //total cells on the board
  bool playerOne = true; //boolean to keep track of current player
  bool gameWon = false; //boolean to keep track if game won
  int column = -1;

  initializeBoard();
  displayBoard();

  //Game play loop
  //loop until game is won or the board is full
  while ((!gameWon) && (totalMoves < MAX_MOVES)) {
    bool validMove = false;

    while (!validMove) {
      column = getColumn(playerOne);
      validMove = addPiece(column, playerOne);
      if (!validMove) {
        cout << "Invalid move. Try again." << endl;
      }
    }

    totalMoves++;
    displayBoard();

    //Check if game is won
    gameWon = isWinner(playerOne, column);
    if (gameWon) {
      cout << "CONGRATULATIONS Player " << (playerOne ? "1" : "2")
           << ". You've won Connect 4!!!!" << endl;
    } else if (totalMoves == MAX_MOVES) {
      cout << "Game over! No moves remaining." << endl;
    } else {
      playerOne = !playerOne; //switch player
    }
  }

  return 0;
}

void initializeBoard() {
  //Loops through each column
  for (int c = 0; c < COLS; c++) {
    //Loop through each row in a given column
    for (int r = 0; r < ROWS; r++){
      board[c][r] = EMP; //initialize all cells to empty
    }
  }
}

void displayBoard() {
  cout << endl << "Connect 4 Board:" << endl;
  //Display the board one row at a time
  for (int r = 0; r < ROWS; r++) {
    //For each row display all of the columns
    for (int c = 0; c < COLS; c++) {
      cout << board[c][r] << " ";
    }
    cout << endl; //After each row output a newline character
  }
  //Now display the column numbers below the board
  for (int c = 0; c < COLS; c++) {
    cout << c << " ";
  }
  cout << endl << endl;
}

int getColumn(bool isPlayer1) {
  int col = 0;

  //The below is a short form of above output
  cout << "Player " << (isPlayer1 ? "1" : "2")
       << " - Select a column [0," << (COLS-1) <<  "] to play: ";
  cin >> col;
  return col;
}



bool addPiece(int c, bool isPlayer1) {
  //check if column is valid for board
  if ((c < 0) || (c >= COLS)) { 
    return false;
  }
  //check if column c has space
  if (board[c][0] == EMP) {
    //add a piece to the lowest unoccupied row in column c
    for (int r = (ROWS-1); r >= 0; r--) {
      if (board[c][r] == EMP) {
        //add piece
        if (isPlayer1) {
          board[c][r] = P1;
        } else {
          board[c][r] = P2;
        }
        return true;
      }
    }
  }
  //if column is full return false
  return false;
}

bool isWinner(bool isPlayer1, int lastCol) {
  int counter;
  //Identify which tile is used by current player
  char currTile;
  if (isPlayer1) {
    currTile = P1;
  } else {
    currTile = P2;
  }

  //this function return a number of row at which currTile is
  int rowNum = getRow(board[lastCol], currTile);


  //Check if current player has 4 vertical tiles in a row in the current column
  counter = 0;
  for (int r = ROWS-1; r >= 0; r--) {
    if (board[lastCol][r] == currTile) {
      counter++;
      if (counter == 4) {
        return true;
      }
    } else if (board[lastCol][r] == EMP) { //an optimization to stop searching
      break;                               //if no more tiles in columns
    }
    else {
      counter = 0;
    }
  }


  //Check if current player has 4 horizontal tiles in a row
  counter = 0;
  //loop through the columns
  for (int c = 0; c <= (COLS - 1); c++) {
    // cout << "c = " << c << endl;
    // cout << "rowNum = " << rowNum << endl;
    //check if position on board equals to X or O and increment counter
    if (board[c][rowNum] == currTile) {
      counter++;
      // cout << "counter = " << counter << endl;
      if (counter == 4) {
        return true;
      }
    }
    else {                                            
    counter = 0;
    }
  }


  //Check if current player has 4 diagonal tiles in a row 

  //case 1
  counter = 0;
  int column = 0;
  int rows = ROWS - 1;
  bool alter_c = false;
  //loop through the rows
  for (int r = ROWS-1; r >= 0; r--) {
    //this condition is needed if a position on board does not occupied by X or O
    //we need to start looking from the next column
    if (alter_c) {
      column++;
    }
    //loop through the columns
    for (int c = column; c <= COLS - 1; c++) {
      if (board[c][r] == currTile) {
        counter++;
        if (counter == 4) {
          return true;
        //this else condition is needed for decrementing a row if program didn't 
        //find 4 tiles in diagonal
        } else {
          r--;
        }
      } else {
      counter = 0; 
      alter_c = true;
      //if a program doesnt find X or O at a position, this function is executed, 
      //because we need to start looping from rows = 5 again
      bool diagonalWin = diagonalWinner1(rows, counter, column, currTile, alter_c);
      //if this function  returns true, it means that diagonal won has been detected
      if (diagonalWin == true) {
        return true;
      }
      break;
      }
    }  
  }


  //second case of diagonal
  counter = 0;
  column = 6;
  rows = ROWS - 1;
  alter_c = false;
  //loop through the rows
  for (int r = ROWS-1; r >= 0; r--) {
    //this condition is needed if a position on board does not occupied by X or O
    //we need to start looking from the previous column
    if (alter_c) {
      column--;
    }
    //loop through the columns
    for (int c = column; c > 0; c--) {
      if (board[c][r] == currTile) {
        counter++;
        if (counter == 4) {
          return true;
        //this else condition is needed for decrementing a row if program didn't 
        //find 4 tiles in diagonal
        } else {
          r--;
        }
      } else {
      counter = 0; 
      alter_c = true;
      //if a program doesnt find X or O at a position, this function is executed, 
      //because we need to start looping from rows = 5 again
      bool diagonalWin = diagonalWinner2(rows, counter, column, currTile, alter_c);
      //if this function  returns true, it means that diagonal won has been detected
      if (diagonalWin == true) {
        return true;
      }
      break;
      }
    }  
  }

  //otherwise no winner
  return false;
}


//Get row to check for horizontal win
int getRow(char col[], char currTile) {
  //loop through columns
  for (int i = (COLS - 1); i >= 0; i--) {
    //if tile is found at a particular row, return the number of that row
    if (col[i] == currTile) {
      return i;
    }
  }
}

bool diagonalWinner1(int row, int counter, int column, char currTile, bool alter_c) {
  //loop thorough the rows
  for (int r = ROWS-1; r >= 0; r--) {
    //increment column if no position has been occupied
    if (alter_c) {
      column++;
    }
    //loop thorough the columns
    for (int c = column; c <= COLS - 1; c++) {
      if (board[c][r] == currTile) {
        counter++;
        if (counter == 4) {
          return true;
        //this else condition is needed for decrementing a row if program didn't 
        //find 4 tiles in diagonal
        } else {
          r--;
        }
      } else {
      counter = 0; 
      alter_c = true;
      break;
      }
    }  
  }
}

bool diagonalWinner2(int row, int counter, int column, char currTile, bool alter_c) {
  //loop through the rows
  for (int r = ROWS-1; r >= 0; r--) {
    //increment column if no position has been occupied
    if (alter_c) {
      column--;
    }
    //loop through the columns
    for (int c = column; c > 0; c--) {
      if (board[c][r] == currTile) {
        counter++;
        if (counter == 4) {
          return true;
        //this else condition is needed for decrementing a row if program didn't 
        //find 4 tiles in diagonal
        } else {
          r--;
        }
      } else {
      counter = 0; 
      alter_c = true;
      break;
      }
    }  
  }
}
