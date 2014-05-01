#include "tetrimino.h"
#include "board.h"
Tetrimino::Tetrimino(const char & piece, Board * b, const int & l, const bool & trans, const bool & skip){
   switch(piece){
      //hard-coding since there isn't an algorithm for this.
      //top left of the board looks like this:
      //|00 01 02 03 <-
      //|10 11 12 13 <- Reserve Rows for rotation
      //|20 21 22 23 <-
      //------------
      //|30 31 32 33 <- This is where the blocks align to
      //|40 41 42 43
      case 'J': x[0] = 4;
                y[0] = 2;
                x[1] = 4;
                y[1] = 1;
                x[2] = 4;
                y[2] = 0;
                x[3] = 3;
                y[3] = 0;
                break;
      case 'L': x[0] = 4;
                y[0] = 0;
                x[1] = 4;
                y[1] = 1;
                x[2] = 4;
                y[2] = 2;
                x[3] = 3;
                y[3] = 2;
                break;
      case 'S': x[0] = 4;
                y[0] = 0;
                x[1] = 4;
                y[1] = 1;
                x[2] = 3;
                y[2] = 1;
                x[3] = 3;
                y[3] = 2;
                break;
      case 'Z': x[0] = 3;
                y[0] = 0;
                x[1] = 3;
                y[1] = 1;
                x[2] = 4;
                y[2] = 1;
                x[3] = 4;
                y[3] = 2;
                break;
      case 'I': x[0] = 3;
                y[0] = 0;
                x[1] = 3;
                y[1] = 1;
                x[2] = 3;
                y[2] = 2;
                x[3] = 3;
                y[3] = 3;
                break;
      case 'T': x[0] = 3;
                y[0] = 0;
                x[1] = 3;
                y[1] = 1;
                x[2] = 3;
                y[2] = 2;
                x[3] = 4;
                y[3] = 1;
                break;
      case 'O': x[0] = 3;
                y[0] = 0;
                x[1] = 3;
                y[1] = 1;
                x[2] = 4;
                y[2] = 0;
                x[3] = 4;
                y[3] = 1;
                break;
   }
   transparent = trans;
   skippable = skip; 
   dir = 0;
   state = true;
   block = piece;
   numBlocks = 4;
   level = l;
   //std::cerr << "notifying" << std::endl; 
   //notifyBoard(b);
}

void Tetrimino::clear(Board * theBoard){
   for(int i = 0; i < 4;  i++){
      theBoard->notify(x[i],y[i],' ');
      //sends positions, and char for block to clear
   }
}

int Tetrimino::getLevel(){
   return level;
}

void rotateRaw(const char & block, int & dir,const int & dirc, const int & times, int * nx, int * ny);

void Tetrimino::rotate(const int &dirc, const int & times, Board * b){
   clear(b);
   if(b->rotatePossible(block,dir,dirc,times,this->x,this->y)){
      rotateRaw(block,dir,dirc,times,this->x,this->y);
   }
   notifyBoard(b);
   return;
}
bool Tetrimino::getSkip(){
   return skippable;
}

bool Tetrimino::getTrans(){
   return transparent;
}

char Tetrimino::whatBlock(){
   return block;
}
void rotateRaw(const char & block, int & dir,const int & dirc, const int & times, int * nx, int * ny){
   if(block == 'O'){
      return;
   }
   int xoff, yoff; //converting to cartesian, this part may be confusing as hell
   int xp[4];
   int yp[4];
   //finding the origin
   if(block == 'J'){
      switch(dir){
         case 0: yoff = nx[1] - 1;
                 xoff = ny[1];
                 break;
         case 1: yoff = nx[3] - 1;
                 xoff = ny[3];
                 break;
         case 2: yoff = nx[1] + 1;
                 xoff = ny[1];
                 break;
         case 3: yoff = nx[3] + 1;
                 xoff = ny[3];
                 break;
      }
   }
   else if(block == 'L'){
      switch(dir){
         case 0: yoff = nx[1] - 1;
                 xoff = ny[1];
                 break;
         case 1: yoff = nx[3] + 1;
                 xoff = ny[3];
                 break;
         case 2: yoff = nx[1] + 1;
                 xoff = ny[1];
                 break;
         case 3: yoff = nx[3] - 1;
                 xoff = ny[3];
                 break;
      }
   }
   else if(block == 'S'){
      xoff = ny[2];
      yoff = nx[2];
   }
   else if(block == 'Z'){
      xoff = ny[1];
      yoff = nx[1];
   }
   else if(block == 'T'){
      xoff = ny[1];
      yoff = nx[1];
   }
   else if(block == 'I'){
      switch(dir){
         case 0: yoff = nx[1];
                 xoff = ny[1];
                 break;
         case 1: yoff = nx[2];
                 xoff = ny[2];
                 break;
         case 2: yoff = nx[1];
                 xoff = ny[1];
                 break;
         case 3: yoff = nx[2];
                 xoff = ny[2];
      }
   }
   int temp;
   int repeats = times;
   if(dirc == 1){ //clockwise
      repeats *= 3;
   }
   for(int j = 0; j < repeats; j++){
      for(int i = 0; i < 4; i++){
         yp[i] = nx[i];     //make a copy but in cartesian
         xp[i] = ny[i];
         yp[i] -= yoff;    //off set it to the origin
         xp[i] -= xoff;
         yp[i] *= -1;      //in converting to cartesion, y will be inversed
         temp = xp[i];
         xp[i] = -1*yp[i]; //by the matrix rotation at 90 degrees cw
         yp[i] = temp;
         yp[i] *= -1;
         xp[i] += xoff;    //get back to the original origin
         yp[i] += yoff;
         nx[i] = yp[i];     //put the new coordinates in
         ny[i] = xp[i];
      }
   }
   dir+= repeats;
   dir = dir % 4;
}
void Tetrimino::move(const int & direction, Board * b){
   //here we check left and right first
   if(!b->moveLR(x,y,direction)){
      return;
   }
   clear(b);
   
   switch(direction){
      case 1: for(int i = 0; i < 4; i++){y[i]-=1;} //left
              break;
      case 2: for(int i = 0; i < 4; i++){y[i]+=1;} //right
              break;
   }
   if(direction == 3){ //now we test for down, hardcode because faster
      if(!b->moveD(x,y)){
         die(); //reached bottom
      }
      else{
         for(int i = 0; i < 4; i++){
            x[i]++;//down
         }        
      }
   }
   notifyBoard(b); //things changed
}

void Tetrimino::update(int *r, int * c){
   for(int i = 0; i < 4; i++){
      x[i] = r[i];
      y[i] = c[i];
   }
}

void Tetrimino::killBlock(const int & block){
   if(x[block] != -1){
      numBlocks--;
   }
   x[block] = -1;
}
int * Tetrimino::get(const char & cor){
   if(cor == 'x'){
      return x;
   }
   else{
      return y;
   }
}
int Tetrimino::getAlive(){
   return numBlocks;
}
int Tetrimino::getDir(){
   return dir;
}
void Tetrimino::updateDir(const int & newD){
   dir = newD;
}

void Tetrimino::notifyBoard(Board * theBoard){
   for(int i = 0; i < 4;  i++){
      //std::cerr << "sending " << i <<std::endl;
      theBoard->notify(x[i],y[i],block); 
      //sends positions, and char for block
   }
   if(!state){
      theBoard->death();
   }
}

void Tetrimino::die(){
   state = false;

}
Tetrimino::~Tetrimino(){}
