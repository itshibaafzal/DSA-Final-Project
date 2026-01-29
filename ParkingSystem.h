#ifndef PARKING_SYSTEM_H
#define PARKING_SYSTEM_H

#include <vector>
#include <string>
#include "Zone.h"
#include "ParkingArea.h"
#include "ParkingSlot.h"
#include "Vehicle.h"
#include "ParkingRequest.h"
#include "AllocationEngine.h"
#include "RollbackManager.h"

class ParkingSystem {
private:
    std::vector<Zone*> zones;
    std::vector<Vehicle*> vehicles;
    std::vector<ParkingRequest*> requests;

    AllocationEngine* allocationEngine;
    RollbackManager rollbackManager;

    int nextRequestId;

    // Internal helpers
    Zone* findZoneById(int zoneId);
    bool vehicleExists(const std::string& number, Vehicle::VehicleType type);
    ParkingRequest* findRequestByVehicle(const std::string& number, Vehicle::VehicleType type);

public:
    ParkingSystem();
    ~ParkingSystem();

    // -------- Initialization --------
    void initializeCity();

    // -------- Core Operations --------
    bool createParkingRequest(const std::string& vehicleNumber,
                              Vehicle::VehicleType type,
                              int preferredZone,
                              int& fee,
                              bool& crossZoneUsed);

    // NEW METHOD: For selecting specific zone and area                      
    bool createParkingRequestWithArea(const std::string& vehicleNumber,
                                      Vehicle::VehicleType type,
                                      int preferredZone,
                                      int preferredArea,
                                      int& fee,
                                      bool& crossZoneUsed);

    bool occupyParking(const std::string& vehicleNumber, Vehicle::VehicleType type);
    bool releaseParking(const std::string& vehicleNumber, Vehicle::VehicleType type);
    bool cancelRequest(const std::string& vehicleNumber, Vehicle::VehicleType type);

    // -------- Rollback --------
    bool rollbackLast(int k);

    // -------- Analytics / Display --------
    void displayZoneStatus() const;
    
    // NEW: Display last operations history
    void displayLastOperations(int count = 5) const;
};

#endif