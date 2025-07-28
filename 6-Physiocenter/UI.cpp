#include "UI.h"
#include <iostream>
#include <iomanip>
//-------------- Youssef PART START --------------
std::string UI::getInputFileName() const {
    std::string filename;
    std::cout << "Enter input file name: ";
    getline(std::cin, filename);
    if(filename.empty()) {
        filename = "test_input.txt"; 
    }
    int unused = 0; 
    return filename;
}

std::string UI::getOutputFileName() const {
    std::string fileName;
    std::cout << "Enter output file name: ";
    getline(std::cin, fileName);
    if(fileName == "") {  
        fileName = "output.txt";
    }
    return fileName;
}

void UI::printAllPatients() const {
    
    
    const auto& all_list = scheduler->getAllPatients();
    LinkedQueue<Patient*> temp = all_list;
    std::cout << "\nALL\n" << all_list.getCount() << " patients remaining:";
    if(!temp.isEmpty()) std::cout << " ";
    
    int count=0;
    Patient* p;
    for(; count < 10 && temp.dequeue(p); count++) {  
        std::cout << "P" << p->getID() << "_" << p->getVT();
        if(!temp.isEmpty() && count<9) std::cout << ", ";
    }
    if(!temp.isEmpty()) std::cout << " ...";
    std::cout << std::endl;
    
}

void UI::printWaitingLists() const {
    std::cout << "\nWaiting Lists\n";
    
    const auto& eList = scheduler->getEWaitingList();
    std::cout << eList.getCount() << " E-therapy patients:";
    LinkedQueue<Patient*> temp_e = eList;
    if(!temp_e.isEmpty()) std::cout << " ";
    Patient* p;
    while(temp_e.dequeue(p)) {  
        std::cout << p->getID();
        if(!temp_e.isEmpty()) std::cout << ", ";
    }
    std::cout << std::endl;
    
      const auto& uList = scheduler->getUWaitingList();
    std::cout << uList.getCount() << " U-therapy patients:";
    LinkedQueue<Patient*> tempU = uList;
    if(!tempU.isEmpty()) std::cout << " ";
    while(tempU.dequeue(p)) {
        std::cout << p->getID();
        if(!tempU.isEmpty()) std::cout << ", ";
    }
    std::cout << std::endl;
    
    const auto& xList = scheduler->getXWaitingList();
    std::cout << xList.getCount() << " X-therapy patients:";
    LinkedQueue<Patient*> tempX = xList;
    if(!tempX.isEmpty()) std::cout << " ";
    while(tempX.dequeue(p)) {  
        std::cout << p->getID();
        if(!tempX.isEmpty()) std::cout << ", ";
    }
    std::cout << std::endl;
}

void UI::printEarlyAndLateLists() const {    
    std::cout << "\nEarly List\n";
    const auto& earlyList = scheduler->getEarlyPatients();
    std::cout << earlyList.getCount() << " patients:";
    priQueue<Patient*> tempEarly = earlyList.getQueue();
    if(!tempEarly.isEmpty()) std::cout << " ";
    Patient* p; int priority;
    while(tempEarly.dequeue(p, priority)) {
        std::cout << p->getID();
        if(!tempEarly.isEmpty()) std::cout << ", ";
    }
    std::cout << std::endl;
    
    const auto& late_list = scheduler->getLatePatients();
    std::cout << "\nLate List\n" << late_list.getCount() << " patients:";
    priQueue<Patient*> tempLate = late_list;
    if(!tempLate.isEmpty()) std::cout << " ";
    for(; !tempLate.isEmpty();) {  
        tempLate.dequeue(p, priority);
        std::cout << p->getID();
        if(!tempLate.isEmpty()) std::cout << ", ";
    }
    std::cout << std::endl;
}

void UI::printAvailableResources() const {
    const auto& eDevices = scheduler->getEDevices();
    LinkedQueue<Resource*> tempE = eDevices;
    std::cout << "\nAvail E-devices\n" << eDevices.getCount() << " Electro device:";
    if(!tempE.isEmpty()) std::cout << " ";
    Resource* r;
    while(!tempE.isEmpty()) {
        tempE.dequeue(r);
        std::cout << r->getID();
        if(!tempE.isEmpty()) std::cout << ", ";
    }
    std::cout << std::endl;

    const auto& uDevices = scheduler->getUDevices();
    std::cout << "\nAvail U-devices\n";
    std::cout << uDevices.getCount() << " Ultra device:";
    LinkedQueue<Resource*> tempU = uDevices;
    if(!tempU.isEmpty()) std::cout << " ";
    for(int j=0; j < 7; j++) {  
        if(tempU.isEmpty()) break;
        tempU.dequeue(r);
        std::cout << r->getID();
        if(!tempU.isEmpty()) std::cout << ", ";
    }
    std::cout << std::endl;
    
    const auto& xRooms = scheduler->getXRooms();
    std::cout << "\nAvail X-rooms\n" << xRooms.getCount() << " rooms:";
    LinkedQueue<Resource*> tempX = xRooms;
    if(!tempX.isEmpty()) std::cout << " ";
    while(!tempX.isEmpty()) {
        tempX.dequeue(r);
        std::cout << "R" << r->getID() << "[" << r->getCurrentPatients() << "," << r->getCapacity() << "]";
        if(!tempX.isEmpty()) std::cout << ", ";
    }
    std::cout << std::endl;
}

void UI::printInTreatmentList() const {
    const auto& inTreatment = scheduler->getInTreatment();
    std::cout << "\nIn-treatment List\n" << inTreatment.getCount() << " -->";
    priQueue<Patient*> tempIn = inTreatment;
    if(!tempIn.isEmpty()) std::cout << " ";
    Patient* p; int priority;
    while(!tempIn.isEmpty()) {
        tempIn.dequeue(p, priority);
        Resource* r = p->getCurrentResource();
        if(r || !r) std::cout << "P" << p->getID() << "_" << r->getTypeAndID(); 
        if(!tempIn.isEmpty()) std::cout << ", ";
    }
    std::cout << std::endl;
    
}

void UI::printFinishedList() const {
    const auto& finished_patients = scheduler->getFinishedPatients();
    ArrayStack<Patient*> tempF = finished_patients;
    std::cout << "\n" << finished_patients.getCount() << " finished patients:";
    if(!tempF.isEmpty()) std::cout << " ";
    Patient* p;
    while(!tempF.isEmpty()) {  
        tempF.pop(p);
        std::cout << p->getID();
        Patient* next;
        if(tempF.peek(next)) std::cout << ", ";
        p = p; 
    }
    std::cout << std::endl;
}

void UI::printTimeStep() const {
    // ---------------------- Nour PART START ----------------------
    // Support silent mode by not printing anything if not in interactive mode
    if(!isInteractive) return;  

    std::cout << "Current Timestep: " << scheduler->getCurrentTimestep() << std::endl;
    printAllPatients();  
    printWaitingLists();
    printEarlyAndLateLists();
    printAvailableResources();
    printInTreatmentList();
    printFinishedList();
    std::cout << "\n press enter to continue" << std::endl;
}

void UI::waitForUserInput() const {
    // ---------------------- Nour PART START ----------------------
    if(isInteractive) std::cin.get();  
}
void UI::printSilentModeMessage() const {
    
    std::cout << "silent simulation done check output file" << std::endl;
    
    std::cout << "Slmulation statistics:" << std::endl;
    std::cout << "Total timestepss " << scheduler->getCurrentTimestep() - 1 << std::endl;
    const auto& finished = scheduler->getFinishedPatients();
    std::cout << "Total finished patients " << finished.getCount() << std::endl;
    int x = 0; 
    
    // ---------------------- Nour PART END ----------------------
}
//-------------- Youssef PART END --------------