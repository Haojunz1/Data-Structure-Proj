#ifndef MEDIANHEAP_CPP_
#define MEDIANHEAP_CPP_

#include "MedianHeap.h"
#include <stdexcept>
using namespace std;


template <typename T>
MedianHeap<T>::MedianHeap(){
	m_minHeap = NULL;
	m_maxHeap = NULL;
	m_size = 0;
	m_capacity = 0;
	m_max = NULL;
	m_min = NULL;
}

template <typename T>
MedianHeap<T>::~MedianHeap(){
	delete m_minHeap;
	delete m_maxHeap;
}

template <typename T>
MedianHeap<T>::MedianHeap( bool (*lt) (const T&, const T&), bool (*gt) (const T&, const T&), int cap){
	m_minHeap = new Heap<T>(cap);
	m_maxHeap = new Heap<T>(cap);
	m_minHeap -> compare = lt;
	m_maxHeap -> compare = gt;
	m_size = 0;
	m_capacity = cap;
}

template <typename T>
MedianHeap<T>::MedianHeap(const MedianHeap <T>& otherH) {
	m_capacity = otherH.m_capacity;
	m_size = otherH.m_size;
	m_max = otherH.m_max;
	m_min = otherH.m_min;
	int minHeapSize = otherH.m_minHeap -> m_heapSize;
	int maxHeapSize = otherH.m_maxHeap -> m_heapSize;
	m_minHeap = new Heap<T>(m_capacity);
	m_maxHeap = new Heap<T>(m_capacity);
	for (int i = 1; i < minHeapSize + 1; i++) {
		m_minHeap -> m_heap[i] = otherH.m_minHeap -> m_heap[i];		 
	}
	for (int i = 1; i < maxHeapSize + 1; i++) {
		m_maxHeap -> m_heap[i] = otherH.m_maxHeap -> m_heap[i];		 
	}
	m_minHeap -> compare = otherH.m_minHeap -> compare;
	m_maxHeap -> compare = otherH.m_maxHeap -> compare;
}

template <typename T>
const MedianHeap<T>& MedianHeap<T>::operator= (const MedianHeap<T>& rhs){
	if (this == &rhs) {
		return *this;
	} else {
		m_size = rhs.m_size;
		m_capacity = rhs.m_capacity;
		delete m_minHeap;
		delete m_maxHeap;
	    m_max = rhs.m_max;
	    m_min = rhs.m_min;
		int minHeapSize = rhs.m_minHeap -> m_heapSize;
		int maxHeapSize = rhs.m_maxHeap -> m_heapSize;
		m_minHeap = new Heap<T>(m_capacity);
		m_maxHeap = new Heap<T>(m_capacity);
		for (int i = 1; i < minHeapSize + 1; i++) {
		m_minHeap -> m_heap[i] = rhs.m_minHeap -> m_heap[i];		 
		}
		for (int i = 1; i < maxHeapSize + 1; i++) {
			m_maxHeap -> m_heap[i] = rhs.m_maxHeap -> m_heap[i];		 
		}
		m_minHeap -> compare = rhs.m_minHeap -> compare;
	    m_maxHeap -> compare = rhs.m_maxHeap -> compare;
	}
}

template <typename T>
int MedianHeap<T>::size(){
	return m_size;
}

template <typename T>
int MedianHeap<T>::capacity(){
	return m_capacity;
}

template <typename T>
void MedianHeap<T>::insert(const T& item) {

	// when insert first
	if (m_size == 0) {
		m_maxHeap -> m_heap[1] = item;
		m_maxHeap -> m_heapSize = m_maxHeap -> m_heapSize + 1;
		m_size++;
		m_max = item;
		m_min = item;
		return;
	}

	if (m_size == m_capacity) {
		throw out_of_range("The heap is full");
	}
	
	T median = getMedian();

	//use compareMax to see track the max item
	if (m_maxHeap -> compare(m_max, item) == false) {
		m_max = item;
	}
	if (m_minHeap -> compare(m_min, item) == false) {
		m_min = item;
	}

	// use maxHeap compareMax to see if item is greater than median
	if (m_minHeap -> compare(item, median) == false){
		m_minHeap -> m_heapSize = m_minHeap -> m_heapSize + 1;
		m_minHeap -> m_heap[m_minHeap -> m_heapSize] = item;
		m_minHeap -> bubbleUp(m_minHeap -> m_heapSize);
	} else {
		m_maxHeap -> m_heapSize = m_maxHeap -> m_heapSize + 1;
		m_maxHeap -> m_heap[m_maxHeap -> m_heapSize] = item;
		m_maxHeap -> bubbleUp(m_maxHeap -> m_heapSize);
	}


	if (m_maxHeap -> m_heapSize - m_minHeap -> m_heapSize > 1) {
		T data = m_maxHeap -> m_heap[1];

		m_maxHeap -> m_heap[1] = m_maxHeap -> m_heap[m_maxHeap -> m_heapSize];
		m_maxHeap -> m_heapSize = m_maxHeap -> m_heapSize - 1;
		m_maxHeap -> trickleDown(1);
		//m_maxHeap -> m_heapSize = m_maxHeap -> m_heapSize - 1;

		m_minHeap -> m_heapSize = m_minHeap -> m_heapSize + 1;
		m_minHeap -> m_heap[m_minHeap -> m_heapSize] = data;
		m_minHeap -> bubbleUp(m_minHeap -> m_heapSize); 

	} else if (m_minHeap -> m_heapSize - m_maxHeap -> m_heapSize > 1) {
		T data = m_minHeap -> m_heap[1];

		m_minHeap -> m_heap[1] = m_minHeap -> m_heap[m_minHeap -> m_heapSize];
		m_minHeap -> m_heapSize = m_minHeap -> m_heapSize - 1;
		m_minHeap -> trickleDown(1);
		//m_minHeap -> m_heapSize = m_minHeap -> m_heapSize - 1;
		
		m_maxHeap -> m_heapSize = m_maxHeap -> m_heapSize + 1;
		m_maxHeap -> m_heap[m_maxHeap -> m_heapSize] = data;
		m_maxHeap -> bubbleUp(m_maxHeap -> m_heapSize);  
	}

	m_size++;
}

template <typename T>
T MedianHeap<T>::getMedian(){
	if (m_minHeap -> m_heapSize > m_maxHeap -> m_heapSize) {
		return m_minHeap -> m_heap[1];	
	} 
	if (m_maxHeap -> m_heapSize > m_minHeap -> m_heapSize) {
		return m_maxHeap -> m_heap[1];	
	} 
	return m_maxHeap -> m_heap[1];
}

template <typename T>
T MedianHeap<T>::getMin(){
	return m_min;
}

template <typename T>
T MedianHeap<T>::getMax(){
	return m_max;
}

template <typename T>
bool MedianHeap<T>::deleteItem(T& givenItem, bool (*equalTo) (const T&, const T&) ){
	if (m_size == 0) {
		throw out_of_range("The heap is empty");
	}
	bool checker = false;
	bool checkNewMax = false;
	bool checkNewMin = false;
	int hole;
	for (int i = 1; i < m_minHeap -> m_heapSize + 1; i++) {
		if (equalTo(givenItem, m_minHeap -> m_heap[i])) {
			givenItem = m_minHeap -> m_heap[i];
			m_minHeap -> m_heap[i] = m_minHeap -> m_heap[m_minHeap -> m_heapSize];
			hole = i;
			checker = true;
			if (equalTo(givenItem, m_max)) {
				// delete in min, so check m_max
				checkNewMax = true;
			}
			break;
		}
	}

	if (checker == true) {
		if (hole != 1 && m_minHeap -> compare(m_minHeap -> m_heap[hole / 2], m_minHeap -> m_heap[hole]) == true) {
			m_minHeap -> m_heapSize = m_minHeap -> m_heapSize - 1;
		    m_minHeap -> trickleDown(hole);
		    //m_minHeap -> m_heapSize = m_minHeap -> m_heapSize - 1;
		} else {
			m_minHeap -> m_heapSize = m_minHeap -> m_heapSize - 1;
		    m_minHeap -> bubbleUp(hole);
		}
		// m_minHeap -> m_heapSize = m_minHeap -> m_heapSize - 1;
		// m_minHeap -> trickleDown(hole);
		
		if (checkNewMax == true) {
			m_max = m_minHeap -> m_heap[1];
			for (int i = 1; i < m_minHeap -> m_heapSize + 1; i++) {
				if (m_minHeap -> compare(m_minHeap -> m_heap[i], m_max) == false) {
					m_max = m_minHeap -> m_heap[i];
				}
			}
		}
	} else {
		for (int i = 1; i < m_maxHeap -> m_heapSize + 1; i++) {
			if (equalTo(givenItem, m_maxHeap -> m_heap[i])) {
				givenItem = m_maxHeap -> m_heap[i];
				m_maxHeap -> m_heap[i] = m_maxHeap -> m_heap[m_maxHeap -> m_heapSize];
				hole = i;
				checker = true;
				if (equalTo(givenItem, m_min)) {
					//delete in max, so check m_min
					checkNewMin = true;
				}
				break;
			}
		}
		if (checker == true) {
			if (hole != 1 &&  m_maxHeap -> compare(m_maxHeap -> m_heap[hole / 2], m_maxHeap -> m_heap[hole]) == true) {
				m_maxHeap -> m_heapSize = m_maxHeap -> m_heapSize - 1;
				m_maxHeap -> trickleDown(hole);
				//m_maxHeap -> m_heapSize = m_maxHeap -> m_heapSize - 1;
			} else {
				m_maxHeap -> m_heapSize = m_maxHeap -> m_heapSize - 1;
			    m_maxHeap -> bubbleUp(hole);
			}
			// m_maxHeap -> m_heapSize = m_maxHeap -> m_heapSize - 1;
			// 	m_maxHeap -> trickleDown(hole);

			if (checkNewMin == true) {
				m_min = m_maxHeap -> m_heap[1];
				for (int i = 1; i < m_maxHeap -> m_heapSize + 1; i++) {
					if (m_maxHeap -> compare(m_maxHeap -> m_heap[i], m_min) == false) {
						m_min = m_maxHeap -> m_heap[i];
					}
				}
			}
		}
	}

	if (checker == true) {

		if (m_maxHeap -> m_heapSize - m_minHeap -> m_heapSize > 1) {
			T data = m_maxHeap -> m_heap[1];

			m_maxHeap -> m_heap[1] = m_maxHeap -> m_heap[m_maxHeap -> m_heapSize];
			m_maxHeap -> m_heapSize = m_maxHeap -> m_heapSize - 1;
			m_maxHeap -> trickleDown(1);
			//m_maxHeap -> m_heapSize = m_maxHeap -> m_heapSize - 1;

			m_minHeap -> m_heapSize = m_minHeap -> m_heapSize + 1;
			m_minHeap -> m_heap[m_minHeap -> m_heapSize] = data;
			m_minHeap -> bubbleUp(m_minHeap -> m_heapSize); 

		} else if (m_minHeap -> m_heapSize - m_maxHeap -> m_heapSize > 1) {
			T data = m_minHeap -> m_heap[1];

			m_minHeap -> m_heap[1] = m_minHeap -> m_heap[m_minHeap -> m_heapSize];
			m_minHeap -> m_heapSize = m_minHeap -> m_heapSize - 1;
			m_minHeap -> trickleDown(1);
			//m_minHeap -> m_heapSize = m_minHeap -> m_heapSize - 1;

			m_maxHeap -> m_heapSize = m_maxHeap -> m_heapSize + 1;
			m_maxHeap -> m_heap[m_maxHeap -> m_heapSize] = data;
			m_maxHeap -> bubbleUp(m_maxHeap -> m_heapSize);  
		}
		m_size--;
		return true;
	} else {
		return false;
	}

}


template <typename T>
void MedianHeap<T>::dump(){
	cout << "----------Max Heap----------" << endl;
	cout << "size = " << m_maxHeap -> m_heapSize << ", capacity = " << m_maxHeap -> m_heapCapacity << endl;;
	for (int i = 1; i < m_maxHeap -> m_heapSize + 1; i++) {
		cout << "Heap[" << i << "] = (" << m_maxHeap -> m_heap[i] << ")"<< endl; 
	}

	cout << "----------Min Heap----------" << endl;
	cout << "size = " << m_minHeap -> m_heapSize << ", capacity = " << m_minHeap -> m_heapCapacity << endl;;
	for (int i = 1; i < m_minHeap -> m_heapSize + 1; i++) {
		cout << "Heap[" << i << "] = (" << m_minHeap -> m_heap[i] << ")"<< endl; 
	}

	cout << "min    = " << getMin() << endl;
	cout << "median = " << getMedian() << endl;
	cout << "max    = " << getMax() << endl;
}

template <typename T>
int MedianHeap<T>::maxHeapSize(){
	return m_maxHeap -> m_heapSize;
}

template <typename T>
int MedianHeap<T>::minHeapSize(){
	return m_minHeap -> m_heapSize;
}

template <typename T>
T MedianHeap<T>::locateInMinHeap(int pos){
	return m_minHeap -> m_heap[pos];
}

template <typename T>
T MedianHeap<T>::locateInMaxHeap(int pos){
	return m_maxHeap -> m_heap[pos];
}

template <typename T>
Heap<T>::Heap(){
	m_heapSize = 0;
	m_heap = NULL;
	m_heapCapacity = 0;
}

template <typename T>
Heap<T>::~Heap(){
	m_heapSize = 0;
	if (m_heap!= NULL){
		delete [] m_heap;
		m_heap = NULL;
	}
}

template <typename T>
Heap<T>::Heap(int capacity){
	m_heapSize = 0;
	m_heap = new T[capacity+1];
	m_heapCapacity = capacity / 2 + 2;
}

template <typename T>
void Heap<T>::bubbleUp(int hole){
	while(hole != 1 && compare(m_heap[hole],m_heap[hole / 2]) == true){
		T temp = m_heap[hole / 2];
		m_heap[hole / 2] = m_heap[hole];
		m_heap[hole] = temp;
		hole = hole / 2;
	}
}

template <typename T>
void Heap<T>::trickleDown(int hole){
	int child;
	//child = hole;
	T temp = m_heap[hole];
	for (; hole * 2 <= m_heapSize; hole = child) {
		child = hole * 2;

		// check which side has higher priority
		if (child != m_heapSize && compare(m_heap[child + 1], m_heap[child]) == true) {
			child++;
		}


		if (compare(m_heap[child], temp) == true) {
			m_heap[hole] = m_heap[child];
			m_heap[child] = temp;
	    } else {
	    	m_heap[hole] = temp;
	    	break;
	    }
	}
}

#endif

