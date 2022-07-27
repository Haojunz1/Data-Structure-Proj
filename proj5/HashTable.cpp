// File: HashTable.cpp
// Name: April Zhu
// E-mail: haojunz1@umbc.edu
// Description: cpp file for proj 5
// Fall 2018
// 
//
#ifndef HASHTABLE_CPP
#define HASHTABLE_CPP


#include <cstring>
#include <iostream>
#include <cstddef>
#include <stdexcept>
#include <vector>
#include "HashTable.h"
//#include "words.h"
#include "primes.cpp"


HashTable::HashTable(int n) {
  if (n < 101) {
    n = 101;	
  }
  m_capacityOne = roundUpPrime(n);
  m_sizeOne = 0;
  m_whichTable = 1;
  m_rehashing = false;
  if (m_capacityOne > 199999 || n > 199999) {
    throw range_error("out of range");
  }
  m_tableOne = new char* [m_capacityOne];
  for(int i = 0; i < m_capacityOne; i++) {
    m_tableOne[i] = NULL;
  }
  //
  m_tableTwo = NULL;
  m_temp = NULL;
}

char * const HashTable::DELETED  = (char *) &DELETED;
unsigned int HashTable::hashCode(const char *str) {
  
  unsigned int val = 0 ;
  const unsigned int thirtyThree = 33 ;  // magic number from textbook
  
  int i = 0 ;
  while (str[i] != '\0') {
    val = val * thirtyThree + str[i] ;
    i++ ;
  }
  return val ;
}


HashTable::~HashTable() {
  
  for (int i = 0; i < m_capacityOne; i++){
    if(m_tableOne[i] != NULL && m_tableOne[i] != DELETED){
      free(m_tableOne[i]);
      m_tableOne[i] = NULL;
    }else{
      m_tableOne[i] = NULL;
    }
  }
  
  delete [] m_tableOne;
  m_tableOne = NULL;
  
  if(m_rehashing == true){
    for (int i = 0; i < m_capacityTwo; i++){
      if(m_tableTwo[i] != NULL && m_tableTwo[i] != DELETED){
	free(m_tableTwo[i]);
	m_tableTwo[i] = NULL;
      }else{
	m_tableTwo[i] = NULL;
      }
    }
    
    delete [] m_tableTwo;
    m_tableTwo = NULL;
    
    if (m_temp != NULL){
      for (int i = 0; i < m_capacityOne; i++) {
	if(m_temp[i] != NULL && m_temp[i] != DELETED){
	  free(m_temp[i]);
	  m_temp[i] = NULL;
	}else{
	  m_temp[i] = NULL;
	}
      }
      delete [] m_temp;
      m_temp = NULL;    
    }
  }
}
HashTable::HashTable (HashTable& other) {
  // if (other.isRehashing()) {
  //   m_sizeOne = other.m_sizeOne;
  //   m_capacityOne = other.m_capacityOne;
  //   m_tableOne = new char* [m_capacityOne];
  //   for (int i = 0 i < m_capacityOne; i++) {
  //     if(other.m_tableOne[i] != NULL && other.m_tableOne[i] != DELETED) {
  //       char* temp = strdup(other.m_tableOne[i]);
  //       m_tableOne[i] = temp;
  //       free(temp);
  //     }else{
  //       m_tableOne[i] = NULL; 
  //     }
  //   } 
  //   m_rehashing = false;
  //   m_tableTwo =NULL;
  // }else {

  // }

  m_rehashing = other.m_rehashing;
  m_whichTable = other.m_whichTable;
  m_sizeOne = other.m_sizeOne;
  m_capacityOne = other.m_capacityOne;
  m_sizeTwo = other.m_sizeTwo;
  m_capacityTwo = other.m_capacityTwo;

  m_tableOne = new char* [m_capacityOne];
  
  for(int i = 0; i < m_capacityOne; i++) {
    if(other.m_tableOne[i] != NULL) {
      char* temp = strdup(other.m_tableOne[i]);
      m_tableOne[i] = temp;
      free(temp);
      // m_tableOne[i] = other.m_tableOne[i];
    }else{
      m_tableOne = NULL;
      
    }
  // for(int i = 0; i < m_capacityOne; i++) {
  //   if(other.m_tableOne[i] != NULL) {
  //       char* temp = strdup(other.m_tableOne[i]);
  //       m_tableOne[i] = temp;
  //       free(temp);
    // m_tableOne[i] = other.m_tableOne[i];
    //}
  }
  
  if(m_rehashing == true) {
    m_tableTwo = new char* [m_capacityTwo];
    
    for(int i = 0; i < m_capacityTwo; i++) {
      m_tableTwo[i] = NULL;
    }
    
    for(int i = 0; i < m_capacityTwo; i++) {
      if(other.m_tableTwo[i] != NULL) {
	char* temp = strdup(other.m_tableTwo[i]);
	m_tableTwo[i] = temp;
	free(temp);
      }
    }
  } 
}

const HashTable& HashTable::operator= (HashTable& rhs) {
  if (this != &rhs) {
    m_rehashing = rhs.m_rehashing;
    m_whichTable = rhs.m_whichTable;
    m_sizeOne = rhs.m_sizeOne;
    m_capacityOne = rhs.m_capacityOne;
    m_sizeTwo = rhs.m_sizeTwo;
    m_capacityTwo = rhs.m_capacityTwo;

    m_tableOne = new char* [m_capacityOne];
    for(int i = 0; i < m_capacityOne; i++) {
      m_tableOne[i] = NULL;
    }
    for (int i = 0; i < m_capacityOne; i++) {
      if(rhs.m_tableOne[i] != NULL) {
        char* temp = strdup(rhs.m_tableOne[i]);
        m_tableOne[i] = temp;
        free(temp);
      }
    }
  }
  if(m_rehashing == true) {
    m_tableTwo = new char* [m_capacityTwo];
    
    for(int i = 0; i < m_capacityTwo; i++) {
      m_tableTwo[i] = NULL;
    }
    for(int i = 0; i < m_capacityTwo; i++) {
      if(rhs.m_tableTwo[i] != NULL) {
        char* temp = strdup(rhs.m_tableTwo[i]);
        m_tableTwo[i] = temp;
        free(temp);
      }
    }
  }  
  return *this;
}

void HashTable::insert(const char *str) {
  // check load factor to see if the rehash is on
  if (m_rehashing == false) {
    checkRehash(str);
  }
  if (m_rehashing == false) {
    if (m_whichTable == 1) {
      int hashIndex = hashCode(str) % m_capacityOne;
      while(m_tableOne[hashIndex] != NULL) {
	if (strcmp(m_tableOne[hashIndex], DELETED) == 0 || strcmp(m_tableOne[hashIndex], str) == 0) {
	  break;
	}
	hashIndex = (hashIndex + 1) % m_capacityOne;
      }
      if (m_tableOne[hashIndex] == NULL) {
	m_tableOne[hashIndex] = strdup(str);
	m_sizeOne++;
      } else if (strcmp(m_tableOne[hashIndex], DELETED) == 0) {
	m_tableOne[hashIndex] = strdup(str);
	m_sizeOne++;
      } 
    } else if (m_whichTable == 2) {
      int hashIndex = hashCode(str) % m_capacityTwo;
      while(m_tableTwo[hashIndex] != NULL) {
	if (strcmp(m_tableTwo[hashIndex], DELETED) == 0 || strcmp(m_tableTwo[hashIndex], str) == 0) {
	  break;
	}
	hashIndex = (hashIndex + 1) % m_capacityTwo;
      }
      if (m_tableTwo[hashIndex] == NULL) {
	m_tableTwo[hashIndex] = strdup(str);
	m_sizeTwo++;
      } else if (strcmp(m_tableTwo[hashIndex], DELETED) == 0) {
	m_tableTwo[hashIndex] = strdup(str);
	m_sizeTwo++;
      } 	
    }
  } else {
    bool reHash = checkRehashForThird(str);
    if (reHash == true) {
      m_rehashing = false;
      int hashIndex = hashCode(str) % m_capacityOne;
      while(m_tableOne[hashIndex] != NULL) {
	if (strcmp(m_tableOne[hashIndex], DELETED) == 0 || strcmp(m_tableOne[hashIndex], str) == 0) {
	  break;
	}
	hashIndex = (hashIndex + 1) % m_capacityOne;
      }
      if (m_tableOne[hashIndex] == NULL) {
	m_tableOne[hashIndex] = strdup(str);
	m_sizeOne++;
      } else if (strcmp(m_tableOne[hashIndex], DELETED) == 0) {
	m_tableOne[hashIndex] = strdup(str);
	m_sizeOne++;
      } 
    } else {
      rehash(str);
      if (m_whichTable == 1) {
	int hashIndex = hashCode(str) % m_capacityOne;
	while(m_tableOne[hashIndex] != NULL) {
	  if (strcmp(m_tableOne[hashIndex], DELETED) == 0 || strcmp(m_tableOne[hashIndex], str) == 0) {
	    break;
	  }
	  hashIndex = (hashIndex + 1) % m_capacityOne;
	}
	if (m_tableOne[hashIndex] == NULL) {
	  m_tableOne[hashIndex] = strdup(str);
	  m_sizeOne++;
	} else if (strcmp(m_tableOne[hashIndex], DELETED) == 0) {
	  m_tableOne[hashIndex] = strdup(str);
	  m_sizeOne++;
	} 
      } else if (m_whichTable == 2) {
	int hashIndex = hashCode(str) % m_capacityTwo;
	while(m_tableTwo[hashIndex] != NULL) {
	  if (strcmp(m_tableTwo[hashIndex], DELETED) == 0 || strcmp(m_tableTwo[hashIndex], str) == 0) {
	    break;
					}
	  hashIndex = (hashIndex + 1) % m_capacityTwo;
	}
	if (m_tableTwo[hashIndex] == NULL) {
	  m_tableTwo[hashIndex] = strdup(str);
	  m_sizeTwo++;
	} else if (strcmp(m_tableTwo[hashIndex], DELETED) == 0) {
	  m_tableTwo[hashIndex] = strdup(str);
	  m_sizeTwo++;
	} 	
      }
    }
  }
  wrapUp();
}

bool HashTable::find(const char *str) {
  if (m_rehashing == false) {
    checkRehash(str);
  }
  if (m_rehashing == false) {
    if (m_whichTable == 1) {
      int hashIndex = hashCode(str) % m_capacityOne;
      while (m_tableOne[hashIndex] != NULL) {
	if (strcmp(m_tableOne[hashIndex], str) == 0) {
	  return true;
	}
	hashIndex = (hashIndex + 1) % m_capacityOne;
      }
    } else {
      int hashIndex = hashCode(str) % m_capacityTwo;
      while (m_tableTwo[hashIndex] != NULL) {
	if (strcmp(m_tableTwo[hashIndex], str) == 0) {
	  return true;
	}
	hashIndex = (hashIndex + 1) % m_capacityTwo;
      }
    }
  } else {
    rehash(str);
    int hashIndex = hashCode(str) % m_capacityOne;
    while (m_tableOne[hashIndex] != NULL) {
      if (strcmp(m_tableOne[hashIndex], str) == 0) {
	wrapUp();
	return true;
      }
      hashIndex = (hashIndex + 1) % m_capacityOne;
    }
    hashIndex = hashCode(str) % m_capacityTwo;
    while (m_tableTwo[hashIndex] != NULL) {
      if (strcmp(m_tableTwo[hashIndex], str) == 0) {
	wrapUp();
	return true;
      }
      hashIndex = (hashIndex + 1) % m_capacityTwo;
    }
    wrapUp();
  }
  return false;
}

char* HashTable::remove(const char *str) {
  if (m_rehashing == false) {
    checkRehash(str);
  }
  if (m_rehashing == false) {
    if (m_whichTable == 1) {
      int hashIndex = hashCode(str) % m_capacityOne;
      while (m_tableOne[hashIndex] != NULL) {
	if (strcmp(m_tableOne[hashIndex], str) == 0) {
	  char* temp = m_tableOne[hashIndex];
	  // if (m_tableOne[hashIndex] != NULL) {
	  // 	free(m_tableOne[hashIndex]);
	  // 	m_tableOne[hashIndex] = NULL;
	  // }
	  m_tableOne[hashIndex] = DELETED;
	  m_sizeOne--;
	  return temp;
	}
	hashIndex = (hashIndex + 1) % m_capacityOne;
      }
    } else {
      int hashIndex = hashCode(str) % m_capacityTwo;
      while (m_tableTwo[hashIndex] != NULL) {
	if (strcmp(m_tableTwo[hashIndex], str) == 0) {
	  char* temp = m_tableTwo[hashIndex];
	  if (m_tableTwo[hashIndex] != NULL) {
	  	//free(m_tableTwo[hashIndex]);
	  	m_tableTwo[hashIndex] = NULL;
	  }
	  m_tableTwo[hashIndex] = DELETED;
	  m_sizeTwo--;
	  return temp;
	}
	hashIndex = (hashIndex + 1) % m_capacityTwo;
      }
    }
  } else {
    rehash(str);
		int hashIndex = hashCode(str) % m_capacityOne;
		while (m_tableOne[hashIndex] != NULL) {
		  if (strcmp(m_tableOne[hashIndex], str) == 0) {
		    char* temp = m_tableOne[hashIndex];
		    if (m_tableOne[hashIndex] != NULL) {
		    	//free(m_tableOne[hashIndex]);
		    	m_tableOne[hashIndex] = NULL;
		    }
		    m_tableOne[hashIndex] = DELETED;
		    m_sizeOne--;
		    wrapUp();
		    return temp;
		  }
		  hashIndex = (hashIndex + 1) % m_capacityOne;
		}
		hashIndex = hashCode(str) % m_capacityTwo;
		while (m_tableTwo[hashIndex] != NULL) {
		  if (strcmp(m_tableTwo[hashIndex], str) == 0) {
		    char* temp = m_tableTwo[hashIndex];
		    if (m_tableTwo[hashIndex] != NULL) {
		    	//s(m_tableTwo[hashIndex]);
		    	m_tableTwo[hashIndex] = NULL;
		    }
		    m_tableTwo[hashIndex] = DELETED;
		    m_sizeTwo--;
		    wrapUp();
		    return temp;
		  }
		  hashIndex = (hashIndex + 1) % m_capacityTwo;
		}
  }
  //char* temp = NULL;
  wrapUp();
  return NULL;
}

void HashTable::rehash(const char* str) {
  if (m_whichTable == 1) {
    int hashIndex = hashCode(str) % m_capacityTwo;
    int temp = (hashIndex + m_capacityTwo - 1) % m_capacityTwo;
    while(m_tableTwo[hashIndex] != NULL) {
      if (strcmp(m_tableTwo[hashIndex], DELETED) != 0) {
	int hashIndex2 = hashCode(m_tableTwo[hashIndex]) % m_capacityOne;
	while (m_tableOne[hashIndex2] != NULL) {
	  if (strcmp(m_tableOne[hashIndex2], DELETED) == 0 || strcmp(m_tableOne[hashIndex2], str) == 0) {
	    break;
	  }
	  hashIndex2 = (hashIndex2 + 1) % m_capacityOne;
	}
	if (m_tableOne[hashIndex2] == NULL) {
	  //cout << "try to move " << strdup(m_tableTwo[hashIndex]) << "to tabeOne" << endl;
	  m_tableOne[hashIndex2] = /*strdup*/(m_tableTwo[hashIndex]);
	  m_sizeOne++;
	  //m_sizeTwo--;
	  //free(m_tableTwo[hashIndex]);
	} else if (strcmp(m_tableOne[hashIndex2], DELETED) == 0) {
	  //cout << "try to move " << strdup(m_tableTwo[hashIndex]) << "to tabeOne" << endl;
	  m_tableOne[hashIndex2] = /*strdup*/(m_tableTwo[hashIndex]);
	  m_sizeOne++;
	  //m_sizeTwo--;
	  //free(m_tableTwo[hashIndex]);
	} 
      }
      if (m_tableTwo[hashIndex] != NULL) {
	//free(m_tableTwo[hashIndex]);
	m_tableTwo[hashIndex] = NULL;
      }
      m_sizeTwo--;
      hashIndex = (hashIndex + 1) % m_capacityTwo;
    }
    hashIndex = temp;
    while(m_tableTwo[hashIndex] != NULL) {
      if (strcmp(m_tableTwo[hashIndex], DELETED) != 0) {
	int hashIndex2 = hashCode(m_tableTwo[hashIndex]) % m_capacityOne;
	while (m_tableOne[hashIndex2] != NULL) {
	  if (strcmp(m_tableOne[hashIndex2], DELETED) == 0 || strcmp(m_tableOne[hashIndex2], str) == 0) {
	    break;
	  }
	  hashIndex2 = (hashIndex2 + 1) % m_capacityOne;
	}
	if (m_tableOne[hashIndex2] == NULL) {
	  //cout << "try to move " << strdup(m_tableTwo[hashIndex]) << "to tabeOne" << endl;
	  m_tableOne[hashIndex2] = /*strdup*/(m_tableTwo[hashIndex]);
	  m_sizeOne++;
	  //m_sizeTwo--;
	  //free(m_tableTwo[hashIndex]);
	} else if (strcmp(m_tableOne[hashIndex2], DELETED) == 0) {
	  //cout << "try to move " << strdup(m_tableTwo[hashIndex]) << "to tabeOne" << endl;
	  m_tableOne[hashIndex2] = /*strdup*/(m_tableTwo[hashIndex]);
	  m_sizeOne++;
	  //m_sizeTwo--;
	  //free(m_tableTwo[hashIndex]);
	} 
      }
      if (m_tableTwo[hashIndex] != NULL) {
	//free(m_tableTwo[hashIndex]);
	m_tableTwo[hashIndex] = NULL;
      }
      //以上
      m_sizeTwo--;
      hashIndex = (hashIndex + m_capacityTwo - 1) % m_capacityTwo;
    }
  } else {
    int hashIndex = hashCode(str) % m_capacityOne;
    int temp = (hashIndex + m_capacityOne - 1) % m_capacityOne;
    while(m_tableOne[hashIndex] != NULL) {
      if (strcmp(m_tableOne[hashIndex], DELETED) != 0) {
	int hashIndex2 = hashCode(m_tableOne[hashIndex]) % m_capacityTwo;
	while (m_tableTwo[hashIndex2] != NULL) {
	  if (strcmp(m_tableTwo[hashIndex2], DELETED) == 0 || strcmp(m_tableTwo[hashIndex2], str) == 0) {
	    break;
					}
	  hashIndex2 = (hashIndex2 + 1) % m_capacityTwo;
	}
	if (m_tableTwo[hashIndex2] == NULL) {
	  //cout << "try to move " << strdup(m_tableOne[hashIndex]) << "to tabeTwo" << endl;
	  m_tableTwo[hashIndex2] = /*strdup*/(m_tableOne[hashIndex]);
	  m_sizeTwo++;
	  //m_sizeOne--;
	} else if (strcmp(m_tableTwo[hashIndex2], DELETED) == 0) {
	  //cout << "try to move " << strdup(m_tableOne[hashIndex]) << "to tabeTwo" << endl;
	  m_tableTwo[hashIndex2] = /*strdup*/(m_tableOne[hashIndex]);
	  m_sizeTwo++;
	  //m_sizeOne--;
	} 
      }
      if (m_tableOne[hashIndex] != NULL) {
	//free(m_tableOne[hashIndex]);
	m_tableOne[hashIndex] = NULL;
      }
      //以上
      m_sizeOne--;
      hashIndex = (hashIndex + 1) % m_capacityOne;
    }
    hashIndex = temp;
    while(m_tableOne[hashIndex] != NULL) {
      if (strcmp(m_tableOne[hashIndex], DELETED) != 0) {
	int hashIndex2 = hashCode(m_tableOne[hashIndex]) % m_capacityTwo;
	while (m_tableTwo[hashIndex2] != NULL) {
	  if (strcmp(m_tableTwo[hashIndex2], DELETED) == 0 || strcmp(m_tableTwo[hashIndex2], str) == 0) {
	    break;
	  }
	  hashIndex2 = (hashIndex2 + 1) % m_capacityTwo;
	}
	if (m_tableTwo[hashIndex2] == NULL) {
	  //cout << "try to move " << strdup(m_tableOne[hashIndex]) << "to tabeTwo" << endl;
	  m_tableTwo[hashIndex2] = /*strdup*/(m_tableOne[hashIndex]);
	  m_sizeTwo++;
	  //m_sizeOne--;
	} else if (strcmp(m_tableTwo[hashIndex2], DELETED) == 0) {
	  //cout << "try to move " << strdup(m_tableOne[hashIndex]) << "to tabeTwo" << endl;
	  m_tableTwo[hashIndex2] = /*strdup*/(m_tableOne[hashIndex]);
	  m_sizeTwo++;
	  //m_sizeOne--;
	} 
      }
      if (m_tableOne[hashIndex] != NULL) {
	//free(m_tableOne[hashIndex]);
	m_tableOne[hashIndex] = NULL;
      }
      //以上
      m_sizeOne--;
      hashIndex = (hashIndex + m_capacityOne - 1) % m_capacityOne;
    }	
  }
}

void HashTable::checkRehash(const char *str) {
  if (m_whichTable == 1) {
    if (m_tableOne != NULL) {
      int hashIndex = hashCode(str) % m_capacityOne;
      double loadFactor = double(m_sizeOne) / double(m_capacityOne);
      if (loadFactor > 0.5) {
	m_rehashing = true;
      }
      int count = 1;
      while(m_tableOne[hashIndex] != NULL && count < 10) {
	if (strcmp(m_tableOne[hashIndex], DELETED) == 0 || strcmp(m_tableOne[hashIndex], str) == 0) {
	  break;
	}
	count++;
	hashIndex = (hashIndex + 1) % m_capacityOne;
      }
      if (count >= 10) {
	m_rehashing = true;
      }
      if (m_rehashing == true) {
	int newCapacity = roundUpPrime(4 * m_sizeOne);
	if (newCapacity == m_capacityOne) {
	  newCapacity = roundUpPrime(m_capacityOne + 1);
	}
	if (newCapacity > 199999) {
	  throw range_error("out of range");
	}
	
	m_tableTwo = new char* [newCapacity];
	// cout << "table2 created" << endl;
	for (int i = 0; i < newCapacity; i++) {
	  m_tableTwo[i] = NULL;
	}
	m_capacityTwo = newCapacity;
	m_sizeTwo = 0;
	m_whichTable = 2;
	
      }	
    }
  } else if (m_whichTable == 2) {
    if (m_tableTwo != NULL) {
      int hashIndex = hashCode(str) % m_capacityTwo;
      double loadFactor = double(m_sizeTwo) / double(m_capacityTwo);
      if (loadFactor > 0.5) {
	m_rehashing = true;
      }
      int count = 1;
      while(m_tableTwo[hashIndex] != NULL && count < 10) {
	if (strcmp(m_tableTwo[hashIndex], DELETED) == 0 || strcmp(m_tableTwo[hashIndex], str) == 0) {
	  break;
	}
	count++;
	hashIndex = (hashIndex + 1) % m_capacityTwo;
      }
      if (count >= 10) {
	m_rehashing = true;
      }
      if (m_rehashing == true) {
	int newCapacity = roundUpPrime(4 * m_sizeTwo);
	if (newCapacity == m_capacityTwo) {
	  newCapacity = roundUpPrime(m_capacityTwo + 1);
	}
	if (newCapacity > 199999) {
	  throw range_error("out of range");
	}
	//debug try
	m_tableOne = new char* [newCapacity];
	for (int i = 0; i < newCapacity; i++) {
	  m_tableOne[i] = NULL;
	}
	m_capacityOne = newCapacity;
	m_sizeOne = 0;
	m_whichTable = 1;
      }
    }
  }
}

bool HashTable::checkRehashForThird(const char* str) {
  bool reHash = false;
  double loadFactor1 = double(m_sizeOne) / double(m_capacityOne);
  double loadFactor2 = double(m_sizeTwo) / double(m_capacityTwo);
  if (loadFactor1 > 0.5 && loadFactor2 > 0.5) {
    reHash = true;
  } else {
    if (m_whichTable == 1) {
      int hashIndex = hashCode(str) % m_capacityOne;
      int count = 1;
      while(m_tableTwo[hashIndex] != NULL && count < 10) {
	if (strcmp(m_tableTwo[hashIndex], DELETED) == 0 || strcmp(m_tableTwo[hashIndex], str) == 0) {
	  break;
	}
	count++;
	hashIndex = (hashIndex + 1) % m_capacityOne;
      }
      if (count >= 10) {
	reHash = true;
      }	
    } else if (m_whichTable == 2) {
      int hashIndex = hashCode(str) % m_capacityTwo;
      int count = 1;
      while(m_tableTwo[hashIndex] != NULL && count < 10) {
	if (strcmp(m_tableTwo[hashIndex], DELETED) == 0 || strcmp(m_tableTwo[hashIndex], str) == 0) {
	  break;
	}
	count++;
	hashIndex = (hashIndex + 1) % m_capacityTwo;
      }
      if (count >= 10) {
	reHash = true;
      }
    }
  }
  
  if (reHash == true) {
    int newCapacity = roundUpPrime(4 * (m_sizeOne + m_sizeTwo + 1));
    int tempSize = 0;
    if (newCapacity > 199999) {
      throw range_error("out of range");
    }
    
    m_temp = new char* [newCapacity];
    
    for (int i = 0; i < m_capacityOne; i++) {
      if (m_tableOne[i] != NULL) {
	int hashIndex = hashCode(m_tableOne[i]) % newCapacity;
	while(m_temp[hashIndex] != NULL) {
	  if (strcmp(m_temp[hashIndex], DELETED) == 0 || strcmp(m_temp[hashIndex], m_tableOne[i]) == 0) {
	    break;
	  }
	  hashIndex = (hashIndex + 1) % newCapacity;
	}
	if (m_temp[hashIndex] == NULL) {
	  m_temp[hashIndex] = /*strdup*/(m_tableOne[i]);
	  tempSize++;
	} else if (strcmp(m_temp[hashIndex], DELETED) == 0) {
	  m_temp[hashIndex] = /*strdup*/(m_tableOne[i]);
	  tempSize++;
	} 
	if (m_tableOne[i] != NULL) {
		//free(m_tableOne[i]);
	   	m_tableOne[i] = NULL;
	}
	//以上
      }
    }
    
    for (int i = 0; i < m_capacityTwo; i++) {
      if (m_tableTwo[i] != NULL) {
	int hashIndex = hashCode(m_tableTwo[i]) % newCapacity;
	while(m_temp[hashIndex] != NULL) {
	  if (strcmp(m_temp[hashIndex], DELETED) == 0 || strcmp(m_temp[hashIndex], m_tableTwo[i]) == 0) {
	    break;
	  }
	  hashIndex = (hashIndex + 1) % newCapacity;
	}
	if (m_temp[hashIndex] == NULL) {
	  m_temp[hashIndex] = /*strdup*/(m_tableTwo[i]);
					tempSize++;
	} else if (strcmp(m_temp[hashIndex], DELETED) == 0) {
	  m_temp[hashIndex] = /*strdup*/(m_tableTwo[i]);
	  tempSize++;
	} 
	if (m_tableTwo[i] != NULL) {
		//free(m_tableTwo[i]);
	   	m_tableTwo[i] = NULL;
	}
	//以上
      }
    }
    delete [] m_tableOne;
    m_tableOne = NULL;
    delete [] m_tableTwo;
    m_tableTwo = NULL;
    
    m_capacityOne = newCapacity;
    m_sizeOne = tempSize;
    m_whichTable = 1;
    m_capacityTwo = 0;
    m_sizeTwo = 0;
    
    m_tableOne = new char* [newCapacity];

    for (int i = 0; i < newCapacity; i++) {
      if (m_temp[i] != NULL && m_temp[i] != DELETED) {
	m_tableOne[i] = /*strdup*/(m_temp[i]);
	//free(m_temp[i]);
	m_temp[i] = NULL;
      }	
    }
    //delete [] m_temp;
    m_temp = NULL;
  }
  
  return reHash;
}

void HashTable::wrapUp() {
  if (m_rehashing == true) {
    if (m_whichTable == 1) {
      double loadFactor = double(m_sizeTwo) / double(m_capacityTwo);
      if (loadFactor < 0.03) {
	for (int i = 0; i < m_capacityTwo; i++) {
	  if (m_tableTwo[i] != NULL && strcmp(m_tableTwo[i], DELETED) != 0) {
	    int hashIndex = hashCode(m_tableTwo[i]) % m_capacityOne;
	    while(m_tableOne[hashIndex] != NULL) {
	      if (strcmp(m_tableOne[hashIndex], DELETED) == 0 || strcmp(m_tableOne[hashIndex], m_tableTwo[i]) == 0) {
		break;
	      }
	      hashIndex = (hashIndex + 1) % m_capacityOne;
	    }
	    if (m_tableOne[hashIndex] == NULL) {
	      m_tableOne[hashIndex] = strdup(m_tableTwo[i]);
	      m_sizeOne++;
	    } else if (strcmp(m_tableOne[hashIndex], DELETED) == 0) {
	      m_tableOne[hashIndex] = strdup(m_tableTwo[i]);
	      m_sizeOne++;
	    } 
	  }
	  if (m_tableTwo[i] != NULL && m_tableTwo[i] != DELETED) {
	  	//free(m_tableTwo[i]);
	  	m_tableTwo[i] = NULL;	
	  }
	  //以上
	}
	//delete [] m_tableTwo;
	m_tableTwo = NULL;
	m_sizeTwo = 0;
	m_capacityTwo = 0;
	m_rehashing = false;
      }
    } else {
      double loadFactor = double(m_sizeOne) / double(m_capacityOne);
      if (loadFactor < 0.03) {
	for (int i = 0; i < m_capacityOne; i++) {
	  if (m_tableOne[i] != NULL && strcmp(m_tableOne[i], DELETED) != 0) {
	    int hashIndex = hashCode(m_tableOne[i]) % m_capacityTwo;
	    while(m_tableTwo[hashIndex] != NULL) {
	      if (strcmp(m_tableTwo[hashIndex], DELETED) == 0 || strcmp(m_tableTwo[hashIndex], m_tableOne[i]) == 0) {
		break;
	      }
	      hashIndex = (hashIndex + 1) % m_capacityTwo;
						}
	    if (m_tableTwo[hashIndex] == NULL) {
	      m_tableTwo[hashIndex] = strdup(m_tableOne[i]);
	      m_sizeTwo++;
	    } else if (strcmp(m_tableTwo[hashIndex], DELETED) == 0) {
	      m_tableTwo[hashIndex] = strdup(m_tableOne[i]);
	      m_sizeTwo++;
	    } 
	  }
	  if (m_tableOne[i] != NULL && m_tableOne[i] != DELETED) {
	  	//free(m_tableOne[i]);
	  	m_tableOne[i] = NULL;	
	  }
	  //以上
	  // free(m_tableOne[i]);
	  // m_tableOne[i] = NULL;		
	}
	//delete [] m_tableOne;
	m_tableOne = NULL;
	m_sizeOne = 0;
	m_capacityOne = 0;
	m_rehashing = false;
      }	
    }
  }
}

void HashTable:: dump() {
  if (m_rehashing == false) {
    if (m_whichTable == 1) {
      cout << "HashTable #1 : " << "size = " << m_sizeOne << " TableSize =" << m_capacityOne << endl;
      for (int i = 0; i < m_capacityOne; i++){
	if (m_tableOne[i] == NULL) {
	  cout << "H1[" << i << "] = " << endl;
	} else {
	  if (m_tableOne[i] == DELETED) {
	    cout << "H1[" << i << "] = DELETED" << endl;	
	  } else {
	    int originalIndex = hashCode((m_tableOne[i])) % m_capacityOne;
	    cout << "H1[" << i << "] = " << (m_tableOne[i]) << " (" << originalIndex << ")"<< endl;
	  }		
	}
      }	
    } else {
      cout << "HashTable #2 : " << "size = " << m_sizeTwo << " TableSize =" << m_capacityTwo << endl;
      for (int i = 0; i < m_capacityTwo; i++){
	if (m_tableTwo[i] == NULL) {
	  cout << "H2[" << i << "] = " << endl;
	} else {
	  if (m_tableTwo[i] == DELETED) {
	    cout << "H2[" << i << "] = DELETED" << endl;	
	  } else {
	    int originalIndex = hashCode((m_tableTwo[i])) % m_capacityTwo;
	    cout << "H2[" << i << "] = " << (m_tableTwo[i]) << " (" << originalIndex << ")"<< endl;
	  }
	}
      }	
    }
  } else {
    if (m_whichTable == 2) {
      if (m_tableOne != NULL) {
	cout << "HashTable #1 : " << "size = " << m_sizeOne << " TableSize =" << m_capacityOne << endl;
	for (int i = 0; i < m_capacityOne; i++){
	  if (m_tableOne[i] == NULL) {
	    cout << "H1[" << i << "] = " << endl;
	  } else {
	    if (m_tableOne[i] == DELETED) {
	      cout << "H1[" << i << "] = DELETED" << endl;	
	    } else {
	      int originalIndex = hashCode((m_tableOne[i])) % m_capacityOne;
	      cout << "H1[" << i << "] = " << (m_tableOne[i]) << " (" << originalIndex << ")"<< endl;
	    }
	    
	  }
	}
      }
      cout << "HashTable #2 : " << "size = " << m_sizeTwo << " TableSize =" << m_capacityTwo << endl;
      for (int i = 0; i < m_capacityTwo; i++){
	if (m_tableTwo[i] == NULL) {
	  cout << "H2[" << i << "] = " << endl;
	} else {
	  if (m_tableTwo[i] == DELETED) {
	    cout << "H2[" << i << "] = DELETED" << endl;	
	  } else {
	    int originalIndex = hashCode((m_tableTwo[i])) % m_capacityTwo;
	    cout << "H2[" << i << "] = " << (m_tableTwo[i]) << " (" << originalIndex << ")"<< endl;
	  }
	}
      }
    } else {
      if (m_tableTwo != NULL) {
	cout << "HashTable #2 : " << "size = " << m_sizeTwo << " TableSize =" << m_capacityTwo << endl;
	for (int i = 0; i < m_capacityTwo; i++){
	  if (m_tableTwo[i] == NULL) {
	    cout << "H2[" << i << "] = " << endl;
	  } else {
	    if (m_tableTwo[i] == DELETED) {
	      cout << "H2[" << i << "] = DELETED" << endl;	
	    } else {
	      int originalIndex = hashCode((m_tableTwo[i])) % m_capacityTwo;
	      cout << "H2[" << i << "] = " << (m_tableTwo[i]) << " (" << originalIndex << ")"<< endl;
	    }
	  }
	}	
      }
      cout << "HashTable #1 : " << "size = " << m_sizeOne << " TableSize =" << m_capacityOne << endl;
      for (int i = 0; i < m_capacityOne; i++){
	if (m_tableOne[i] == NULL) {
	  cout << "H1[" << i << "] = " << endl;
	} else {
	  if (m_tableOne[i] == DELETED) {
	    cout << "H1[" << i << "] = DELETED" << endl;	
	  } else {
	    int originalIndex = hashCode((m_tableOne[i])) % m_capacityOne;
	    cout << "H1[" << i << "] = " << (m_tableOne[i]) << " (" << originalIndex << ")"<< endl;
	  }		
	}
      }
    }
    
  }
  
}


bool HashTable::isRehashing() {
  return m_rehashing;
}

int HashTable::size(int table) {
  if (m_rehashing == false) {
    if (m_whichTable == 1) {
      return m_sizeOne;
    } else {
      return m_sizeTwo;
    }	
  }
  else {
    if (table == 0) {
      if (m_whichTable == 1) {
	return m_sizeTwo;
      } else {
	return m_sizeOne;
      }
    } else {
      if (m_whichTable == 1) {
	return m_sizeOne;
      } else {
	return m_sizeTwo;
      }	
    }	
  }
}

int HashTable::tableSize(int table) {
  if (m_rehashing == false) {
    if (m_whichTable == 1) {
      return m_capacityOne;
    } else {
      return m_capacityTwo;
    }	
  }
  else {
    if (table == 0) {
      if (m_whichTable == 1) {
	return m_capacityTwo;
      } else {
	return m_capacityOne;
      }
    } else {
      if (m_whichTable == 1) {
	return m_capacityOne;
      } else {
	return m_capacityTwo;
      }	
    }	
  }
}


const char * HashTable::at(int index, int table) {
  if (index < 0) {
    throw range_error("out of range");
  }
  
  if (m_rehashing == false) {
    if (m_whichTable == 1) {
      if (index >= m_capacityOne) {
	throw range_error("out of range");
      }
      if (m_tableOne[index] == NULL) {
	return NULL;
 
      }else {
	return (m_tableOne[index]);
      }
    } else {
      if (index >= m_capacityTwo) {
	throw range_error("out of range");
      }
      if (m_tableTwo[index] == NULL) {
	return NULL;

      }else {
	return (m_tableTwo[index]);
      }
    }	
  } else {
    if (table == 0) {
      if (m_whichTable == 1) {
	if (index >= m_capacityTwo) {
	  throw range_error("out of range");
	}
	if (m_tableTwo[index] == NULL) {
	  return NULL;

	}else {
	  return (m_tableTwo[index]);
	}
      } else {
	if (index >= m_capacityOne) {
	  throw range_error("out of range");
	}
	if (m_tableOne[index] == NULL) {
	  return NULL;

	}else {
	  return (m_tableOne[index]);
	}
      }
    } else {
      if (m_whichTable == 1) {
	if (index >= m_capacityOne) {
	  throw range_error("out of range");
	}
	if (m_tableOne[index] == NULL) {
	  return NULL;
 
	}else {
	  return (m_tableOne[index]);
	}
      } else {
	if (index >= m_capacityTwo) {
	  throw range_error("out of range");
	}
	if (m_tableTwo[index] == NULL) {
	  return NULL;

	}else {
	  return (m_tableTwo[index]);
	}
      }	
    }	
  }	
}


#endif
