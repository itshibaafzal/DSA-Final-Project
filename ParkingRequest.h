#ifndef PARKING_REQUEST_H
#define PARKING_REQUEST_H

#include <string>
#include <ctime>
#include "Vehicle.h"
#include "ParkingSlot.h"

class ParkingRequest {
public:
    enum RequestState {
        REQUESTED,
        ALLOCATED,
        OCCUPIED,
        RELEASED,
        CANCELLED
    };

private:
    int requestId;
    Vehicle vehicle;
    int requestedZoneId;

    ParkingSlot* allocatedSlot;
    RequestState state;

    time_t requestTime;
    time_t occupyTime;
    time_t releaseTime;

public:
    // Constructor
    ParkingRequest(int id, const Vehicle& vehicle, int zoneId);

    // -------- Identity --------
    int getRequestId() const;
    std::string getVehicleNumber() const;
    Vehicle::VehicleType getVehicleType() const;

    // -------- State --------
    RequestState getState() const;
    std::string getStateAsString() const;

    // -------- Lifecycle Actions --------
    bool allocateSlot(ParkingSlot* slot);
    bool occupy();
    bool release();
    bool cancel();

    // -------- Slot & Zone --------
    ParkingSlot* getAllocatedSlot() const;
    int getRequestedZoneId() const;
    
    // NEW: Helper methods for detailed messages
    int getAllocatedSlotId() const;
    int getAllocatedZoneId() const;
    int getAllocatedAreaId() const;

    // -------- Time & Analytics --------
    time_t getRequestTime() const;
    time_t getOccupyTime() const;
    time_t getReleaseTime() const;
    double getParkingDurationHours() const;
};

#endif