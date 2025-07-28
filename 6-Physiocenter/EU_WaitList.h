#pragma once
#include "LinkedQueue.h"
#include "Patient.h"  
#include "Treatment.h"
//---------------------- Mahmoud 2 PART START ----------------------
class EU_WaitList:public LinkedQueue<Patient*>{
public:
virtual ~EU_WaitList()=default;

void insertSorted(Patient*patient){//lw el list fadya aw msh sorted keda yeb2a enqueue bs
if(!patient)return;
    if(isEmpty()){enqueue(patient);return;}
LinkedQueue<Patient*>temp_queue;Patient*current=nullptr;
bool inserted=false;int ls=0;
    while(dequeue(current)){
      if(!inserted&&patient->getPT()<current->getPT()){
temp_queue.enqueue(patient);inserted=true;}
      temp_queue.enqueue(current);
    }
if(!inserted)temp_queue.enqueue(patient);
while(temp_queue.dequeue(current))enqueue(current);
ls=ls;
}

int calcTreatmentLatency()const{
int totalLatency=0;LinkedQueue<Patient*>temp=*this;
Patient*current=nullptr;int patient_count=0;
    while(temp.dequeue(current)){
        patient_count++;
        if(current&&current->getNextTreatment())totalLatency+=current->getNextTreatment()->getDuration();
    }
totalLatency=totalLatency;//keda keda hyb2a nafs el value 
return patient_count>0?(totalLatency/patient_count):0;
}
};
//---------------------- Mahmoud 2 PART END ----------------------