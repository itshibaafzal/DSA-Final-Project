#include "ParkingRequest.h"

// -------- Constructor --------
ParkingRequest::ParkingRequest(int id, const Vehicle& v, int zoneId)
    : requestId(id),
      vehicle(v),
      requestedZoneId(zoneId),
      allocatedSlot(nullptr),
      state(REQUESTED) {

    requestTime = time(nullptr);
    occupyTime = 0;
    releaseTime = 0;
}

// -------- Identity --------
int ParkingRequest::getRequestId() const {
    return requestId;
}

std::string ParkingRequest::getVehicleNumber() const {
    return vehicle.getVehicleNumber();
}

Vehicle::VehicleType ParkingRequest::getVehicleType() const {
    return vehicle.getVehicleType();
}

// -------- State --------
ParkingRequest::RequestState ParkingRequest::getState() const {
    return state;
}

std::string ParkingRequest::getStateAsString() const {
    switch (state) {
        case REQUESTED: return "REQUESTED";
        case ALLOCATED: return "ALLOCATED";
        case OCCUPIED:  return "OCCUPIED";
        case RELEASED:  return "RELEASED";
        case CANCELLED: return "CANCELLED";
    }
    return "UNKNOWN";
}

// -------- Lifecycle --------
bool ParkingRequest::allocateSlot(ParkingSlot* slot) {
    if (state != REQUESTED || slot == nullptr || !slot->isAvailable())
        return false;

    allocatedSlot = slot;
    allocatedSlot->markOccupied();
    state = ALLOCATED;
    return true;
}

bool ParkingRequest::occupy() {
    if (state != ALLOCATED)
        return false;

    occupyTime = time(nullptr);
    state = OCCUPIED;
    return true;
}

bool ParkingRequest::release() {
    if (state != OCCUPIED || allocatedSlot == nullptr)
        return false;

    allocatedSlot->markFree();
    releaseTime = time(nullptr);
    state = RELEASED;
    return true;
}

bool ParkingRequest::cancel() {
    if (state == CANCELLED || state == RELEASED)
        return false;

    if (allocatedSlot != nullptr && state == ALLOCATED) {
        allocatedSlot->markFree();
    }

    state = CANCELLED;
    return true;
}

// -------- Slot & Zone --------
ParkingSlot* ParkingRequest::getAllocatedSlot() const {
    return allocatedSlot;
}

int ParkingRequest::getRequestedZoneId() const {
    return requestedZoneId;
}

// NEW: Helper methods for detailed messages
int ParkingRequest::getAllocatedSlotId() const {
    return (allocatedSlot != nullptr) ? allocatedSlot->getSlotId() : -1;
}

int ParkingRequest::getAllocatedZoneId() const {
    return (allocatedSlot != nullptr) ? allocatedSlot->getZoneId() : -1;
}

int ParkingRequest::getAllocatedAreaId() const {
    return (allocatedSlot != nullptr) ? allocatedSlot->getAreaId() : -1;
}

// -------- Analytics --------
time_t ParkingRequest::getRequestTime() const {
    return requestTime;
}

time_t ParkingRequest::getOccupyTime() const {
    return occupyTime;
}

time_t ParkingRequest::getReleaseTime() const {
    return releaseTime;
}

double ParkingRequest::getParkingDurationHours() const {
    if (state != RELEASED || occupyTime == 0 || releaseTime == 0)
        return 0.0;

    return difftime(releaseTime, occupyTime) / 3600.0;
}