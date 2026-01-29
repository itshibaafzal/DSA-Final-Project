#include "Vehicle.h"

Vehicle::Vehicle(const std::string& number, VehicleType t, int preferredZone)
    : vehicleNumber(number), type(t), preferredZoneId(preferredZone) {}

std::string Vehicle::getVehicleNumber() const {
    return vehicleNumber;
}

Vehicle::VehicleType Vehicle::getVehicleType() const {
    return type;
}

int Vehicle::getPreferredZoneId() const {
    return preferredZoneId;
}

bool Vehicle::isSameVehicle(const Vehicle& other) const {
    return vehicleNumber == other.vehicleNumber && type == other.type;
}

std::string Vehicle::vehicleTypeToString(VehicleType type) {
    return (type == CAR) ? "Car" : "Bike";
}