#include <iostream>
#include <iomanip>
#include "board.h"

Board * Board::instance = 0;
Board * Board::getInstance(const bool & t){
if(!instance){
      instance = new Board(t);
      atexit(cleanup);
   }
   return instance;
}
Board::Board(const bool & t):x(0),y(0),score(0),highscore(0),level(0),
		gameBoard(0),current(0),active(0),tOnly(0),npT(0),npS(0),
	        GDisp(0),oldHeat(0){
   myFiles[0] = "sequence.txt";
   myFiles[1] = "lvl1.txt";
   myFiles[2] = "lvl2.txt";
 	myFiles[3] = "lvl3.txt";	
 	myFiles[4] = "lvl4.txt";
   myFiles[5] = "lvl5.txt";
   myFiles[6] = "lvl6.txt";
   myFiles[7] = "lvl7.txt";
   myFiles[8] = "lvl8.txt";
   myFiles[9] = "lvl9.txt";
   myFiles[10] = "lvl10.txt";
   myFiles[11] = "lvl11.txt";
   myFiles[12] = "lvl12.txt";
   myFiles[13] = "lvl13.txt";	
   myFiles[14] = "lvl14.txt";
   tOnly = t;
   if(!tOnly){
      GDisp = new Xwindow;
   }
}

void Board::setZero(const std::string & file){
   myFiles[0] = file;
}
void Board::start(const int & r, const int & c,const int &slevel,const unsigned int & seed){
   x = r;
   y = c;
   if(gameBoard!=NULL){
      cleanBoard();
   }
   
   active = true;
   startingLevel = slevel; //backup
   this->level = startingLevel;
   this->seed = seed;
   gameBoard = new char*[x];
   for(int i = 0; i < x; i++){
      gameBoard[i] = new char[y];
      for(int j = 0; j < y; j++){
         gameBoard[i][j] = ' ';
      }
   }
   for(int i = 0; i <2; i++){
      for(int j = 0; j < 4; j++){
         nextGrid[i][j] = ' ';
      }
   }
   prng.seed(seed);
   changePieces();
   //std::cerr <<"pieces are cahgned "<< std::endl;
   updateNext();//first next
   //std::cerr <<"pieces are now ready"<< std::endl;
   checkCurrent();
   
}

void Board::changePieces(){
   if(pieces.size()>0){
      pieces.clear();
   }
   std::fstream myScript;
   char temp;
   //std::cerr<<level<< " ";
   myScript.open(myFiles[level].c_str());
   //std::cerr << "file good" << std::endl;
   while(!myScript.eof()){
      myScript >> temp;
      pieces.push_back(temp);
      //std::cerr<< temp<<" piece added" << std::endl;
   }
   myScript.close();
}
void Board::lvlU(){
   level++;
   if(level >= 14){
      level = 14;
   }
   changePieces();
}

void Board::lvlD(){
   level--;
   if(level < 0){
      level = 0;
   }
   changePieces();
}

void Board::dropPiece(){
   //checking if we lost
   //reason for return
   //is to prevent dropping a piece on a board
   //then dropping it again
   //after the board gets recreated.
   switch(next){
      case 'J': if(gameBoard[3][0] != ' ' ||gameBoard[4][0] != ' ' ||
                   gameBoard[4][1] != ' ' ||gameBoard[4][2] != ' '){
                   restart();
                   return;
                }
                break;
      case 'L': if(gameBoard[3][2] != ' ' ||gameBoard[4][0] != ' ' ||
                   gameBoard[4][1] != ' ' ||gameBoard[4][2] != ' '){
                   restart();
                   return;
                }
                break;
      case 'S': if(gameBoard[3][1] != ' ' ||gameBoard[4][0] != ' ' ||
                   gameBoard[4][1] != ' ' ||gameBoard[3][2] != ' '){
                   restart();
                   return;
                }
                break;
      case 'Z': if(gameBoard[3][0] != ' ' ||gameBoard[4][1] != ' ' ||
                   gameBoard[3][1] != ' ' ||gameBoard[4][2] != ' '){
                   restart();
                   return;
                }
                break;
      case 'T': if(gameBoard[3][0] != ' ' ||gameBoard[4][1] != ' ' ||
                   gameBoard[3][1] != ' ' ||gameBoard[3][2] != ' '){
                   restart();
                   return;
               }
                break;
      case 'I': if(gameBoard[3][0] != ' ' ||gameBoard[3][3] != ' ' ||
                   gameBoard[3][1] != ' ' ||gameBoard[3][2] != ' '){
                   restart();
                   return;
                }
                break;
      case 'O': if(gameBoard[3][0] != ' ' ||gameBoard[4][0] != ' ' ||
                   gameBoard[3][1] != ' ' ||gameBoard[4][1] != ' '){
                   restart();
                   return;
                }
                break;
               
   }
   
   current = new Tetrimino(next,instance,level,npT,npS);
   current->notifyBoard(instance);
   //std::cerr << "board is up" << std::endl;
}
void Board::checkCurrent(){
   if(current == NULL){ //Piece might die if moving 
      //Random piece picking
      dropPiece();  
      //picks a random index from 0 to sizeof(pieces)
      //and use that index to access one of the chars, and drop that piece onto the board
      updateNext();
      //std::cerr << "piece dropped and next is ready" << std::endl;
   } 

}
void Board::updateNext(){
   next = pieces[prng(0,pieces.size()-1)];
   
   //randomizing next piece's property
   int PT = prng(0,10);
   int PS = prng(0,10);
   
   if(PT == 1){
      npT = true;
   }
   else{
      npT = false;
   }
   if(PS == 1){
      npS = true;
   }
   else{
      npS = false;
   }
   for(int i = 0; i <2; i++){
      for(int j = 0; j < 4; j++){
         nextGrid[i][j] = ' ';
      }
   }
   switch(next){
      //hard-coding since there isn't an algorithm for this.
      //top left of the board looks like this:
      //|00 01 02 03 <-
      //|10 11 12 13 <- the spaces we have

      case 'J':   nextGrid[0][0] = 'J';
                  nextGrid[1][0] = 'J';
                  nextGrid[1][1] = 'J';
                  nextGrid[1][2] = 'J';
                  break;   
      case 'L':   nextGrid[0][2] = 'L';
                  nextGrid[1][0] = 'L';
                  nextGrid[1][1] = 'L';
                  nextGrid[1][2] = 'L';
                  break;  
      case 'S':   nextGrid[1][0] = 'S';
                  nextGrid[1][1] = 'S';
                  nextGrid[0][1] = 'S';
                  nextGrid[0][2] = 'S';
                  break;  
      case 'Z':   nextGrid[0][0] = 'Z';
                  nextGrid[0][1] = 'Z';
                  nextGrid[1][1] = 'Z';
                  nextGrid[1][2] = 'Z';
                  break;  
      case 'I':   nextGrid[0][0] = 'I';
                  nextGrid[0][1] = 'I';
                  nextGrid[0][2] = 'I';
                  nextGrid[0][3] = 'I';
                  break;  
      case 'O':   nextGrid[0][0] = 'O';
                  nextGrid[0][1] = 'O';
                  nextGrid[1][0] = 'O';
                  nextGrid[1][1] = 'O';
                  break;  
      case 'T':   nextGrid[0][0] = 'T';
                  nextGrid[0][1] = 'T';
                  nextGrid[1][1] = 'T';
                  nextGrid[0][2] = 'T';
                  break;   
   }
}
void Board::move(const int & direction){
   if(direction == 3){ //drop
      while(current!= NULL){
         current->move(3,instance);
      }
   }
   else{
      current->move(direction+1, instance);
   }
   checkCurrent(); 
}
bool Board::moveLR(int * r, int* c, int dir){
   int pair[4] = {-1};
   int num = 0;
   //finds index of all coordinates with unique x's
   pair[num] = 0;
   num++;
   for(int i = 1; i < 4; i++){
      bool in = false;
      for(int j = 0; j < num; j++){
         if(r[pair[j]] == r[i]){  //if duplicate
            if(c[pair[j]] > c[i] && dir == 1){  //if going left, keep smaller
               pair[j] = i;
            }
            if(c[pair[j]] < c[i] && dir == 2){  //if going right, keep larger
               pair[j] = i;
            }
            in = true;
         }
      }
      if(!in){
         pair[num] = i;
         num++;
      }
   }
   //check for wall hits
   for(int i = 0; i< num; i++){
      if(dir == 1){
         if(c[pair[i]]-1 <0){
           return false;
         }
      }
      if(dir == 2){
         if(c[pair[i]]+1 >= this->y){
            return false;
         }
      }
   }
   //check for piece hits
   if(!current->getTrans()){
      for(int i = 0; i < num; i++){
         if(dir == 1 && gameBoard[r[pair[i]]][c[pair[i]]-1] != ' '){
            return false;
         }
         if(dir == 2 && gameBoard[r[pair[i]]][c[pair[i]]+1] != ' '){
            return false;
         }
      }
   }
   return true;
}
void rotateRaw(const char & block, int & dir,const int & dirc, const int & times, int * nx, int * ny);
void Board::rotateBlock(const int & direction,const int & times){
   current->rotate(direction,times,instance);
}
bool Board::rotatePossible(const char & block, int & dir, const int & dirc, const int & times, int * x, int * y){
   int newx[4];
   int newy[4];
   int newDir = dir;
   char backup[4];
   for(int i = 0; i < 4; i++){
      newx[i] = x[i];
      newy[i] = y[i];
   }
   rotateRaw(block,newDir,dirc,times,newx,newy);
   //out of bounds
   for(int i = 0; i < 4; i++){
      if(newx[i] < 0 || newy[i] >= this->y){
         return false;
      }
   }
   //piece hits
   if(!current->getTrans()){
      for(int i = 0; i < 4; i++){
         if(gameBoard[newx[i]][newy[i]] != ' '){  
            return false;
         }
      }
   }
   return true;
}
bool Board::moveD(int * r, int * c){
   int pair[4] = {-1};
   int num = 0;
   //finds index of all coordinates with unique y's
   pair[num] = 0;
   num++;
   for(int i = 1; i < 4; i++){
      bool in = false;
      for(int j = 0; j < num; j++){
         if(c[pair[j]] == c[i]){  //if duplicate
            if(r[pair[j]] < r[i]){  //keep larger because lower
               pair[j] = i;
            }
            in = true;
         }
      }
      if(!in){
         pair[num] = i;
         num++;
      }
   }
   //check for hits
   for(int i = 0; i < num; i++){
      if(r[pair[i]]+1 == this->x){
         return false;
      }
      if(!(current->getTrans()) && (gameBoard[r[pair[i]]+1][c[pair[i]]] != ' ')){
         return false;
      }
   } 
   return true;  
}

void Board::notify(const int &x, const int &y, const char &block){
   if(active && current != NULL){ //guard
      if(!current->getTrans()){
         //std::cerr << "not transparent" << std::endl;
         gameBoard[x][y] = block;
      }
      else{
         //std::cerr << "transparent" << std::endl;
         if(gameBoard[x][y] == ' ' || gameBoard[x][y] == current->whatBlock()){
            gameBoard[x][y] = block;
         }
      }
   } //triple guard
}

void Board::death(){
   int x[4];
   int y[4];
   //transparent pieces may have less than 4 pieces alive when they die
   if(current->getTrans()){
      for(int i = 0; i < 4; i++){
         x[i] = current->get('x')[i];
         y[i] = current->get('y')[i];
         if(gameBoard[x[i]][y[i]] != current->whatBlock()){ //if it's not that piece
            current->killBlock(i);                          //then we need to kill it
         }
      }      
   }
   deadpieces.push_back(current);
   current = NULL;
   detect();
}

void Board::detect(){
   if(score > highscore){
      highscore = score;
   }
   int lines = 0;
   for(int i = 3; i < x; i++){
      bool clear = true;
      for(int j = 0; j < y; j++){
         if(gameBoard[i][j] == ' '){
            clear = false;
         }
      }
      if(clear){
         clearLine(i);
         checkPieces(i);
         lines ++;
      }
   }
   score += (level + lines) * (level + lines);
}

void Board::skip(){
   if(current->getSkip()){
      current->clear(instance);
      delete current;
      current = NULL;
      checkCurrent();
   }
}
void Board::checkPieces(const int & line){
   int tempx[4];
   for(int i = 0; i< deadpieces.size(); i++){
      //copies the x-coordinates of a deadpiece
      for(int j = 0; j < 4; j++){
         tempx[j] = deadpieces[i]->get('x')[j];
         //compares the line value to the x-value
         //same value gets a substraction to the nunMber of alive blocks
         if(tempx[j] == line){
            deadpieces[i]->killBlock(j);
         }
         tempx[j]++;
         //checks if the block is alive, if it's dead, keep it that way
         if(deadpieces[i]->get('x')[j] == -1){
            tempx[j] = -1;
         }
      }
      //update the coordinates after clearing line
      deadpieces[i]->update(tempx, deadpieces[i]->get('y'));
      if(deadpieces[i]->getAlive() == 0){
				 score+= (deadpieces[i]->getLevel() + 1) *
					       (deadpieces[i]->getLevel() + 1);
         //when all of the blocks gets cleared from a piece
         Tetrimino * temp = deadpieces[i];
         deadpieces.erase(deadpieces.begin()+i);
         delete temp;
         //add score

      }
   }
}

void Board::drawG(){
   int heat = deadpieces.size()/((level+1)*2);
   //heat is calculated using # of dead pieces /2(level+1)
   if(heat != oldHeat || deadpieces.size() == 0){ //first time
      oldHeat = heat;
      GDisp->drawBoard(heat); //draw Board
   }
   //std::cerr << "clearing canvas!" << std::endl; 
   GDisp->clearCanvas(); //clears tetrimino space
   
   //we want to minimize block draw because our fancy
   //graphics requires 17 rectangles to draw a single cell
   //each tetrimino has 4 cells
   //so to fill the whole board we'd need draw 2550 rectangles!
   //we want to draw as little as possible without over complicating the
   //algorithm, so max rectangle drawn is n<=2550 +1 (for canvas clear)
   //std::cerr << "drawing blocks!" << std::endl;
   for(int i = 0; i < x; i ++){
      for(int j = 0; j<y; j++){
         if(gameBoard[i][j] != ' '){ //minimizes block draw
            GDisp->drawBlock(i,j,gameBoard[i][j]); 
         }
      }
   }

   //std::cerr<<"drawing next!" << std::endl;
   //now we draw next block
   GDisp->drawNext(next,npT,npS,heat);
   
   //std::cerr <<"drawing score!" <<std::endl;
   //now we draw scores
   GDisp->drawScore(level, score, highscore,heat);
}
void Board::clearLine(const int & n){ //recursively copies the previous line from line n
   if(n == 3){return;}
   for(int i = 0; i < y; i++){
      gameBoard[n][i] = gameBoard[n-1][i];
   }
   clearLine(n-1);
}


void Board::restart(){
   cleanBoard();
   start(x,y,startingLevel,seed);
}
void Board::cleanup(){
   delete instance;
}
void Board::cleanBoard(){
   active = false;
   if(gameBoard){
      for(int i = 0; i < x; i++){
         delete [] gameBoard[i];
      }
      delete [] gameBoard;
   }
   if(current){
      delete current;
      current = NULL;
   }
   Tetrimino * temp;
   while(!deadpieces.empty()){
     temp = deadpieces.back();
     delete temp;
     deadpieces.pop_back();
   }
   //std::cerr<<"deleting dead" << std::endl;
   score = 0;
   level = 0;
   gameBoard = NULL;
   
}

std::ostream &operator<<(std::ostream &out, Board & b){
   if(!b.tOnly){
     b.drawG(); //draws graphics
   }
   system("clear");
   out << std::setw(13) <<std::left<< "Level:" << std::right << b.level << std::endl;
   out << std::setw(13) << std::left << "Score:"; 
   out << std::right << b.score << std::endl;
   out << std::setw(13) << std::left  << "Hi Score:"; 
   out << std::right << b.highscore;
   out << std::endl << std::left;
   out << "----------" << std::endl;
   for(int i = 0; i < b.x; i++){
      for(int j = 0; j < b.y; j++){
          out << b.gameBoard[i][j];
      }
      out << std::endl;
   }
   out << "----------" << std::endl;
   out << "Next: ";
   if(b.npT){
      out << " Transparent";
   }
   if(b.npS){
      out << " Skippable";
   }
   out<<std::endl;
   
   for(int i = 0; i < 2; i++){
      for(int j = 0; j < 4; j++){
         out<< b.nextGrid[i][j];
      }
      out << std::endl;
   }
   return out;
}

bool Board::isActive(){
   return active;
}
