// File: Driver.cpp                                                                         
// Name: April Zhu                                                                             
// E-mail: haojunz1@umbc.edu                                                                   
// Description: test file for proj 5                                                            
// Fall 2018  adapted from test2                                                                                 
//                                                                                             
//   


#include <iostream>
#include <cstdlib>
using namespace std ;

#include "HashTable.h"


int main() {
   HashTable T(107) ;

   T.insert("To") ;     
   T.insert("The") ;
   T.insert("Moon") ;       
   T.insert("By") ;    

   T.insert("Percy") ;       
   T.insert("Bysshe") ;     
   T.insert("Shelley") ;      

   T.insert("Art") ;          
   T.insert("thou") ;      
   T.insert("pale") ;       
   T.insert("for") ;    
   T.insert("weariness") ;     

   T.insert("Of") ;      
   T.insert("climbing") ;   
   T.insert("heaven") ;     
   T.insert("and") ;   
   T.insert("gazing") ;      

   T.insert("on") ;	
   T.insert("earth") ;	
   T.insert("wandering") ;	
   T.insert("companionless") ;	
   T.insert("Among") ;	
   T.insert("stars") ;	
   T.insert("that") ;	
   T.insert("have") ;	
   T.insert("a") ;	

   cout << "----------------------------------------------------\n" ;
   cout << "Original hash table\n" ;
   cout << "----------------------------------------------------\n" ;
   T.dump() ;
   cout << "----------------------------------------------------\n" ;


   cout << "\n\nDo some finds, inserts and removes\n\n" ;

   const char *str ;
   char *ptr ;

   if( T.find(str="gazing") ) {
      cout << "Found " << str << endl ;
   } else {
      cout << "Did not find " << str << endl ;
   }

   if( T.find(str="joyless") ) {
      cout << "Found " << str << endl ;
   } else {
      cout << "Did not find " << str << endl ;
   }

   ptr = T.remove(str="changing") ;
   if (ptr == NULL) {
      cout << "String " << str << " not found, not removed\n" ;
   } else {
      cout << "Removed string = " << ptr << endl ;
      free(ptr) ;
   }

   ptr = T.remove(str="weariness") ;
   if (ptr == NULL) {
      cout << "String " << str << " not found, not removed\n" ;
   } else {
      cout << "Removed string = " << ptr << endl ;
      free(ptr) ;
   }

   if( T.find(str="pale") ) {
      cout << "Found " << str << endl ;
   } else {
      cout << "Did not find " << str << endl ;
   }

   T.insert("birth") ;     
   T.insert("like") ; 
   T.insert("eye") ;     
   T.insert("joyless") ;    


   cout << "----------------------------------------------------\n" ;
   cout << "Hash table after more insertions.\n" ;
   cout << "----------------------------------------------------\n" ;
   T.dump() ;
   cout << "----------------------------------------------------\n" ;

   cout << "\n\nA find on \"joyless\" \n" ;

   if( T.find(str="joyless") ) {
      cout << "Found " << str << endl ;
   } else {
      cout << "Did not find " << str << endl ;
   }

   if( T.find(str="reaction") ) {
      cout << "Found " << str << endl ;
   } else {
      cout << "Did not find " << str << endl ;
   }

   T.insert("undulation") ;     
   T.insert("impertinent") ;    
   T.insert("maladies") ;       
   T.insert("dominions") ;	

   T.insert("waspish") ;        
   T.insert("wildebeest") ;     
   T.insert("reaction") ;       

   T.insert("pawns") ;          
   T.insert("vacuously") ;      
   T.insert("firth") ;          
   T.insert("manipulator") ;    
   T.insert("dreariness") ;     

   T.insert("insanity") ;       
   T.insert("enthronement") ;   
   T.insert("falsifiers") ;     
   T.insert("ignominious") ;    
   T.insert("mummified") ;      

   T.insert("tributes") ;	
   T.insert("skulduggery") ;
   T.insert("frothed") ;	
   T.insert("horrify") ;	
   T.insert("blackmailers") ;	
   T.insert("defenestrated") ;	
   T.insert("garrison") ;	
   T.insert("lidless") ;

   cout << "----------------------------------------------------\n" ;
   cout << "Hash table after some finds and insertions\n" ;
   cout << "----------------------------------------------------\n" ;
   T.dump() ;
   cout << "----------------------------------------------------\n" ;
}
