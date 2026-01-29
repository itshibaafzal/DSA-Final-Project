#include "ParkingArea.h"
#include "ParkingSlot.h"

// -------- Constructor --------
ParkingArea::ParkingArea(int id, const std::string& name, int zone)
    : areaId(id), areaName(name), zoneId(zone) {}

    
// -------- Identity --------
int ParkingArea::getAreaId() const {
    return areaId;
}


std::string ParkingArea::getAreaName() const {
    return areaName;
}


int ParkingArea::getZoneId() const {
    return zoneId;
}


// -------- Slot Management --------
void ParkingArea::addSlot(ParkingSlot* slot) {
    if (slot != nullptr) {
        slots.push_back(slot);
    }
}


int ParkingArea::getTotalSlots() const {
    return slots.size();
}


int ParkingArea::getOccupiedSlots() const {
    int count = 0;
    for (auto slot : slots) {
        if (!slot->isAvailable()) {
            count++;
        }
    }
    return count;
}


int ParkingArea::getFreeSlots() const {
    return getTotalSlots() - getOccupiedSlots();
}


bool ParkingArea::isFull() const {
    return getFreeSlots() == 0;
}


// -------- Slot Access --------
const std::vector<ParkingSlot*>& ParkingArea::getSlots() const {
    return slots;
}
