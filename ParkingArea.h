#ifndef ZONE_H
#define ZONE_H

#include <string>
#include <vector>

// Forward declaration (definition comes in ParkingArea.h)
class ParkingArea;

class Zone {
private:
    int zoneId;
    std::string zoneName;

    // Parking areas inside this zone
    std::vector<ParkingArea*> parkingAreas;

    // Logical neighboring zones (custom adjacency, not graph)
    std::vector<Zone*> neighborZones;

public:
    // Constructor
    Zone(int id, const std::string& name);

    // -------- Zone Identity --------
    int getZoneId() const;
    std::string getZoneName() const;

    // -------- Parking Area Management --------
    void addParkingArea(ParkingArea* area);
    int getTotalParkingAreas() const;

    // -------- Slot Statistics (Zone Level) --------
    int getTotalSlots() const;
    int getOccupiedSlots() const;
    int getFreeSlots() const;
    bool isZoneFull() const;

    // -------- Zone Preference / Cross-Zone Rules --------
    void addNeighborZone(Zone* zone);
    bool isNeighborZone(int zoneId) const;
    bool isCrossZoneAllowed() const;

    // -------- Utilization & Analytics --------
    double getUtilizationRate() const;

    // -------- Accessors --------
    const std::vector<ParkingArea*>& getParkingAreas() const;
};

#endif
