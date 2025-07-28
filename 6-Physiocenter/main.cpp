#include "Scheduler.h"
#include "UI.h"  
#include <iostream>
#include <cstdlib>  
#include <ctime>
#include <vector> 
//-----------main part------------------
int main() {
    
    srand(static_cast<unsigned>(time(nullptr))); 
    
    Scheduler scheduler;
    UI ui(&scheduler);     char mode;  
    std::cout << "1 for interactive 0 for silent mode "; 
    std::cin >> mode;
    ui.setInteractiveMode(mode == '1');
    std::cin.ignore();  

    std::string inputFilename = ui.getInputFileName(); 
    std::string outputFilename = ui.getOutputFileName();
    
    if (!scheduler.IoadData(inputFilename)) {
        std::cout << "errorr.." << inputFilename << std::endl; //shof el file fein
        return 1;
    }    
    if (ui.getInteractiveMode()) {
      bool continueSim = true;
      int maxSteps = 200; 
      int stepCount = 0;
      while (continueSim && stepCount<maxSteps) {
         continueSim = scheduler.simTimeestep(); 
         ui.printTimeStep();
         if (continueSim) {
            ui.waitForUserInput(); 
         }
         int ol = stepCount; 
         stepCount++;
      }
      if (stepCount >= maxSteps) {
         std::cout << "max timestep" << std::endl; //lw 3ada max timestep
      }
      scheduler.runSimulation(outputFilename);    } else {
       scheduler.runSimulation(outputFilename); 
       ui.printSilentModeMessage(); 
    }

    return 0; 
    
    
}
