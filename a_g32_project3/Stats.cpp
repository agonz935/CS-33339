/******************************
 * Submitted by: Alejandra Gonzalez, a_g32
 * CS 3339 - Fall 2020, Texas State University
 * Project 3 Pipelining
 * Copyright 2020, all rights reserved
 * Updated by Lee B. Hinkle based on prior work by Martin Burtscher and Molly O'Neil
 ******************************/
 
#include "Stats.h"

Stats::Stats() {
  cycles = PIPESTAGES - 1; // pipeline startup cost
  flushes = 0;
  bubbles = 0;

  memops = 0;
  branches = 0;
  taken = 0;

  for(int i = IF1; i < PIPESTAGES; i++) {
    resultReg[i] = -1;
  }
}

void Stats::clock() {
  cycles++;

  // advance all pipeline flip-flops
  for(int i = WB; i > IF1; i--) {
    resultReg[i] = resultReg[i-1];
  }
  // inject a NOP in IF1
  resultReg[IF1] = -1;
}

void Stats::registerSrc(int r) {

int bub_num = 0; //number of bubbles needed until register reaches WB
    for(int i = EXE1; i < WB; i++){
        if(resultReg[i] == r && r != 0)
        {
            bub_num = WB - i;
            
            while(bub_num > 0){
                bubble();
                bub_num--;
            }
        } 
    }
}
void Stats::registerDest(int r) {
    resultReg[ID] = r; //keeps track of rds still in pipeline
}

void Stats::flush(int count) { // count == how many ops to flush
    for(int i = 0; i < count; i++){
        flushes++;
        clock(); //pipeline advances all the way back to IF1
    }
}

void Stats::bubble() {  
    bubbles++;
    cycles++;

//advances only the pipeline stages after ID
    for(int i = WB; i > EXE1; i--){
        resultReg[i] = resultReg[i-1]; 
    }

    resultReg[EXE1] = -1;
}

void Stats::showPipe() {
  // this method is to assist testing and debug, please do not delete or edit
  // you are welcome to use it but remove any debug outputs before you submit
  cout << "              IF1  IF2 *ID* EXE1 EXE2 MEM1 MEM2 WB         #C      #B      #F" << endl; 
  cout << "  resultReg ";
  for(int i = 0; i < PIPESTAGES; i++) {
    cout << "  " << dec << setw(2) << resultReg[i] << " ";
  }
  cout << "   " << setw(7) << cycles << " " << setw(7) << bubbles << " " << setw(7) << flushes;
  cout << endl;
}
