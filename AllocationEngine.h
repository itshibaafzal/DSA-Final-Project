#ifndef ALLOCATION_ENGINE_H
#define ALLOCATION_ENGINE_H

#include <vector>

class Zone;
class ParkingRequest;
class RollbackManager;

class AllocationEngine {
private:
    std::vector<Zone*> zones;
    RollbackManager* rollbackManager;

    
    bool allocateInZone(Zone* zone, ParkingRequest& request, int& fee);
   
    bool allocateInSpecificArea(Zone* zone, int areaId, ParkingRequest& request, int& fee);
    
    int calculateBaseFee(int type) const;  

public:
    
    AllocationEngine(const std::vector<Zone*>& zones, RollbackManager* rb);
    // --------  Allocation with specific area preference --------
    bool allocateSlotWithArea(ParkingRequest& request, int preferredArea, int& totalFee, bool& crossZoneUsed);

    // -------- Allocation --------
    bool allocateSlot(ParkingRequest& request, int& totalFee, bool& crossZoneUsed);
    
};

#endif