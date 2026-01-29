#include "ParkingSlot.h"

ParkingSlot::ParkingSlot(int id, int zId, int aId)
    : slotId(id), zoneId(zId), areaId(aId), available(true) {}

int ParkingSlot::getSlotId() const {
    return slotId;
}

int ParkingSlot::getZoneId() const {
    return zoneId;
}

int ParkingSlot::getAreaId() const {
    return areaId;
}

bool ParkingSlot::isAvailable() const {
    return available;
}

void ParkingSlot::markOccupied() {
    available = false;
}

void ParkingSlot::markFree() {
    available = true;
}