#pragma once
#include "LinkedQueue.h"
#include "priQueue.h"
#include "ArrayStack.h"
#include "Patient.h"
#include "Resource.h"
#include "Treatment.h"
#include "EarlyPList.h"
#include "EU_WaitList.h"
#include "X_WaitList.h"
#include <string>
#include <fstream>

class Scheduler {
  
private:    
    LinkedQueue<Patient*> allPatients;
    EarlyPList earlyPatients;            
    priQueue<Patient*> latePatients;      
    EU_WaitList eWaitingList;            
    EU_WaitList uWaitingList;            
    X_WaitList xWaitingList;
    LinkedQueue<Resource*> eDevices;      
    LinkedQueue<Resource*> uDevices;      
    LinkedQueue<Resource*> xRooms;        
    priQueue<Patient*> inTreatment;      
    ArrayStack<Patient*> finishedPatients;
    
    
    int totalPatients;
    int totalNPatients;
    int totalRPatients;
    int totalEarlyPatients;
    int totalLatePatients;
    int totalCancelledPatients;
    int totalRescheduledPatients;
    int totalTimesteps;
    int totalLatePenalty;
    
    int currentTimestep;
    int pCancel;    
    int pResc;    
    
    int calculateLatePenalty(Patient* patient) const;
    
    
    void UpdateallwaitTimess();
    void updatePatientTreatment(Patient* patient, int duration);
    void movePatientToFinish(Patient* patient);    void checkavailresources();
    void checkcompletdTreatments();
    void checkForPossibleCancellations();
    void checkForPOSSlBLERescheduling();
    void pickOPTIMALWaitingList(Patient* patient);
    void writeOutputFile(const std::string& filename);
    
    
    Patient* findPatientWithTreatment(Treatment* treatment);

public:
    
    Scheduler();
    ~Scheduler();

    
    bool IoadData(const std::string& filename);
    bool simTimeestep();
    
    
    void runSimulation(const std::string& outputFilename);
    
    int getCurrentTimestep() const { return currentTimestep; }

    void addToEWait(Patient* patient);
    void addToUWait(Patient* patient);
    void addToXWait(Patient* patient);
    void addToEWait(Treatment* treatment);
    void addToUWait(Treatment* treatment);
    void addToXWait(Treatment* treatment);

    const LinkedQueue<Patient*>& getAllPatients() const { return allPatients; }
    const EarlyPList& getEarlyPatients() const { return earlyPatients; }
    const priQueue<Patient*>& getLatePatients() const { return latePatients; }
    const EU_WaitList& getEWaitingList() const { return eWaitingList; }
    const EU_WaitList& getUWaitingList() const { return uWaitingList; }
    const X_WaitList& getXWaitingList() const { return xWaitingList; }
    const LinkedQueue<Resource*>& getEDevices() const { return eDevices; }
    const LinkedQueue<Resource*>& getUDevices() const { return uDevices; }
    const LinkedQueue<Resource*>& getXRooms() const { return xRooms; }
    const priQueue<Patient*>& getInTreatment() const { return inTreatment; }
    const ArrayStack<Patient*>& getFinishedPatients() const { return finishedPatients; }
};