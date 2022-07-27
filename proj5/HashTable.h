// File: HashTable.h                                                                        
// Name: April Zhu                                                                            
// E-mail: haojunz1@umbc.edu                                                                  
// Description: heder file for proj 5                                                         
// Fall 2018                                                                                  
//                                                                                            
//   
#ifndef HASHTABLE_H
#define HASHTABLE_H

using namespace std;

class HashTable {
	public:
		static char * const DELETED ;
		bool isRehashing() ;
	    int tableSize(int table=0) ;
	    int size(int table=0) ;
		const char * at(int index, int table=0) ;

		HashTable(int n=101);
		~HashTable();
		HashTable (HashTable& other) ;
		const HashTable& operator= (HashTable& rhs) ;

		unsigned int hashCode(const char* str);
		void insert(const char* str) ;
		bool find(const char* str) ;
		char* remove(const char *str) ;
		void rehash(const char* str);
		void wrapUp();
		void checkRehash(const char* str);
		bool checkRehashForThird(const char* str);
		void dump();
	private:
		char ** m_tableOne;
		char ** m_tableTwo;
		char ** m_temp;
		bool m_rehashing = false;
		int m_whichTable = 1;
		int m_sizeOne = 0;
		int m_capacityOne = 0;
		int m_sizeTwo = 0;
		int m_capacityTwo = 0;
};

#endif
