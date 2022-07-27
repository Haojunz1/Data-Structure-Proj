// file: InnerCB.cpp
//
// UMBC CMSC 341 Fall 2018 Project 1
//
// CPP file for inner Cicular Buffer.
//
//

#include "InnerCB.h"
#include <stdexcept>
#include <iostream>
using namespace std;


// Constructor, default size is 10.
InnerCB :: InnerCB(int n) {
  m_start = 0;
  m_end = 0;
  m_size = 0;
  m_capacity = n;
  m_buffer = new int[m_capacity];
}

// Copy constructor
InnerCB :: InnerCB(const InnerCB& other) {
  m_start = other.m_start;
  m_end = other.m_end;
  m_size = other.m_size;
  m_capacity = other.m_capacity;
  m_buffer = new int[m_capacity];
  
  for (int i = 0; i < m_capacity; i++){
    m_buffer[i] = other.m_buffer[i];
  }
}

// Destructor
InnerCB :: ~InnerCB() {
  delete[] m_buffer;
  m_start = 0;
  m_end = 0;
  m_size = 0;
  m_capacity = 0;
}

// Add item to circular buffer
void InnerCB :: enqueue(int data) {
  if(isFull()){
    throw std::overflow_error("overflow_error");
    
  }else if (isEmpty()) {
    m_start = 0;
    m_end = 0;
    m_buffer[m_end] = data; 
    m_size++;
  } else {
    m_end = (m_end + 1) % m_capacity;
    m_buffer[m_end] = data;
    
    m_size++;
  }
  
}
// Remove item from circular buffer
int InnerCB :: dequeue(){
  
  int data;
  if (isEmpty()){
    throw std::underflow_error("underflow_error");
  }else{
    
    data = m_buffer[m_start];
    m_size --;
    m_start = (m_start + 1) % m_capacity;
  }
  return data;
}


bool InnerCB :: isFull(){
  if(m_capacity == m_size){
    return true;
  }
  return false;
}


// True if buffer holds no items
bool InnerCB :: isEmpty(){
  if (m_size == 0){
    return true;
  }
  return false;
}

// return maximum number of items this buffer can hold
int InnerCB :: capacity(){
  return m_capacity;
}

// return number of items currently held in the buffer
int InnerCB :: size(){
  return m_size;
}

// overloaded assignment operator
const InnerCB& InnerCB :: operator=(const InnerCB& rhs){
  if(this != &rhs){
    delete[] m_buffer;

    m_start = rhs.m_start;
    m_end = rhs.m_end;
    m_size = rhs.m_size;
    m_capacity = rhs.m_capacity;
    m_buffer = new int[rhs.m_capacity];

    for(int i = 0; i < rhs.m_capacity; i++){
      m_buffer[i] = rhs.m_buffer[i];
    }
  }
  return* this;
}

// debugging function. Prints out contents.
void InnerCB :: dump(){
  cout << "InnerCB dump(): m_size = " << m_size << endl;
  if(m_size == 0){
    cout << "Empty" << endl;
  }else{ 
    int start, end;
    start = m_start;
    end = m_end;
    
    for(int i = 0; i < m_size; i++){
      cout << "[" << start << "] " << m_buffer[start] << " ";
      start = (start + 1) % m_capacity;
    }    
  }
}



