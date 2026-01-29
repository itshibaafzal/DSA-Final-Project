#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>

class Vehicle {
public:
    enum VehicleType {
        CAR,
        BIKE
    };

private:
    std::string vehicleNumber;
    VehicleType type;
    int preferredZoneId;

public:
    Vehicle(const std::string& number, VehicleType type, int preferredZone);
    std::string getVehicleNumber() const;
    VehicleType getVehicleType() const;
    int getPreferredZoneId() const;
    bool isSameVehicle(const Vehicle& other) const;
    static std::string vehicleTypeToString(VehicleType type);
};

#endif
// END OF FILE - NO IMPLEMENTATION HERE