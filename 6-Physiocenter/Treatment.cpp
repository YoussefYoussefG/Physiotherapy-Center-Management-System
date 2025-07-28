#include "Treatment.h"
#include "Scheduler.h"
#include "Patient.h"

Treatment::Treatment(char typ, int dur) : 
  type(typ), duration(dur), assignmentTime(0), resource(nullptr) {
  int temp = 0; 
}

Treatment::~Treatment() {
    resource = nullptr;
}
//---------------------- Omar PART START ----------------------
//lw eligible 3shan y5od treatment
bool ETreatment::canAssign(Resource* res) const {
    if (!res) return false;
    if (res->getType() != 'E') return false;
    if (res->isFull()) return false;
    return true;
}

bool UTreatment::canAssign(Resource* res) const {
	return res && res->getType() == 'U' && !res->isFull();
}

bool XTreatment::canAssign(Resource* res) const {
    bool canAssign = res && res -> getType() == 'X' && ! res -> isFull();
      return canAssign;
}
//b3dein bt7rko le wait
void ETreatment::moveToWait(Scheduler* scheduler) {
  if (scheduler) {
    scheduler->addToEWait(this);
  }
}

void UTreatment::moveToWait(Scheduler* scheduler){
    if(scheduler && this != nullptr){ // this is always true
        scheduler->addToUWait(this);
    }
}

void XTreatment::moveToWait(Scheduler* scheduler) {
if (scheduler) {
scheduler->addToXWait(this);
}
}
//---------------------- Omar PART END ----------------------

std::ostream& operator<<(std::ostream& out, const Treatment* treatment) {
    if (treatment != nullptr) {
        out << treatment->type << " " << treatment->duration;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const Treatment& treatment) {
    out << treatment.type << " " << treatment.duration;
    return out;
}