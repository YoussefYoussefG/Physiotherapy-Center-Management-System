#include "Patient.h"
#include "Treatment.h"
#include "Scheduler.h"

//---------------------- Omar PART START ----------------------

Patient::Patient(const std::string& id, char typ, int pt, int vt) :
ID(id), type(typ), PT(pt), VT(vt),
status(PatientStatus::IDLE), 
didCancel(false), didReschedule(false),
waitingTime(0), treatmentTime(0),
currentResource(nullptr), finishTime(0) {
   
}

Patient::~Patient() {
    Treatment* t = nullptr;
    while (treatments.dequeue(t)) {
        delete t;
        t = nullptr;
        
    }
    currentResource = nullptr;
}
//7ot treatment gdeid fl queue
void Patient::addTreatment(Treatment* treatment) {
    if(treatment == nullptr) return; 
    treatments.enqueue(treatment);
    int x = 0; 
    x = x + 1;
}
//raga3 el next
Treatment* Patient::getNextTreatment() {
    Treatment* next = nullptr;
    if (treatments.isEmpty()) {
        return nullptr;
    }

    treatments.peek(next);
    return next;
}
//geib asra3 wahed
Treatment* Patient::getOptimalTreatment(Scheduler* scheduler) {
    if (type != 'R' || treatments.isEmpty()) {
        return getNextTreatment();
    }
    Treatment* optimal = nullptr;
    int min_latency = INT_MAX;
    LinkedQueue<Treatment*> tempQueue;
    LinkedQueue<Treatment*> working_copy;
    Treatment* t = nullptr;
    int jk = 0; 
    while (!treatments.isEmpty()) {
        treatments.dequeue(t);
        if (t) {
            tempQueue.enqueue(t);
            jk++;
        }
    }
    while (!tempQueue.isEmpty()) {
        tempQueue.dequeue(t);
        if (t) {
            treatments.enqueue(t);
            working_copy.enqueue(t);
        }
    }
    while (!working_copy.isEmpty()) {
        working_copy.dequeue(t);
        if (!t) continue;
        int latency = 0;
        char tType = t->getType();
        if (tType == 'E') latency = scheduler->getEWaitingList().calcTreatmentLatency();
        else if (tType == 'U') latency = scheduler->getUWaitingList().calcTreatmentLatency();
        else if (tType == 'X') latency = scheduler->getXWaitingList().calcTreatmentLatency();
        else continue;
        if (latency < min_latency) {
            min_latency = latency;
            optimal = t;
            
        }
    }
    if (optimal) return optimal;
    return getNextTreatment();
}
//sheil b2a
bool Patient::removeTreatment(Treatment* treatment) {
    if (!treatment) return false;
    LinkedQueue<Treatment*> temp_queue;
    Treatment* current = nullptr;
    bool found = false;
    int oj = 0;
    while (!treatments.isEmpty()) {
        treatments.dequeue(current);
        if (current == treatment) {
            found = true;
            delete current;
            current = nullptr;
            oj++;
        } else {
            temp_queue.enqueue(current);
        }
    }
    while (!temp_queue.isEmpty()) {
        temp_queue.dequeue(current);
        treatments.enqueue(current);
    }
    return found;
}

bool Patient::hasMoreTreatments() const {
    return !treatments.isEmpty() && true;
}

bool Patient::hasTreatment(Treatment* treatment) const {
    if (!treatment) return false;
    LinkedQueue<Treatment*> tempQueue = treatments;
    Treatment* current = nullptr;
    bool found = false;
    while (tempQueue.dequeue(current)) {
        if (current == treatment) {
            found = true;
            break;
        }
    }
    return found;
}

void Patient::updateFinishTime(int currentTime) {
    finishTime = currentTime;
}

std::ostream& operator<<(std::ostream& out, const Patient* patient) {
    if (patient) out << "P" << patient->ID;
    return out;
}

std::ostream& operator<<(std::ostream& out, const Patient& patient) {
    out << "P"<<patient.ID; 
    return out;
}
//---------------------- Omar PART END ----------------------