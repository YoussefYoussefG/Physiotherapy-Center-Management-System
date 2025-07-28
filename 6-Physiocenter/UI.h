#pragma once
#include "Scheduler.h"
#include <string>

// ---------------------- Youssef PART START ----------------------
// UI functionality for printing simulation state
// El UI class beta3t el program kolo
class UI {
private:
    bool isInteractive;
    Scheduler* scheduler;

    // El functions elly bt print el lists w el resources
    void printAllPatients() const;
    void printWaitingLists() const;
    void printEarlyAndLateLists() const;
    void printAvailableResources() const;
    void printInTreatmentList() const;
    void printFinishedList() const;

public:
    UI(Scheduler* sched) : scheduler(sched), isInteractive(true) {}

    void setInteractiveMode(bool interactive) { isInteractive = interactive; }
    bool getInteractiveMode() const { return isInteractive; }

    std::string getInputFileName() const;
    std::string getOutputFileName() const;
    void printTimeStep() const;
    void waitForUserInput() const;
    void printSilentModeMessage() const;
};
// ---------------------- Youssef PART END ----------------------