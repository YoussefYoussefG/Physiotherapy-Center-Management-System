#pragma once
#include "Treatment.h"
#include "LinkedQueue.h"
#include <string>
#include <iostream>
#include <climits>

//---------------------- OmarPART START ----------------------

class Scheduler;


enum class PatientStatus {
    IDLE,   
    ERLY,   
    LATE,   
    WAIT,   
    SERV,   
    FNSH    
};

class Patient {
private:
    std::string ID;              
    char type;                   
    int PT;                      
    int VT;                      
    PatientStatus status;        
    LinkedQueue<Treatment*> treatments;  
    bool didCancel;             
    bool didReschedule;         
    int waitingTime;            
    int treatmentTime;          
    Resource* currentResource;  
    int finishTime;             
    
    friend std::ostream& operator<<(std::ostream& out, const Patient* patient);
    friend std::ostream& operator<<(std::ostream& out, const Patient& patient);

public:
    
    Patient(const std::string& id, char typ, int pt, int vt);
    
    ~Patient();
    
    
    Patient(const Patient&) = delete;
    Patient& operator=(const Patient&) = delete;
    
    
    std::string getID() const { return ID; }
    char getType() const { return type; }
    int getPT() const { return PT; }
    int getVT() const { return VT; }
    PatientStatus getStatus() const { return status; }
    bool getDidCancel() const { return didCancel; }
    bool getDidReschedule() const { return didReschedule; }
    Resource* getCurrentResource() const { return currentResource; }
    int getFinishTime() const { return finishTime; }
    
    
    void setPT(int newPT) { PT = newPT; }
    void setStatus(PatientStatus newStatus) { status = newStatus; }
    void setDidCancel(bool val) { didCancel = val; }
    void setDidReschedule(bool val) { didReschedule = val; }
    void setCurrentResource(Resource* r) { currentResource = r; }
    void updateFinishTime(int currentTime);
    
      
    void addTreatment(Treatment* treatment);
    Treatment* getNextTreatment();
    Treatment* getOptimalTreatment(Scheduler* scheduler); 
    bool removeTreatment(Treatment* treatment);
    bool hasMoreTreatments() const;
    bool hasTreatment(Treatment* treatment) const; 
    
    
    void addWaitingTime(int time) { waitingTime += time; }
    void addTreatmentTime(int time) { treatmentTime += time; }
    int getWaitingTime() const { return waitingTime; }
    int getTreatmentTime() const { return treatmentTime; }
};
 //---------------------- Omar PART END ----------------------