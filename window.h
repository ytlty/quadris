#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <X11/Xlib.h>
#include <iostream>
#include <string>

class Xwindow {
  Display *d;
  Window w;
  int s;
  GC gc;
  unsigned long colours[30];
  int width, height;

 public:
  Xwindow(int width=500, int height=500);  // Constructor; displays the window.
  ~Xwindow();                              // Destructor; destroys the window.

  //enum {White=0, Black, Red, Green, Blue, Cyan, Yellow, Magenta, Orange, Brown}; // Available colours.
  //Mo' colours Mo' problems
  enum {White=0, Black, 
           ZA, ZB, ZC, //piece|highlight, mid-section, shadow
           SA, SB, SC,
           JA, JB, JC,
           IA, IB, IC,
           OA, OB, OC,
           TA, TB, TC,
           LA, LB, LC,
           CBack,      //canvas background
           //rest of the background based on heat
           RA, RB, RC, RD, RE, RF};
  
  
  // Draws a rectangle
  void fillRectangle(int x, int y, int width, int height, int colour=Black);
  void drawBoard(int heat);
  //draws the canvas & next & everything else
  void drawBlock(int x, int y, char block);
  //draws a block on canvas
  void clearCanvas();
  //undraws a block on canvas
  void drawNext(char piece, bool trans, bool skip, int heat);
  //draws the next piece
  void drawScore(int level, int score, int hiScore, int heat);
  
  // Draws a string
  void drawString(int x, int y, std::string msg, int colour=Black);

  // Draws a string
  void drawBigString(int x, int y, std::string msg, int colour=Black);
};

#endif
