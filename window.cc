#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <unistd.h>
#include "window.h"

using namespace std;

Xwindow::Xwindow(int width, int height): width(width), height(height) {

  d = XOpenDisplay(NULL);
  if (d == NULL) {
    cerr << "Cannot open display" << endl;
    exit(1);
  }
  s = DefaultScreen(d);
  w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, width, height, 1,
                          BlackPixel(d, s), WhitePixel(d, s));
  XSelectInput(d, w, ExposureMask | KeyPressMask);
  XMapRaised(d, w);

  Pixmap pix = XCreatePixmap(d,w,width,
        height,DefaultDepth(d,DefaultScreen(d)));
  gc = XCreateGC(d, pix, 0,(XGCValues *)0);

  XFlush(d);
  XFlush(d);

  // Set up colours.
  XColor xcolour;
  Colormap cmap;
  //char color_vals[10][10]={"white", "black", "red", "green", "blue", "cyan", "yellow", "magenta", "orange", "brown"};
  //let's get some more colours
  
                           //white   black    
  char color_vals[30][10]={"white", "black", 
                           //Tetrimino colours in x11 colorset, names in standard string, values in hex
                           //highlight, mid-section, shadow
                           //for Z piece 
                           //light coral & crimson & dark red 
                           "#F08080", "#DC143C", "#8B0000",
                           //for S piece 
                           //lime green & forest green & dark green &
                           "#32CD32", "#228B22", "#006400",   
                           //for J piece 
                           //royal blue & medium blue &  Midnight Blue 
                           "#4169E1", "#0000CD",  "#191970", 
                           //for I piece 
                           //aqua & deep sky blue & dark cyan 
                           "#00FFFF", "#00BFFF", "#008B8B",
                           //for O piece 
                           //yellow & gold & goldenrod 
                           "#FFFF00", "#FFD700", "#DAA520",
                           //for T piece 
                           //medium orchid & purple & indigo
                           "#BA55D3", "#800080", "#4B0082", 
                           //for L piece 
                           //dark orange & orange-red & chocolate
                           "#FF8C00", "#FF4500", "#D2691E",
                           //for canvas background // dark slate grey
                           "#2F4F4F",
                           //for rest of the background: cool -> hot based on play
                           //gainsboro, khaki, sandy brown, tomato, fire brick, red
                           "#DCDCDC", "#F0E68C", "#F4A460", "#FF6347", "#B22222" , "#FF0000" 
                           };
  cmap=DefaultColormap(d,DefaultScreen(d));
  for(int i=0; i < 30; ++i) {
      if (!XParseColor(d,cmap,color_vals[i],&xcolour)) {
         cerr << "Bad colour: " << color_vals[i] << endl;
      }
      if (!XAllocColor(d,cmap,&xcolour)) {
         cerr << "Bad colour: " << color_vals[i] << endl;
      }
      colours[i]=xcolour.pixel;
  }

  XSetForeground(d,gc,colours[Black]);

  // Make window non-resizeable.
  XSizeHints hints;
  hints.flags = (USPosition | PSize | PMinSize | PMaxSize );
  hints.height = hints.base_height = hints.min_height = hints.max_height = height;
  hints.width = hints.base_width = hints.min_width = hints.max_width = width;
  XSetNormalHints(d, w, &hints);

  XSynchronize(d,True);

  usleep(1000);
}

Xwindow::~Xwindow() {
  XFreeGC(d, gc);
  XCloseDisplay(d);
}

void Xwindow::drawBlock(int x, int y, char block){
  int ypos = 25 + x*25;
  int xpos = 25 + y*25;
  int c[3] = {0};
  switch(block){
     case 'O': c[0]=OA; c[1] = OB; c[2] = OC; //hey oc! REPOOST
               break;
     case 'I': c[0]=IA; c[1] = IB; c[2] = IC; 
               break;
     case 'T': c[0]=TA; c[1] = TB; c[2] = TC; 
               break;
     case 'S': c[0]=SA; c[1] = SB; c[2] = SC; 
               break;
     case 'Z': c[0]=ZA; c[1] = ZB; c[2] = ZC; 
               break;
     case 'J': c[0]=JA; c[1] = JB; c[2] = JC;
               break; 
     case 'L': c[0]=LA; c[1] = LB; c[2] = LC; 
               break;    
  }
  fillRectangle(xpos, ypos, 4, 24, c[0]);   //vertical highlight 1
  fillRectangle(xpos, ypos, 21, 4, c[0]);   //horizontal highlight 1
  
  fillRectangle(xpos+4, ypos+4, 17, 17, c[1]);   //mid section
  
  fillRectangle(xpos+21, ypos, 4, 24, c[2]);   //vertical shadow 1
  fillRectangle(xpos, ypos+21, 24, 4, c[2]);   //horizontal shadow 1
}

void Xwindow::drawBoard(int heat){
  int colour;
  switch(heat){
     case 0: colour = RA;
             break;
     case 1: colour = RB;
             break;
     case 2: colour = RC;
             break;
     case 3: colour = RD;
             break;
     case 4: colour = RE;
             break; 
  }
  if (heat >= 5){
     colour = RF;
  }
  //draw background
  fillRectangle(0,0,500,500, colour);
  
  //draw canvas and boarder
  fillRectangle(20,20,5,460,Black); //left
  fillRectangle(20,20,260,5,Black); //top
  fillRectangle(275,20,5,460,Black); //right
  fillRectangle(20,475,260,5,Black); //bottom
  
  fillRectangle(25,25,250,450,CBack);
  
  //draw next and boarder
  fillRectangle(310,60,5,60,Black); //left
  fillRectangle(310,60,110,5,Black); //top
  fillRectangle(415,60,5,60,Black); //right
  fillRectangle(310,115,110,5,Black); //bottom
  
  fillRectangle(315,65,100,50,CBack);
  
  //draw the "Next: " string
  drawBigString(310, 30, "Next: ", Black);
  //draws the box for scores
  fillRectangle(310,400,110,5,CBack); //top
  fillRectangle(310,470,110,5,CBack); //bottom
  //cout <<"board is drawn!" << endl;
}

void Xwindow::drawScore(int level, int score, int hiScore,int heat){
  int colour;
  switch(heat){
     case 0: colour = RA;
             break;
     case 1: colour = RB;
             break;
     case 2: colour = RC;
             break;
     case 3: colour = RD;
             break;
     case 4: colour = RE;
             break;
  }
  if (heat >= 5){
     colour = RF;
  }

  fillRectangle(310,405,189,65,colour); //clear spot
  stringstream s1;
  s1 << level;
  string l;
  s1 >> l;
  stringstream s2;
  s2 << score;
  string s;
  s2 >> s;
  stringstream s3;
  s3 << hiScore;
  string hs;
  s3 >> hs;
  l = "Level: " + l;
  s = "Score: " + s;
  hs = "Highscore: " +hs;
  drawString(310, 425, l, Black);
  drawString(310, 440, s, Black);
  drawString(310, 455, hs, Black);
}
void Xwindow::clearCanvas(){
  fillRectangle(25,25,250,450,CBack);
}
void Xwindow::drawNext(char piece, bool trans, bool skip, int heat){
  fillRectangle(315,65,100,50,CBack); //clear first
  int colour;
  switch(heat){
     case 0: colour = RA;
             break;
     case 1: colour = RB;
             break;
     case 2: colour = RC;
             break;
     case 3: colour = RD;
             break;
     case 4: colour = RE;
             break; 
  }
  if (heat >= 5){
     colour = RF;
  }
  int x[4]; int y[4];
  switch(piece){
     case 'J': x[0] = 315; y[0] = 65;
               x[1] = 315; y[1] = 90;
               x[2] = 340; y[2] = 90;
               x[3] = 365; y[3] = 90;
               break;
     case 'L': x[0] = 365; y[0] = 65;
               x[1] = 315; y[1] = 90;
               x[2] = 340; y[2] = 90;
               x[3] = 365; y[3] = 90;
               break;
     case 'S': x[0] = 315; y[0] = 90;
               x[1] = 340; y[1] = 90;
               x[2] = 340; y[2] = 65;
               x[3] = 365; y[3] = 65;
               break;
     case 'Z': x[0] = 315; y[0] = 65;
               x[1] = 340; y[1] = 65;
               x[2] = 340; y[2] = 90;
               x[3] = 365; y[3] = 90;
               break;
     case 'I': x[0] = 315; y[0] = 65;
               x[1] = 340; y[1] = 65;
               x[2] = 365; y[2] = 65;
               x[3] = 390; y[3] = 65;
               break;
     case 'O': x[0] = 315; y[0] = 65;
               x[1] = 315; y[1] = 90;
               x[2] = 340; y[2] = 65;
               x[3] = 340; y[3] = 90;
               break;
     case 'T': x[0] = 315; y[0] = 65;
               x[1] = 340; y[1] = 65;
               x[2] = 365; y[2] = 65;
               x[3] = 340; y[3] = 90;
               break;
  }
  int c[3] = {0};
  switch(piece){
     case 'O': c[0]=OA; c[1] = OB; c[2] = OC; //hey oc! REPOOST
               break;
     case 'I': c[0]=IA; c[1] = IB; c[2] = IC;
               break;
     case 'T': c[0]=TA; c[1] = TB; c[2] = TC;
               break;
     case 'S': c[0]=SA; c[1] = SB; c[2] = SC;
               break;
     case 'Z': c[0]=ZA; c[1] = ZB; c[2] = ZC;
               break;
     case 'J': c[0]=JA; c[1] = JB; c[2] = JC;
               break;
     case 'L': c[0]=LA; c[1] = LB; c[2] = LC;
               break;
  }

  for(int i = 0; i < 4; i++){
     fillRectangle(x[i], y[i], 4, 24, c[0]);   //vertical highlight 1
     fillRectangle(x[i], y[i], 21, 4, c[0]);   //horizontal highlight 1

     fillRectangle(x[i]+4, y[i]+4, 17, 17, c[1]);   //mid section

     fillRectangle(x[i]+21, y[i], 4, 24, c[2]);   //vertical shadow 1
     fillRectangle(x[i], y[i]+21, 24, 4, c[2]);   //horizontal shadow 1
  }
  fillRectangle(310,135,189,50,colour);
  if(trans){
     drawString(310, 150, "Transparent!", RD);
  }
  if(skip){
     drawString(310, 165, "Skippable!", RD);
  }
  //cout << "next drawn!" << endl;
}
void Xwindow::fillRectangle(int x, int y, int width, int height, int colour) {
  XSetForeground(d, gc, colours[colour]);
  XFillRectangle(d, w, gc, x, y, width, height);
  XSetForeground(d, gc, colours[Black]);
}

void Xwindow::drawString(int x, int y, string msg, int colour) {
  XSetForeground(d, gc, colours[colour]);
  //Font f = XLoadFont(d, "-*-helvetica-bold-r-normal--*-240-*-*-*-*-*");
  ostringstream name;
  name << "-*-helvetica-bold-r-*-*-*-120-" << width/5 << "-" << height/5 << "-*-*-*-*";

  XFontStruct * f = XLoadQueryFont(d, name.str().c_str());
  XTextItem ti;
  ti.chars = const_cast<char*>(msg.c_str());
  ti.nchars = msg.length();
  ti.delta = 0;
  ti.font = f->fid;
  XDrawText(d, w, gc, x, y, &ti, 1);
  XSetForeground(d, gc, colours[Black]);
  XFlush(d);
  //cout << "draw small string finished" << endl;
}


void Xwindow::drawBigString(int x, int y, string msg, int colour) {
  //cout << "drawing string " << endl;
  XSetForeground(d, gc, colours[colour]);
  //Font f = XLoadFont(d, "-*-helvetica-bold-r-normal--*-240-*-*-*-*-*");
  ostringstream name;
  name << "-*-helvetica-bold-r-*-*-*-180-" << width/5 << "-" << height/5 << "-*-*-*-*";

  XFontStruct * f = XLoadQueryFont(d, name.str().c_str());
  //cout << "font is ok" << endl;
  XTextItem ti;
  ti.chars = const_cast<char*>(msg.c_str());
  ti.nchars = msg.length();
  ti.delta = 0;
  ti.font = f->fid;
  XDrawText(d, w, gc, x, y, &ti, 1);
  //cout << "text drawn" << endl;
  XSetForeground(d, gc, colours[Black]);
  XFlush(d);
  //cout << "draw string finished" << endl;
}
