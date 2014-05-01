#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <cstdio>
#include "board.h"
#include "tetrimino.h"
#include "window.h"

//custom constants
#define LEFT 0
#define RIGHT 1
#define DOWN 2
#define DROP 3
#define CW 4
#define CCW 5
#define RESTART 6
#define LVLUP 7
#define LVLDOWN 8
#define SKIP 9

#define NUMCMD 10

using namespace std;
//aliases
typedef vector<string> Command;


string advMatch(const string & am, Command & b){ // a step by step match and returns a valid command name or ""
   int n = 0;
   vector<int> killBill; //hehehe
   string a = am;
   for(int i = 0; i < b.size(); i++){
      if(b[i].length() < a.length()){ 
         killBill.push_back(i);  //getting rid of the shorter than command ones
      }
   }
   for(int i = 0; i < killBill.size(); i++){
      b.erase(b.begin()+killBill[i]);
      for(int j = 0; j < killBill.size(); j++){
         killBill[j]--;
      }
   }
   if(b.size() == 0){
      return " ";
   }
   killBill.clear();
   while(1){
      bool erased = false;
      for(int i  = 0; i < b.size(); i++){
         if(a[n] != b[i][n]){
            b.erase(b.begin()+i);
            erased = true;
            break;
         }
      }
      if(!erased){ 
         n++;
      }
      if(b.empty()){
         return " ";
      }
      if(n < a.length() && n < b[0].length()){
         if(b.size()== 1 && a[n+1] == b[0][n+1]){        //YOU ARE TEH CHOSEN ONE
            //cerr << "found! " << b[0] << endl;
            return b[0];
         }
      }
      else{
         if(b.size()==1){
            return b[0];
         }
      }
      if(a.length() == n){
         return " ";
      }
   }
   
}

//match between either "" and commands or a valid command name and commands
int match(const string& a, const Command (&cmdList)[NUMCMD]){
   if(a == " "){
      return -1;
   }
   for(int i = 0; i < NUMCMD; i++){
      for(int j = 0; j < cmdList[i].size(); j++){
         if(a == cmdList[i][j]){
            //cerr << "found again!" << endl;
            return i;
         }
      }   
   }
   return -1;
}

Command flat(const Command (&cmdList)[NUMCMD]){
   Command cmds;
   for(int i = 0; i < NUMCMD; i++){
      for(int j = 0; j < cmdList[i].size(); j++){
         cmds.push_back(cmdList[i][j]);
      }
   }
   return cmds;
}
bool interpret(int & com, int & times, const string & cmd, const Command (&cmdList)[NUMCMD]){
   string ncmd = cmd;
   if((ncmd[0] >= '0' ) && (ncmd[0] <= '9')){  //an actual number
      times = ncmd[0] - '0';
      ncmd.erase(ncmd.begin()); //take out that char
   }
   else{
      times = 1;
   }
   Command temp = flat(cmdList); 
   //cerr << "command list flattened" << endl;
   int test = match(advMatch(ncmd,temp),cmdList);
   if(test == -1){ //no match found =[
      return false;
   }
   com = test;
   //cerr << "command set" << endl;
   return true;
}

void execute(int interp, int times, Board *b){
   //cerr << "executing..." << interp << endl;	
       switch(interp){
	   case LEFT:  for(int i = 0; i < times; i++){
	                  b->move(LEFT);   
	                  
	               }
	               break;
	   case RIGHT: for(int i = 0; i < times; i++){
	                  b->move(RIGHT);   
	               }
	               break;
	   case DOWN:  for(int i = 0; i < times; i++){
	                  b->move(DOWN); 
	               }
	               break;
	   case DROP:  for(int i = 0; i < times; i++){
                          b->move(DROP);                  
	               }
	               break;
	   case CW:    b->rotateBlock(1,times);  
	               break;
	   case CCW:   b->rotateBlock(0,times);   
	               break;
	   case LVLUP:    for(int i = 0; i < times; i++){
	                     b->lvlU();   
	                  }
	                  break;
	   case LVLDOWN:  for(int i = 0; i < times; i++){
	                     b->lvlD();   
	                  }
	                  break;
	   case RESTART:  
                     b->restart();
                     break;
           case SKIP:  b->skip();
                       break;
	}
   //cerr << "excuted!" << endl;
}

int main(int argc, char* argv[]){
   
   bool tOnly = false;      //text only
   unsigned int startingLevel = 0;
   unsigned int seed = 362436069; //default seed
   fstream myScript;
   string myFile = "sequence.txt"; //default file
   //STARTING SEQUENCE
   //Reading commandline options: 
   for(int i = 0; i < argc; i++){
      string command = argv[i];
      cerr << command << endl;
      stringstream ss;
      if(command == "-text"){
         tOnly = true;
         cout << "ya" << endl; 
      }
      if(i+1 != argc){
         if(command == "-scriptfile"){
            myFile = argv[i+1];
         }
         else if(command == "-startlevel"){
            ss << argv[i+1];
            ss >> startingLevel;
         }
         else if(command == "-seed"){
            ss << argv[i+1];
            ss >> seed;
            //cerr << "seed set" << endl;
         }
      }
   }
   //system("paplay music.wav &");
   Board * b = Board::getInstance(tOnly); 
   //cerr << "board operational" << endl;
   b->start(18,10,startingLevel,seed); //default
   //cerr << "board setup complete" << endl;
   b->setZero(myFile); //set level 0 script
   //cerr << "lvl0 set" << endl;

   //setting up a command list
   Command cmdList[NUMCMD];
   //default commands
   cmdList[0].push_back("left");
   cmdList[1].push_back("right");
   cmdList[2].push_back("down");
   cmdList[3].push_back("drop");
   cmdList[4].push_back("clockwise");
   cmdList[5].push_back("counterclockwise");
   cmdList[6].push_back("restart");
   cmdList[7].push_back("levelup");
   cmdList[8].push_back("leveldown");
   cmdList[9].push_back("skip");

   //cerr << "initialization complete" << endl;
   bool keyOn = false;
   int ch;
   while(1){//main loop
      if(b->isActive()){
         cout << *b;
      }
      string cmd;
      int interp;
      int times = 1; //bydefault, excute once;
      if(cin.eof()){
         system("clear");
         b->cleanBoard();
         break;
      }
      else {
         if(!keyOn){
            cin >> cmd;
         }
      }
      if(cmd == "rename"){
         string newcmd;
         string replacing;
         cin >> replacing;
         cin >> newcmd; 
         for(int i = 0; i < NUMCMD; i++){
            for(int j = 0; j < cmdList[i].size(); j++){
               if(replacing == cmdList[i][j]){
                  cmdList[i][j] = newcmd;
               }
            }
         }
      }
      else if(cmd == "keyboard"){
         keyOn = true;
         //system("/bin/stty raw");
      }
      else if(cmd == "addCommand"){
         string newcmd;
         string function;
         cin >> function;
         cin >> newcmd;
         bool dup = false;
         for(int i = 0; i < NUMCMD; i++){
            for(int j = 0; j < cmdList[i].size(); j++){
               if(newcmd == cmdList[i][j]){
                  dup = true;
                  break;
               }
            }
         }
         if(!dup){
            for(int i = 0; i < NUMCMD; i++){
               for(int j = 0; j < cmdList[i].size(); j++){
                  if(function == cmdList[i][j]){
                     cmdList[i].push_back(newcmd);
                  }
               }
	    }
         }
      }
      else if(cmd == "rmCommand"){
         string oldcmd;
         cin >> oldcmd;
         for(int i = 0; i < NUMCMD; i++){
            for(int j = 0; j < cmdList[i].size(); j++){
               if(oldcmd == cmdList[i][j]){
                  if(cmdList[i].size() == 1){ //can't rm all the commands
                     break;
                  }
                  else{
                     cmdList[i].erase(cmdList[i].begin()+j);
                  }
               }
            }
         }
      }
      else if(interpret(interp,times,cmd,cmdList)){ //if valid
         execute(interp,times,b); //excute it
      }
      if(keyOn){
         ch = getchar();
         switch(ch){
            case 27:    ch = getchar();
                        if(ch == 91){
                           ch = getchar();   
                           if(ch == 65){execute(CW,1,b);} //ccw rotate
                           else if(ch == 66){execute(DOWN,1,b);} //left
                           else if(ch == 67){execute(RIGHT,1,b);} //right
                           else if(ch == 68){execute(LEFT,1,b);} //down
                        }
                        break;              
            case ' ':      execute(DROP,1,b); //drop
                        break;
            case 'c':      execute(SKIP,1,b); //skip
                        break;
            case 'u':      execute(LVLUP,1,b); //levelup
                        break;
            case 'd':      execute(LVLDOWN,1,b); //leveldown
                        break;
            case 'r':      execute(RESTART,1,b); //restart
                        break;
            case 'q':      keyOn = false;
                           //system("/bin/stty cooked");
                           break;
	    default:       break;
         }
      }
   }
   
   //system("pulseaudio --kill");
   return 0;
}
