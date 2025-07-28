#include "Resource.h"

Resource::Resource(char typ,int ID,int cap): 
type(typ),ID(ID),capacity(cap),currentPatients(0){
	
}
//bishof lw full
bool Resource::isFull()const{
  int check=currentPatients>=capacity; 
return check;
}
//lw l2 bd5al patient
bool Resource::addPatient(){
    if(!isFull()){
	currentPatients++; 
	return true;
      }else{
    return false;
}
}
//bsheil patient ama y5las
void Resource::removePatient(){
	if(currentPatients>0){
currentPatients--;
	}
   
  }

std::ostream& operator<<(std::ostream& out,const Resource* resource){
if(resource){
      if(resource->type=='X'){
out<<"R"<<resource->ID<<"["<<resource->currentPatients<<","<<resource->capacity<<"]";
      }else{
	  out<<resource->ID;
}
}
    return out; 
}

std::ostream& operator<<(std::ostream& out,const Resource& resource){
  if(resource.type=='X')
out<<"R"<<resource.ID<<"["<<resource.currentPatients<<","<<resource.capacity<<"]";
  else
      out<<resource.ID;
return out;
}

