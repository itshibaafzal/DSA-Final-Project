#ifndef PARKING_SLOT_H
#define PARKING_SLOT_H

class ParkingSlot {
private:
    int slotId;
    int zoneId;
    int areaId;
    bool available;

public:
    ParkingSlot(int slotId, int zoneId, int areaId);
    
    int getSlotId() const;
    int getZoneId() const;
    int getAreaId() const;
    
    bool isAvailable() const;
    void markOccupied();
    void markFree();
};

#endif