#ifndef __TETRIMINO_H__
#define __TETRIMINO_H__
#include <iostream>
//#include "board.h"

class Board;
class Tetrimino{
   int x[4];      //x coordinates of the 4 blocks
   int y[4];      //y coordinates of the 4 blocks
   int dir;		   //direction
   bool state;		//state of the tetrimino
   char block;		//block character
   int numBlocks;	//number of alive blocks 
   int level;		//level at which this block was created
   bool transparent;
   bool skippable;
   public:
      Tetrimino(const char &, Board *,const int &, const bool &, const bool &);
      //character, board, level, transparent, skippable
      void update(int *, int *);
      int * get(const char &);
      void rotate(const int &, const int &, Board *);
      friend void rotateRaw(const char &, int &,const int&, const int &, int *, int *);
      void move(const int &, Board *);
      void updateDir(const int &);
      int getAlive();
      void killBlock(const int &);
      int getDir();
      int getLevel();
      void clear(Board *);
      void notifyBoard(Board *);
      void die();
      bool getSkip();
      bool getTrans();
      char whatBlock();
      ~Tetrimino();
};

#endif

