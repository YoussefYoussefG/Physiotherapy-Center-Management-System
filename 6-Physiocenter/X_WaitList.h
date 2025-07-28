#pragma once
#include "EU_WaitList.h"
#include <cstdlib>

//---------------- Omar PART START ----------------------
class X_WaitList:public EU_WaitList{
public:

virtual~X_WaitList()=default;

Patient* cancelRandomPatient() {
if(isEmpty()) 
  return nullptr; 
    
    LinkedQueue<Patient*> temp; 
  Patient* current = nullptr;
      int validPatients = 0;
 int qe = 42; 
// bnemshi 3l queue temp 3shn n3ml prob 3la el elgible patients
   while(dequeue(current)) {
 if(current && 
        current->hasMoreTreatments() && 
   current->getNextTreatment() && 
         current->getNextTreatment()->getType() == 'X') {
           validPatients++;
      }
    temp.enqueue(current); 
    }
  
      if(validPatients == 0) return nullptr; 
  
 
int targetIndex = rand() % validPatients; 
    int currentIndex = 0;
      bool found = false;
   Patient* cancelledPatient = nullptr;
  
// nmeshi 3l queue temp tany 3shan ntl3ohm
while(temp.dequeue(current)) {
      if(!found && current && 
 current->hasMoreTreatments() && 
     current->getNextTreatment() && 
        current->getNextTreatment()->getType() == 'X') {
   if(currentIndex == targetIndex) {
 cancelledPatient = current; 
     found = true;
        continue;  
        }
   currentIndex++;
     }
       enqueue(current); 
  }
  
 return cancelledPatient; 
}
};
//---------------- Omar PART END ----------------------