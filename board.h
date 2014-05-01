#ifndef __BOARD_H__
#define __BOARD_H__
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "tetrimino.h"
#include <vector>
#include "PRNG.h"
#include "window.h"
class Board{
   static Board * instance;
	 //singleton pattern
   int x,y;
	 //board size
   int score; //current score
   int highscore; //highscore
   int level; //current level
   std::vector<Tetrimino*> deadpieces;
   char ** gameBoard; //the board to be displayed
   Tetrimino * current; //active piece
   char next; //next piece;
   bool active; //does the board exist?
   bool tOnly; //text-only
   std::vector<char> pieces; //all the possible pieces
   std::string myFiles[15]; //level file names
   PRNG prng;  //the pseudo random number generator
   int startingLevel; //starting level backup for restarting the game
   unsigned int seed; //random seed
   char nextGrid[2][4]; //for displaying the next piece;
   bool npT; //next is transparent?
   bool npS; //next is skippable?
   
   int oldHeat; //old heat number
   Xwindow * GDisp; //graphics YEAHHHH
   
   static void cleanup();
   
 public:
    Board(const bool &); //basic constructor, set NULL and whateves
    void setZero(const std::string &);
    //sets the level 0 file
    void lvlU(); //level up
    void lvlD(); //level down    void changePieces();
    //loads the file containing pieces according to level
    //this changes the set of pieces to be picked
    void updateNext();
    //randomly updates the next block & randomizes it's properties
    void start(const int &, const int &,const int &,const unsigned int &); 
    //this generates a n*m board, sets starting level, sets random seed
    void rotateBlock(const int &,const int &);
    //rotates current block
    void move(const int &);
    //this moves the current piece
    bool moveLR(int* , int* , int);
    //this checks for collision left and right
    bool moveD(int* , int*);
    //this checks for collision down
    void restart();
    void lose();
    void skip();
    //skips the piece if possible
    void dropPiece();
    //this drops a piece to the topleft of the board depending on 
    //the piece
    bool rotatePossible(const char &, int &, const int &, const int &, int *, int *);
    //checks if it's possible to rotate;
    void cleanBoard();
    //this clears the whole board, i.e. deletes it
    void detect();
    //this runs through the board to see if there are any filled 
    //rows
    void checkCurrent();
    //checks the current, if NULL, drop new piece & pick next
    bool isActive();
    //checks state
    void death();
    //this is the function that tetrimino calls when the current 
    //piece dies, i.e. reaches bottom
    void checkPieces(const int &);
    //checks through deadpieces to see if any of the blocks had been erased
    void clearLine(const int &); 
    //clears line n by
    //recursively moves the lines down
    static Board *getInstance(const bool &);
    //SINGLETON PATTERN: gets a new board
    void drawG();
    //draws to graphics display
    void notify(const int & , const int &, const char &);
    //turns x,y from ' ' to whatever block is
    friend std::ostream & operator << (std::ostream & , Board & );
    //print the whole board
};
#endif
