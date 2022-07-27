/********************************************
 ** File:    MedianHeap.h
 ** Author:  April Zhu
 ** Date:    11/19/2018
 ** E-mail:  haojunz1@umbc.edu
 *********************************************/

#ifndef MEDIANHEAP_H
#define MEDIANHEAP_H

#include <iostream>

template <typename T>
class Heap {
public:
  Heap();
  ~Heap();
  Heap(int capacity);
  
  void reBalance();
  bool (*compare)(const T&, const T&);
  void bubbleUp(int hole);
  void trickleDown(int hole);
  
  int m_heapCapacity;
  int m_heapSize;
  
  T* m_heap;
  
};

template <typename T>
class MedianHeap {
 public:
  

  MedianHeap();
  ~MedianHeap();
  MedianHeap( bool (*lt) (const T&, const T&), bool (*gt) (const T&, const T&), int cap=100 ) ;
  MedianHeap(const MedianHeap <T>& otherH);
  const MedianHeap<T>& operator=(const MedianHeap<T>& rhs);

  int size();
  int capacity();
  void insert(const T& item);
  T getMedian();
  T getMin();
  T getMax();

  bool deleteItem(T& givenItem, bool (*equalTo) (const T&, const T&) ) ;

  void dump();

  int maxHeapSize();
  int minHeapSize();
  T locateInMinHeap(int pos);
  T locateInMaxHeap(int pos);
  
  int m_capacity;
  int m_size;
  T m_max;
  T m_min;
  
  Heap<T>* m_maxHeap;
  Heap<T>* m_minHeap;

};

#include "MedianHeap.cpp"
#endif
