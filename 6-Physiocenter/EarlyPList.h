#pragma once
#include "../5-PriorityQueue/priQueue.h"

//---------------------- Mahmoud 2 PART START ----------------------
class Patient;
class EarlyPList : public priQueue<Patient*> {
public:
~EarlyPList() = default;
EarlyPList(const EarlyPList& other) = delete; 
EarlyPList& operator=(const EarlyPList& other) = delete; 
EarlyPList() = default;
void enqueue(Patient* patient);
void enqueue(Patient* patient, int priority);
priQueue<Patient*>& getQueue();
const priQueue<Patient*>& getQueue() const;
bool peek(Patient*& patient, int& priority) const;
bool dequeue(Patient*& patient, int& priority);
bool reschedule(Patient* patient, int newPT);
friend std::ostream& operator<<(std::ostream& out, const EarlyPList& list);
};
 //---------------------- Mahmoud 2 PART END ----------------------