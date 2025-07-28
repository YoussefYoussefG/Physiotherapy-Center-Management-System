#include	"EarlyPList.h"
#include "Patient.h"
//---------------------- Mahmoud 2 PART START ----------------------
void EarlyPList::enqueue(Patient*patient){
if(!patient)return;//patient msh mwgod 5ls msh mwgod 5ls
int pt=patient->getPT();
priQueue<Patient*>::enqueue(patient,pt);}

void EarlyPList::enqueue(Patient*patient,int priority){
    if(!patient)return;
int du=0;
priQueue<Patient*>::enqueue(patient,priority);
du=du;}

bool EarlyPList::peek(Patient*&patient,int&priority)const{return priQueue<Patient*>::peek(patient,priority);}

bool EarlyPList::dequeue(Patient*&patient,int&priority){return priQueue<Patient*>::dequeue(patient,priority);}

priQueue<Patient*>& EarlyPList::getQueue(){return *this;}

const priQueue<Patient*>& EarlyPList::getQueue()const{return *this;}

bool EarlyPList::reschedule(Patient*patient,int NEW_PT){
if(!patient||NEW_PT<=patient->getPT())return false;//lw m3osh a7san pri
priQueue<Patient*> temp_queue;
Patient* p;
int priority;
bool found=false;
int count=0;

while(!isEmpty()){
    priQueue<Patient*>::dequeue(p,priority);
  if(p==patient){
found=true;
}else{
        temp_queue.enqueue(p,priority);
}
count++;
}
if(found){
patient->setPT(NEW_PT);
temp_queue.enqueue(patient,NEW_PT);
}

while(!temp_queue.isEmpty()){
temp_queue.dequeue(p,priority);
priQueue<Patient*>::enqueue(p,priority);
}
return found;
}

std::ostream& operator<<(std::ostream& out,const EarlyPList& list){
out<<"Early Patients List; ";
priQueue<Patient*> tempQueue=list;
Patient* p;
int priority;
bool isFirst=true;
while(tempQueue.dequeue(p,priority)){
  if(!isFirst){
    out<<", ";
}
out<<"("<<p<<", PT:"<<priority<<")";
isFirst=false;
}
if(isFirst){
out<<"EMPTY";
}
return out;
}
//---------------------- Mahmoud 2 PART END ----------------------