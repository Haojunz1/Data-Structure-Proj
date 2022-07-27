// file: CBofCB.cpp
//
// UMBC CMSC 341 Fall 2018 Project 1
//
// cpp file for outer circular buffer
//

#include "CBofCB.h"
#include <stdexcept>
#include <iostream>
using namespace std;


// default constructor
CBofCB :: CBofCB(){
  m_obSize = 1;
  m_oldest = 0;
  m_newest = 0;
  
  for(int i = 0; i < m_obCapacity; i++){
    m_buffers[i] = NULL;
  }
  
  m_buffers[0] = new InnerCB();
}

// copy constructor
CBofCB :: CBofCB(const CBofCB& other){
  m_obSize = other.m_obSize;
  m_oldest = other.m_oldest;
  m_newest = other.m_newest;
  
  for(int i = 0; i < m_obCapacity; i++){
    m_buffers[i] = NULL;
  }
  
  for(int i = 0; i < m_obCapacity; i++){
    if(other.m_buffers[i] != NULL){
      m_buffers[i] = new InnerCB(*other.m_buffers[i]);
    }
  }
}

// destructor
CBofCB:: ~CBofCB(){
  for(int i = 0; i < m_obCapacity; i++){
    if(m_buffers[i] != NULL){
      delete m_buffers[i];
      m_buffers[i] = NULL;
    }
  }
  m_obSize = 0;
  m_oldest = 0;
  m_newest = 0;
  
}

void CBofCB :: enqueue(int data){
  
  if(isFull()){
    
    throw std:: overflow_error("overflow_error");
  }else if(isEmpty()){
    
    m_newest = 0;
    m_oldest = 0;
    m_buffers[m_newest]->enqueue(data);

  }else if(m_buffers[m_newest]->isFull()){
    
    int capacity = m_buffers[m_newest]->capacity();
    
    m_newest = (m_newest + 1) % m_obCapacity;
    
    m_buffers[m_newest] = new InnerCB(2*capacity);
    
    m_obSize++;
    
    m_buffers[m_newest]->enqueue(data);
    
  }else{
    m_buffers[m_newest]->enqueue(data);
  }
}


int CBofCB :: dequeue(){
  int data;
  if(isEmpty()){
    throw std::underflow_error("underflow_error");
    
  }else{
    data = m_buffers[m_oldest]->dequeue();
    if(m_buffers[m_oldest]->isEmpty() && m_obSize != 1){
      delete m_buffers[m_oldest];
      m_buffers[m_oldest] = NULL;
      m_obSize--;
      m_oldest = (m_oldest + 1) % m_obCapacity;
    }
  }
  return data;
}

bool CBofCB :: isFull(){
  if(m_obSize == m_obCapacity
     && m_buffers[m_newest]->isFull()){
    return true;
  }
  return false;
} 

// returns true if no items stored in data structure
bool CBofCB :: isEmpty(){
  
  if(m_obSize == 1
     && m_buffers[m_oldest]->isEmpty()) {
    return true;
  } else {
    return false;
  }
}

// number of items in the data structure as a whole.
// Note: not the number of InnerCB's
int CBofCB :: size(){
  int size = 0;
  if(isEmpty()){
    size = 0;
  }else {
    int oldest;
    oldest = m_oldest;
    
    for(int i = 0; i < m_obSize; i++){
      size = size + m_buffers[oldest]->size();
      oldest = (oldest + 1) % m_obCapacity;
    }
  }
  return size;
}

// overloaded assignment operator
const CBofCB& CBofCB :: operator=(const CBofCB& rhs){
  if(this != &rhs){
    for(int i = 0; i < m_obCapacity; i++){
      if(m_buffers[i] != NULL){
        delete m_buffers[i];
        m_buffers[i] = NULL;
      }
    }
    for(int i = 0; i < m_obCapacity; i++ ){
      
      if(rhs.m_buffers[i] != NULL){
	m_buffers[i] = new InnerCB(*rhs.m_buffers[i]);
	
      } 
    }
    m_oldest = rhs.m_oldest;
    m_newest = rhs.m_newest;
    m_obSize = rhs.m_obSize; 
  }
  return* this;
}

// debugging function, prints out contents of data structure
void CBofCB :: dump(){
  cout << "Outer dump() : m_obSize = " << m_obSize << endl;
  int oldest, newest;
  oldest = m_oldest;
  newest = m_newest;
  
  for(int i = 0; i < m_obSize; i++){
    m_buffers[oldest]->dump();
    oldest = (oldest + 1) % m_obCapacity;
  }
} 


