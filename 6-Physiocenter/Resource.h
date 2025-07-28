#pragma once
#include <iostream>
#include <string>

 //---------------------- OmarPART START ----------------------


class Resource {
private:
    
    char type;     
    int ID;         
    int capacity;   
    int currentPatients; 

    friend std::ostream& operator<<(std::ostream& out, const Resource* resource);
    friend std::ostream& operator<<(std::ostream& out, const Resource& resource);

public:
    
    Resource(char typ, int resourceID, int cap = 1);
    char getType() const { return type; }
    int getID() const { return ID; }
    bool isFull() const;
    bool addPatient();     
    void removePatient();  
    int getCapacity() const { return capacity; }
    int getCurrentPatients() const { return currentPatients; }
    std::string getTypeAndID() const {
        std::string result;
        result.push_back(type);
        result.append(std::to_string(ID));
        return result;
    }
};
 //---------------------- OmarPART END ----------------------