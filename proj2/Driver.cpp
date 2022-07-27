// File: driver.cpp
//
// CMSC 341 Fall 2018 PROJ2
//
// accepts user input tor filename of Sally Forth source code
#include <iostream>
#include <fstream>
using namespace std ;

#include "Sally.h"

int main() {
  string fname ;

  cout << "Enter file name: " ;
  cin >> fname ;
  ifstream ifile(fname.c_str()) ;

  Sally S(ifile) ;

  S.mainLoop() ;

  ifile.close() ;
  return 0 ;
}
