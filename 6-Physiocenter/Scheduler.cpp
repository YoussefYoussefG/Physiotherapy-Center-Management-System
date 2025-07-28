#include "Scheduler.h"
#include <cstdlib>  
#include <iostream>
  #include <iomanip>
#include <algorithm>

Scheduler::Scheduler() : 
    currentTimestep(-1), //Test Fixed (start point problem )
      pCancel(0),  //error here
    pResc(0),
      totalPatients(0),
    totalNPatients(0),
      totalRPatients(0),
    totalEarlyPatients(0),
        totalLatePatients(0),
    totalCancelledPatients(0),
      totalRescheduledPatients(0),
    totalTimesteps(0),
      totalLatePenalty(0) {}


Scheduler::~Scheduler() {
    Patient* p;
      while(allPatients.dequeue(p)) delete p;
    int priority;
      while(earlyPatients.dequeue(p, priority)) delete p;
    priQueue<Patient*> tempLate = latePatients;
        while(tempLate.dequeue(p, priority)) delete p;
      while(eWaitingList.dequeue(p)) delete p;
    while(uWaitingList.dequeue(p)) delete p;
        while(xWaitingList.dequeue(p)) delete p;
    while(inTreatment.dequeue(p, priority)) delete p;
      while(finishedPatients.pop(p)) delete p;
    Resource* r;
        while(eDevices.dequeue(r)) delete r;
    while(uDevices.dequeue(r)) delete r;
      while(xRooms.dequeue(r)) delete r;
}


bool Scheduler::IoadData(const std::string& filename) {
  std::ifstream inFile(filename);
    if (!inFile.is_open()) return false;
      int numE,numU,numRooms;
    inFile >> numE >> numU >> numRooms;
      int temp = 0; 
  for (int i=0; i<numRooms; ++i) {
        int cap;
      inFile >> cap;
        xRooms.enqueue(new Resource('X', i + 1, cap));
    }
      for (int i = 0; i < numE; ++i) 
    eDevices.enqueue(new Resource('E', i + 1));
        for (int i = 0; i < numU; ++i) 
      uDevices.enqueue(new Resource('U', i+1));
    inFile >> pCancel >> pResc;
      int patientCount;
    inFile >> patientCount;
      totalPatients = patientCount;
  for (int j=0; j < patientCount; ++j) {
      char pType;
        int pt, vt, nt;
      inFile >> pType >> pt >> vt >> nt;
    std::string pid = std::to_string(j + 1);
        Patient* patient = new Patient(pid, pType, pt, vt);
      if (pType == 'N') ++totalNPatients;
    else if (pType == 'R') ++totalRPatients;
        for (int k = 0; k<nt; ++k) {
          char tType;
        int duration;
          inFile >> tType >> duration;
        Treatment* treatment = nullptr;
            switch(tType) {
              case 'E': treatment = new ETreatment(duration); break;
            case 'U': treatment = new UTreatment(duration); break;
                case 'X': treatment = new XTreatment(duration); break;
              default: break;
          }
        if (treatment) patient->addTreatment(treatment);
      }
 


    allPatients.enqueue(patient);
    }

  priQueue<Patient*> sortQ;
  Patient* tmp;
  while (allPatients.dequeue(tmp)) {
      sortQ.enqueue(tmp, -tmp->getVT());  //Test Fixed : sorted the all patient list
  }
  int vt;
  while (sortQ.dequeue(tmp, vt)) {
      allPatients.enqueue(tmp);
  }
  /////////////////// Sort the allPatients queue based on VT////////////////////////////
      inFile.close();
  return true;
}

int Scheduler::calculateLatePenalty(Patient* patient) const {
    return (patient->getVT() - patient->getPT()) / 2; 
}
//---------------------- Youssef PART START ----------------------
// el sim
bool Scheduler::simTimeestep() {
    
  if (allPatients.isEmpty() && earlyPatients.isEmpty() && 
        latePatients.isEmpty() && eWaitingList.isEmpty() &&
      uWaitingList.isEmpty() && xWaitingList.isEmpty() &&
        inTreatment.isEmpty()) {
    return false; 
    }
      static int noChangeCount = 0;
    static int prevFinishedCount = 0;
      int step = currentTimestep;
    if (step > 120) {
      int totalProcessed = finishedPatients.getCount();
        if (totalProcessed == prevFinishedCount && totalProcessed > 0) {
          noChangeCount++;
        if (noChangeCount > 20) return false;
      } else {            
            noChangeCount = 0;
          prevFinishedCount = totalProcessed;
        }
    }
    //---------------------- Youssef PART END ----------------------

      UpdateallwaitTimess();
      //---------------------- Mahmoud PART START ----------------------
         //hn3ml process le arriving patients - FROM ALL to Early/Late/Appropriate waiting list

    Patient* p;
  while (!allPatients.isEmpty()) {
        allPatients.peek(p);
      if (p->getVT() > currentTimestep+1) break;
    allPatients.dequeue(p);
      if (p->getVT() < p->getPT()) {
          p->setStatus(PatientStatus::ERLY);
        earlyPatients.enqueue(p,p->getPT());
          totalEarlyPatients++;
      } 
        else if (p->getVT() > p->getPT()) {
      p->setStatus(PatientStatus::LATE);
          int penalty = calculateLatePenalty(p);
        totalLatePenalty+=penalty;
      latePatients.enqueue(p, p->getPT() + penalty);
          totalLatePatients++;
    } else {  
          p->setStatus(PatientStatus::WAIT);
        if (p->getType() == 'R') pickOPTIMALWaitingList(p);
          else {
        Treatment* t = p->getNextTreatment();
            if (t) {
          char type = t->getType();
            if (type == 'E') addToEWait(p);
            else if (type == 'U') addToUWait(p);
            else if (type == 'X') addToXWait(p);
            else addToEWait(p);
            p->removeTreatment(t);
          } else if (p->hasMoreTreatments()) addToEWait(p);
            else movePatientToFinish(p);
        }
      }
    }
  //dol late

      Patient* late;
    int priority;
      priQueue<Patient*> tempLate;
    while (!latePatients.isEmpty()) {
      latePatients.peek(late, priority);
        int penaltyTime = priority - late->getPT();
      if (currentTimestep >= late->getVT() + penaltyTime) {
          latePatients.dequeue(late, priority);
        late->setStatus(PatientStatus::WAIT);
          if (late->getType() == 'R') pickOPTIMALWaitingList(late);
        else {
            Treatment* t = late->getNextTreatment();
          if (t) {
              char type = t->getType();
            if (type == 'E') addToEWait(late);
              else if (type == 'U') addToUWait(late);
            else if (type == 'X') addToXWait(late);
              else addToEWait(late);
            late->removeTreatment(t);
          } else if (late->hasMoreTreatments()) addToEWait(late);
            else movePatientToFinish(late);
        }
      } else {
          latePatients.dequeue(late, priority);
        tempLate.enqueue(late, priority);
      }
    }
      while (!tempLate.isEmpty()) {        
    tempLate.dequeue(late, priority);
        latePatients.enqueue(late, priority);
    }
      //dol early
      Patient* early;
    priQueue<Patient*> tempEarlyProcessing;
      while (!earlyPatients.isEmpty()) {
    earlyPatients.dequeue(early, priority);
        if (currentTimestep >= early->getPT()) {
      early->setStatus(PatientStatus::WAIT);
          if (early->getType() == 'R') pickOPTIMALWaitingList(early);
        else {
            Treatment* t = early->getNextTreatment();
          if (t) {
              char type = t->getType();
            if (type == 'E') addToEWait(early);
              else if (type == 'U') addToUWait(early);
            else if (type == 'X') addToXWait(early);
              else addToEWait(early);
            early->removeTreatment(t);
          } else if (early->hasMoreTreatments()) addToEWait(early);
            else movePatientToFinish(early);
        }
      } else tempEarlyProcessing.enqueue(early, priority);
    }
      while (!tempEarlyProcessing.isEmpty()) {        
    tempEarlyProcessing.dequeue(early, priority);
        earlyPatients.enqueue(early, priority);
    }
      //---------------------- Mahmoud PART END ----------------------

      checkForPOSSlBLERescheduling();
    checkavailresources();
      checkcompletdTreatments();
    currentTimestep++;
      return true;
}

void Scheduler::UpdateallwaitTimess() {
    //---------------------- Mahmoud 2 PART START ----------------------
       //hnhades waiting time 
    priQueue<Patient*> tempEarly;
  Patient* p;
    int priority;
  int update_count = 0; 
      while(!earlyPatients.isEmpty()) {
    earlyPatients.dequeue(p, priority);
        p->addWaitingTime(1);
      tempEarly.enqueue(p, priority);
        update_count++;
    }
      while(!tempEarly.isEmpty()) {
    tempEarly.dequeue(p, priority);
        earlyPatients.enqueue(p, priority);
    }
      priQueue<Patient*> tempLate;
    while(latePatients.dequeue(p, priority)) {
        p->addWaitingTime(1);
      tempLate.enqueue(p, priority);
    }
      while(tempLate.dequeue(p, priority)) latePatients.enqueue(p, priority);
    LinkedQueue<Patient*> tempE;
      while(eWaitingList.dequeue(p)) {
    p->addWaitingTime(1);
        tempE.enqueue(p);
    }
      while(tempE.dequeue(p)) eWaitingList.enqueue(p);
    LinkedQueue<Patient*> tempU;
      while(uWaitingList.dequeue(p)) {
    p->addWaitingTime(1);
        tempU.enqueue(p);
    }
      while(tempU.dequeue(p)) uWaitingList.enqueue(p);
    LinkedQueue<Patient*> tempX;
      while(xWaitingList.dequeue(p)) {
    p->addWaitingTime(1);
        tempX.enqueue(p);
    }
      while(tempX.dequeue(p)) xWaitingList.enqueue(p);
    priQueue<Patient*> tempInTreatment;
      while(inTreatment.dequeue(p, priority)) {
    p->addTreatmentTime(1);
        tempInTreatment.enqueue(p, priority - 1);
    }
      inTreatment = tempInTreatment;
}
    // ---------------------- Mahmoud PART END ----------------------

void Scheduler::updatePatientTreatment(Patient* patient, int duration) {
    patient->addTreatmentTime(duration); 
}

void Scheduler::movePatientToFinish(Patient* patient) {
  if (!patient || patient == nullptr) return; 
      patient->setStatus(PatientStatus::FNSH);
    patient->updateFinishTime(currentTimestep);
      finishedPatients.push(patient);
}

void Scheduler::checkavailresources() {
    //---------------------- Mahmoud part start ----------------------
     //Assign X functionality and E/U functionality
  while (!xWaitingList.isEmpty() && !xRooms.isEmpty()) {
      Patient* patient = nullptr;
    Resource* resource = nullptr;
        xWaitingList.peek(patient);
      xRooms.peek(resource);
    if (patient && resource && !resource->isFull()) {        
          xWaitingList.dequeue(patient);
        Treatment* treatment = patient->getNextTreatment();
          if (treatment && treatment->getType() == 'X') {
        treatment->setAssignmentTime(currentTimestep);
            treatment->setResource(resource);
          if (resource->addPatient()) {
              xRooms.dequeue(resource);
            if (!resource->isFull()) xRooms.enqueue(resource);
              patient->setStatus(PatientStatus::SERV);
            patient->setCurrentResource(resource);
              inTreatment.enqueue(patient, treatment->getDuration());
          } else {
              patient->setCurrentResource(nullptr);
            treatment->setResource(nullptr);
              xWaitingList.enqueue(patient);
          }
        } else {
            if (treatment) {
          char tType = treatment->getType();
              if (tType == 'E') eWaitingList.insertSorted(patient);
            else if (tType == 'U') uWaitingList.insertSorted(patient);
              else xWaitingList.enqueue(patient);
          } else if (!patient->hasMoreTreatments()) movePatientToFinish(patient);
            else xWaitingList.enqueue(patient);
        }
      } else break;
    }
  
        //---------------------- Mahmoud PART START ----------------------

      while (!eWaitingList.isEmpty() && !eDevices.isEmpty()) {
    Patient* patient = nullptr;
        Resource* resource = nullptr;
      eWaitingList.peek(patient);
    eDevices.peek(resource);
        if (patient && resource && !resource->isFull()) {        
      eWaitingList.dequeue(patient);
          Treatment* treatment = patient->getNextTreatment();
        if (treatment && treatment->getType() == 'E') {
            treatment->setAssignmentTime(currentTimestep);
          treatment->setResource(resource);
            if (resource->addPatient()) {
          eDevices.dequeue(resource);
              eDevices.enqueue(resource);
            patient->setStatus(PatientStatus::SERV);
              patient->setCurrentResource(resource);
            inTreatment.enqueue(patient, treatment->getDuration());
          } else {
              patient->setCurrentResource(nullptr);
            treatment->setResource(nullptr);
              eWaitingList.enqueue(patient);
          }
        } else {
            if (treatment) {
          char tType = treatment->getType();
              if (tType == 'U') uWaitingList.insertSorted(patient);
            else if (tType == 'X') xWaitingList.insertSorted(patient);
              else eWaitingList.enqueue(patient);
          } else if (!patient->hasMoreTreatments()) movePatientToFinish(patient);
            else eWaitingList.enqueue(patient);
        }
      } else break;
    }
      while (!uWaitingList.isEmpty() && !uDevices.isEmpty()) {
    Patient* patient = nullptr;
        Resource* resource = nullptr;
      uWaitingList.peek(patient);
    uDevices.peek(resource);
        if (patient && resource && !resource->isFull()) {        
      uWaitingList.dequeue(patient);
          Treatment* treatment = patient->getNextTreatment();
        if (treatment && treatment->getType() == 'U') {
            treatment->setAssignmentTime(currentTimestep);
          treatment->setResource(resource);
            if (resource->addPatient()) {
          uDevices.dequeue(resource);
              uDevices.enqueue(resource);
            patient->setStatus(PatientStatus::SERV);
              patient->setCurrentResource(resource);
            inTreatment.enqueue(patient, treatment->getDuration());
          } else {
              patient->setCurrentResource(nullptr);
            treatment->setResource(nullptr);
              uWaitingList.enqueue(patient);
          }
        } else {
            if (treatment) {
          char tType = treatment->getType();
              if (tType == 'E') eWaitingList.insertSorted(patient);
            else if (tType == 'X') xWaitingList.insertSorted(patient);
              else uWaitingList.enqueue(patient);
          } else if (!patient->hasMoreTreatments()) movePatientToFinish(patient);
            else uWaitingList.enqueue(patient);
        }
      } else break;
    }
}
//---------------------- Mahmoud PART END ----------------------

void Scheduler::checkcompletdTreatments() {
    //---------------------- Youssef PART START ----------------------
     //el InTreat to wait/finish
     //btshof lw ay treatment 5elso
    priQueue<Patient*> tempInTreatment;
  Patient* patient;
      int remainingTime;
    while (inTreatment.dequeue(patient, remainingTime)) {
      if (remainingTime <= 0) {
          Resource* resource = patient->getCurrentResource();
        if (resource) {
            resource->removePatient();
          if (resource->getType() == 'X' && !resource->isFull()) {
              bool alreadyInQueue = false;
            LinkedQueue<Resource*> tempXRooms;
              Resource* tempResource;
            while (xRooms.dequeue(tempResource)) {
                if (tempResource->getID() == resource->getID()) alreadyInQueue = true;
              tempXRooms.enqueue(tempResource);
            }
              while (tempXRooms.dequeue(tempResource)) xRooms.enqueue(tempResource);
            if (!alreadyInQueue) xRooms.enqueue(resource);
          } 
            else if (resource->getType() == 'E') {
          bool alreadyInQueue = false;
              LinkedQueue<Resource*> tempEDevices;
            Resource* tempResource;
              while (eDevices.dequeue(tempResource)) {
            if (tempResource->getID() == resource->getID()) alreadyInQueue = true;
                tempEDevices.enqueue(tempResource);
            }
              while (tempEDevices.dequeue(tempResource)) eDevices.enqueue(tempResource);
            if (!alreadyInQueue) eDevices.enqueue(resource);
          }
            else if (resource->getType() == 'U') {
          bool alreadyInQueue = false;
              LinkedQueue<Resource*> tempUDevices;
            Resource* tempResource;
              while (uDevices.dequeue(tempResource)) {
            if (tempResource->getID() == resource->getID()) alreadyInQueue = true;
                tempUDevices.enqueue(tempResource);
            }
              while (tempUDevices.dequeue(tempResource)) uDevices.enqueue(tempResource);
            if (!alreadyInQueue) uDevices.enqueue(resource);
          }
            patient->setCurrentResource(nullptr);
        }
          if (patient->hasMoreTreatments()) {
        if (patient->getType() == 'R') {
              patient->setStatus(PatientStatus::WAIT);
            pickOPTIMALWaitingList(patient);
          } else {
              Treatment* t = patient->getNextTreatment();
            if (t) {    
                patient->setStatus(PatientStatus::WAIT);
              char type = t->getType();
                if (type == 'E') addToEWait(patient);
              else if (type == 'U') addToUWait(patient);
                else if (type == 'X') addToXWait(patient);
              else addToEWait(patient);
                patient->removeTreatment(t); 
            } else movePatientToFinish(patient);
          }
        } else movePatientToFinish(patient);
      } else tempInTreatment.enqueue(patient, remainingTime);
    }      
      inTreatment = tempInTreatment;
}
//---------------------- Youssef PART END ----------------------

void Scheduler::checkForPossibleCancellations() {
    // ---------------------- Youssef PART START ----------------------
    //el Cancel 
     
    int randNum = rand() % 100; 
  if (randNum < pCancel) {
      Patient* cancelPatient = xWaitingList.cancelRandomPatient();
    if (cancelPatient) {
          cancelPatient->setDidCancel(true);
        totalCancelledPatients++;
          movePatientToFinish(cancelPatient);
    }
  }
}
//---------------------- Youssef PART END ----------------------


void Scheduler::checkForPOSSlBLERescheduling() {
    //---------------------- Youssef PART START ----------------------
     //el reschedule
    if (pResc < 0) return; 
  int randNum = rand() % 100;
      if (randNum < pResc) {
    int earlyCount = 0;
        Patient* p = nullptr;
      int priority = 0;
    priQueue<Patient*> tempEarly;
        while (!earlyPatients.isEmpty()) {
      if (earlyPatients.dequeue(p, priority)) {
            if (p) {
          earlyCount++;
              tempEarly.enqueue(p, priority);
        }
      }
    }
        while (!tempEarly.isEmpty()) {
      if (tempEarly.dequeue(p, priority)) {
            if (p) earlyPatients.enqueue(p, priority);
        }
      }
    if (earlyCount > 0) {
          int targetIndex = rand() % earlyCount;
        int currentIndex = 0;
          priQueue<Patient*> newEarlyList;
        while (!earlyPatients.isEmpty()) {
            if (earlyPatients.dequeue(p, priority)) {
          if (p) {
                if (currentIndex == targetIndex) {
              int originalPT = p->getPT();
                  int newPT = originalPT + (rand() % 5) + 1;
                p->setPT(newPT);
                  p->setDidReschedule(true);
                totalRescheduledPatients++;
                  newEarlyList.enqueue(p, newPT);
            } else newEarlyList.enqueue(p, priority);
                currentIndex++;
          }
        }
      }
          while (!newEarlyList.isEmpty()) {
        if (newEarlyList.dequeue(p, priority)) {
              if (p) earlyPatients.enqueue(p, priority);
          }
        }
    }
  }
}
//---------------------- Youssef PART END ----------------------

void Scheduler::pickOPTIMALWaitingList(Patient* patient) {
    //--------------------- Omar PART START ----------------------
   //bnhandle rp patient lma n7oto fl waiting
    Treatment* nextTreatment = nullptr;
    nextTreatment = patient->getOptimalTreatment(this);
    if (nextTreatment) {
        char tType = nextTreatment->getType();
        switch (tType) {
        case 'E': addToEWait(patient); break;
        case 'U': addToUWait(patient); break;
        case 'X': addToXWait(patient); break;
        default: addToEWait(patient); break;
        }
        patient->removeTreatment(nextTreatment);
    }
    // ---------------------- Omar PART END ----------------------
}

void Scheduler::runSimulation(const std::string& outputFilename) {
    //---------------------- Youssef PART START ----------------------
     //main sim
     
  int prevStep = -1;
      int stuckCount = 0;
    int maxSteps = 200;
      int stepCount = 0;
    int noActivityCount = 0;
      while (simTimeestep() && stepCount < maxSteps) {
    if (currentTimestep == prevStep) {
          stuckCount++;
        if (stuckCount > 5) break;
      } else {
          prevStep = currentTimestep;
        stuckCount = 0;
      }
    if (allPatients.isEmpty() && earlyPatients.isEmpty() && 
          latePatients.isEmpty() && eWaitingList.isEmpty() && 
        uWaitingList.isEmpty() && xWaitingList.isEmpty()) {
          if (inTreatment.isEmpty()) {
        noActivityCount++;
            if (noActivityCount > 5) break;
        } else noActivityCount = 0;
      } else noActivityCount = 0;
        stepCount++;
    }
      // ---------------------- Nour PART ----------------------
    
      totalTimesteps = currentTimestep - 1;
    writeOutputFile(outputFilename);
}
// ---------------------- Nour PART START ----------------------
// output file generation

void Scheduler::writeOutputFile(const std::string& filename) {
    std::ofstream outFile(filename);
  if (!outFile.is_open()) return;
      outFile << "PID PType PT VT FT WT TT Cancel Resc\n";
    priQueue<Patient*> sortedPatients;
      Patient* p;
    ArrayStack<Patient*> tempStack = finishedPatients;
      while (!tempStack.isEmpty()) {
    tempStack.pop(p);
        sortedPatients.enqueue(p, -p->getFinishTime()); 
    }
      int priority;
    while (sortedPatients.dequeue(p, priority)) {
        outFile << "P" << p->getID() << " " << p->getType() << " " << p->getPT() << " " 
              << p->getVT() << " " << p->getFinishTime() << " " << p->getWaitingTime() << " " 
            << p->getTreatmentTime() << " " << (p->getDidCancel() ? "T" : "F") << " " 
              << (p->getDidReschedule() ? "T" : "F") << "\n";
    }
      outFile << "\nTotal number of timesteps = " << totalTimesteps << "\n";
    outFile << "Total number of all, N, and R patients = " << totalPatients << ", " 
          << totalNPatients << ", " << totalRPatients << "\n";
      double avgWaitAll = 0, avgWaitN = 0, avgWaitR = 0;
    double totalWaitAll = 0, totalWaitN = 0, totalWaitR = 0;
      int countN = 0, countR = 0;
    tempStack = finishedPatients;
      while (!tempStack.isEmpty()) {
    tempStack.pop(p);
        totalWaitAll += p->getWaitingTime();
      if (p->getType() == 'N') {
          totalWaitN += p->getWaitingTime();
        countN++;
      } else {
          totalWaitR += p->getWaitingTime();
        countR++;
      }
    }
      avgWaitAll = totalPatients > 0 ? totalWaitAll / totalPatients : 0;
    avgWaitN = countN > 0 ? totalWaitN / countN : 0;
      avgWaitR = countR > 0 ? totalWaitR / countR : 0;
    outFile << "Average total waiting time for all, N, and R patients = " 
          << std::fixed << std::setprecision(2) << avgWaitAll << ", " << avgWaitN << ", " << avgWaitR << "\n";
      double avgTreatAll = 0, avgTreatN = 0, avgTreatR = 0;
    double totalTreatAll = 0, totalTreatN = 0, totalTreatR = 0;
      countN = 0;
    countR = 0;
      tempStack = finishedPatients;
    while (!tempStack.isEmpty()) {
        tempStack.pop(p);
      totalTreatAll += p->getTreatmentTime();
        if (p->getType() == 'N') {
      totalTreatN += p->getTreatmentTime();
          countN++;
      } else {
          totalTreatR += p->getTreatmentTime();
        countR++;
      }
    }
      avgTreatAll = totalPatients > 0 ? totalTreatAll / totalPatients : 0;
    avgTreatN = countN > 0 ? totalTreatN / countN : 0;
      avgTreatR = countR > 0 ? totalTreatR / countR : 0;
    outFile << "Average total treatment time for all, N, and R patients = " 
          << std::fixed << std::setprecision(2) << avgTreatAll << ", " << avgTreatN << ", " << avgTreatR << "\n";
      double cancelPercent = totalPatients > 0 ? (totalCancelledPatients * 100.0) / totalPatients : 0;
    double reschedulePercent = totalPatients > 0 ? (totalRescheduledPatients * 100.0) / totalPatients : 0;
      if (reschedulePercent > 100.0) reschedulePercent = 100.0;
    double earlyPercent = totalPatients > 0 ? (totalEarlyPatients * 100.0) / totalPatients : 0;
      double latePercent = totalPatients > 0 ? (totalLatePatients * 100.0) / totalPatients : 0;
    outFile << "Percentage of patients of an accepted cancellation (%)= " 
          << std::fixed << std::setprecision(2) << cancelPercent << "%\n";
      outFile << "Percentage of patients of an accepted rescheduling (%)= " 
        << std::fixed << std::setprecision(2) << reschedulePercent << "%\n";
    outFile << "Percentage of early patients (%)= " 
          << std::fixed << std::setprecision(2) << earlyPercent << "%\n";
      outFile << "Percentage of late patients (%)= " 
        << std::fixed << std::setprecision(2) << latePercent << "%\n";
    double avgLatePenalty = totalLatePatients > 0 ? (totalLatePenalty * 1.0) / totalLatePatients : 0;
      outFile << "Average late penalty = " << std::fixed << std::setprecision(2) << avgLatePenalty << " timestep(s)\n";
    outFile.close();
}
// ---------------------- Nour PART END ----------------------

//---------------------- Mahmoud PART START ----------------------
 
void Scheduler::addToEWait(Patient* patient) {
    if (!patient) return; 
  eWaitingList.insertSorted(patient);
}

void Scheduler::addToUWait(Patient* patient) {
    if (!patient) return;
      uWaitingList.insertSorted(patient);
}

void Scheduler::addToXWait(Patient* patient) {
    if (!patient) return;
  xWaitingList.insertSorted(patient);
}

void Scheduler::addToEWait(Treatment* treatment) {
    if (!treatment) return;
      Patient* patient = findPatientWithTreatment(treatment);
    if (patient) addToEWait(patient);
}

void Scheduler::addToUWait(Treatment* treatment) {
    if (!treatment) return;
  Patient* patient = findPatientWithTreatment(treatment);
      if (patient) addToUWait(patient);
}

void Scheduler::addToXWait(Treatment* treatment) {
    if (!treatment) return;
      Patient* patient = findPatientWithTreatment(treatment);
    if (patient) addToXWait(patient);
}
//---------------------- Mahmoud PART END ----------------------
 
Patient* Scheduler::findPatientWithTreatment(Treatment* treatment) {
    if (!treatment) return nullptr; 
  LinkedQueue<Patient*> tempAll = allPatients;
      Patient* p;
    while (tempAll.dequeue(p)) {
      if (p->hasTreatment(treatment)) return p;
    }
      priQueue<Patient*> tempEarly = earlyPatients;
    int priority;
      while (tempEarly.dequeue(p, priority)) {
    if (p->hasTreatment(treatment)) return p;
      }
    priQueue<Patient*> tempLate = latePatients;
      while (tempLate.dequeue(p, priority)) {
    if (p->hasTreatment(treatment)) return p;
      }
    LinkedQueue<Patient*> tempE = eWaitingList;
      while (tempE.dequeue(p)) {
    if (p->hasTreatment(treatment)) return p;
      }
    LinkedQueue<Patient*> tempU = uWaitingList;
      while (tempU.dequeue(p)) {
    if (p->hasTreatment(treatment)) return p;
      }
    LinkedQueue<Patient*> tempX = xWaitingList;
      while (tempX.dequeue(p)) {
    if (p->hasTreatment(treatment)) return p;
      }
    priQueue<Patient*> tempIn = inTreatment;
      while (tempIn.dequeue(p, priority)) {
    if (p->hasTreatment(treatment)) return p;
      }
    return nullptr; 
}