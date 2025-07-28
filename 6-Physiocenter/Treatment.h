#pragma once
#include "Resource.h"
#include <iostream>

 //---------------------- Omar PART START ----------------------


class Scheduler;
class Patient;

class Treatment {
protected:
    int duration;
    int assignmentTime;
    Resource* resource;
    char type;
    
    friend std::ostream& operator<<(std::ostream& out, const Treatment* treatment);
    friend std::ostream& operator<<(std::ostream& out, const Treatment& treatment);

public:
    Treatment(char typ, int dur);
    virtual ~Treatment();
    
    int getDuration() const { return duration; }
    int getAssignmentTime() const { return assignmentTime; }
    char getType() const { return type; }
    Resource* getResource() const { return resource; }
    
    void setAssignmentTime(int time) { assignmentTime = time; }
    void setResource(Resource* res) { resource = res; }
    
    virtual bool canAssign(Resource* res) const = 0;
    virtual void moveToWait(Scheduler* scheduler) = 0;
};


class ETreatment : public Treatment {
public:
    ETreatment(int dur) : Treatment('E', dur) {}
    bool canAssign(Resource* res) const override;
    void moveToWait(Scheduler* scheduler) override;
};


class UTreatment : public Treatment {
public:
    UTreatment(int dur) : Treatment('U', dur) {}
    bool canAssign(Resource* res) const override;
    void moveToWait(Scheduler* scheduler) override;
};

class XTreatment : public Treatment {
public:
    XTreatment(int dur) : Treatment('X', dur) {}
    bool canAssign(Resource* res) const override;
    void moveToWait(Scheduler* scheduler) override;
};
 //---------------------- Omar PART END ----------------------
