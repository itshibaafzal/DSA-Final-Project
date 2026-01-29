#include "Zone.h"
#include "ParkingArea.h"   // Full definition used here

// -------- Constructor --------
Zone::Zone(int id, const std::string& name)
    : zoneId(id), zoneName(name) {}

// -------- Identity --------
int Zone::getZoneId() const {
    return zoneId;
}

std::string Zone::getZoneName() const {
    return zoneName;
}

// -------- Parking Area Management --------
void Zone::addParkingArea(ParkingArea* area) {
    if (area != nullptr) {
        parkingAreas.push_back(area);
    }
}

int Zone::getTotalParkingAreas() const {
    return parkingAreas.size();
}

// -------- Slot Statistics --------
int Zone::getTotalSlots() const {
    int total = 0;
    for (auto area : parkingAreas) {
        total += area->getTotalSlots();
    }
    return total;
}

int Zone::getOccupiedSlots() const {
    int occupied = 0;
    for (auto area : parkingAreas) {
        occupied += area->getOccupiedSlots();
    }
    return occupied;
}

int Zone::getFreeSlots() const {
    return getTotalSlots() - getOccupiedSlots();
}

bool Zone::isZoneFull() const {
    return getFreeSlots() == 0;
}

// -------- Zone Adjacency & Preference --------
void Zone::addNeighborZone(Zone* zone) {
    if (zone != nullptr && zone != this) {
        neighborZones.push_back(zone);
    }
}

bool Zone::isNeighborZone(int id) const {
    for (auto neighbor : neighborZones) {
        if (neighbor->getZoneId() == id) {
            return true;
        }
    }
    return false;
}

bool Zone::isCrossZoneAllowed() const {
    // Rule: cross-zone allowed only if this zone is full
    return isZoneFull();
}

// -------- Utilization --------
double Zone::getUtilizationRate() const {
    int total = getTotalSlots();
    if (total == 0) return 0.0;

    return static_cast<double>(getOccupiedSlots()) / total;
}

// -------- Accessors --------
const std::vector<ParkingArea*>& Zone::getParkingAreas() const {
    return parkingAreas;
}