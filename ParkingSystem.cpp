#include "ParkingSystem.h"
#include <iostream>
#include <algorithm>  // For std::max

// -------- Constructor --------
ParkingSystem::ParkingSystem() : nextRequestId(1) {
    initializeCity();
    allocationEngine = new AllocationEngine(zones, &rollbackManager);
}

// -------- Destructor --------
ParkingSystem::~ParkingSystem() {
    for (auto z : zones) delete z;
    for (auto v : vehicles) delete v;
    for (auto r : requests) delete r;
    delete allocationEngine;
}

// -------- Initialize City --------
void ParkingSystem::initializeCity() {
    int slotIdCounter = 1;

    for (int z = 1; z <= 15; z++) {
        Zone* zone = new Zone(z, "Zone-" + std::to_string(z));

        for (int a = 1; a <= 3; a++) {
            ParkingArea* area = new ParkingArea(a, "Area-" + std::to_string(a), z);

            for (int s = 1; s <= 20; s++) {
                ParkingSlot* slot = new ParkingSlot(slotIdCounter++, z, a);
                area->addSlot(slot);
            }
            zone->addParkingArea(area);
        }
        zones.push_back(zone);
    }
}

// -------- Helpers --------
Zone* ParkingSystem::findZoneById(int zoneId) {
    for (auto z : zones)
        if (z->getZoneId() == zoneId)
            return z;
    return nullptr;
}

bool ParkingSystem::vehicleExists(const std::string& number, Vehicle::VehicleType type) {
    for (auto v : vehicles)
        if (v->getVehicleNumber() == number && v->getVehicleType() == type)
            return true;
    return false;
}

ParkingRequest* ParkingSystem::findRequestByVehicle(const std::string& number, Vehicle::VehicleType type) {
    for (auto r : requests)
        if (r->getVehicleNumber() == number && r->getVehicleType() == type)
            return r;
    return nullptr;
}

// -------- Create Request (Auto Allocation) --------
bool ParkingSystem::createParkingRequest(const std::string& vehicleNumber,
                                         Vehicle::VehicleType type,
                                         int preferredZone,
                                         int& fee,
                                         bool& crossZoneUsed) {

    if (vehicleExists(vehicleNumber, type)) {
        std::cout << "❌ Vehicle already exists in system\n";
        return false;
    }

    Vehicle* vehicle = new Vehicle(vehicleNumber, type, preferredZone);
    vehicles.push_back(vehicle);

    ParkingRequest* request = new ParkingRequest(nextRequestId++, *vehicle, preferredZone);

    if (!allocationEngine->allocateSlot(*request, fee, crossZoneUsed)) {
        std::cout << "❌ No slots available in any zone\n";
        return false;
    }

    requests.push_back(request);

    // Detailed success message
    std::cout << "✅ Vehicle " << vehicleNumber 
              << " successfully allocated slot " << request->getAllocatedSlotId()
              << " in zone " << request->getAllocatedZoneId()
              << " and area " << request->getAllocatedAreaId()
              << " | Fee: Rs " << fee << "\n";
    
    if (crossZoneUsed) {
        std::cout << "⚠ Cross-zone allocation penalty applied\n";
    }
    
    return true;
}

// -------- Create Request With Specific Area --------
bool ParkingSystem::createParkingRequestWithArea(const std::string& vehicleNumber,
                                                 Vehicle::VehicleType type,
                                                 int preferredZone,
                                                 int preferredArea,
                                                 int& fee,
                                                 bool& crossZoneUsed) {

    if (vehicleExists(vehicleNumber, type)) {
        std::cout << "❌ Vehicle already exists in system\n";
        return false;
    }

    // Validate area
    if (preferredArea < 1 || preferredArea > 3) {
        std::cout << "❌ Invalid parking area. Must be between 1-3\n";
        return false;
    }

    Vehicle* vehicle = new Vehicle(vehicleNumber, type, preferredZone);
    vehicles.push_back(vehicle);

    ParkingRequest* request = new ParkingRequest(nextRequestId++, *vehicle, preferredZone);

    if (!allocationEngine->allocateSlotWithArea(*request, preferredArea, fee, crossZoneUsed)) {
        std::cout << "❌ No slots available in the selected area/zone\n";
        return false;
    }

    requests.push_back(request);

    // Detailed success message
    std::cout << "✅ Vehicle " << vehicleNumber 
              << " successfully allocated slot " << request->getAllocatedSlotId()
              << " in zone " << request->getAllocatedZoneId()
              << " and area " << request->getAllocatedAreaId()
              << " | Fee: Rs " << fee << "\n";
    
    if (crossZoneUsed) {
        std::cout << "⚠ Cross-zone allocation penalty applied\n";
    }
    
    return true;
}

// -------- Occupy --------
bool ParkingSystem::occupyParking(const std::string& vehicleNumber, Vehicle::VehicleType type) {
    ParkingRequest* req = findRequestByVehicle(vehicleNumber, type);
    if (!req || !req->occupy()) {
        std::cout << "❌ Vehicle " << vehicleNumber << " not allocated or cannot occupy\n";
        return false;
    }
    
    // Detailed success message
    std::cout << "✅ Vehicle " << vehicleNumber 
              << " successfully occupied slot " << req->getAllocatedSlotId()
              << " in zone " << req->getAllocatedZoneId()
              << " and area " << req->getAllocatedAreaId() << "\n";
    
    return true;
}

// -------- Release --------
bool ParkingSystem::releaseParking(const std::string& vehicleNumber, Vehicle::VehicleType type) {
    ParkingRequest* req = findRequestByVehicle(vehicleNumber, type);
    if (!req || !req->release()) {
        std::cout << "❌ Release failed for vehicle " << vehicleNumber 
                  << " - not in system or not occupied\n";
        return false;
    }
    
    // Detailed success message
    std::cout << "✅ Vehicle " << vehicleNumber 
              << " successfully released slot " << req->getAllocatedSlotId()
              << " from zone " << req->getAllocatedZoneId()
              << " and area " << req->getAllocatedAreaId() << "\n";
    
    return true;
}

// -------- Cancel --------
bool ParkingSystem::cancelRequest(const std::string& vehicleNumber, Vehicle::VehicleType type) {
    ParkingRequest* req = findRequestByVehicle(vehicleNumber, type);
    if (!req || !req->cancel()) {
        std::cout << "❌ Cancellation failed for vehicle " << vehicleNumber 
                  << " - not in system or cannot be cancelled\n";
        return false;
    }

    rollbackManager.recordCancellation(req);
    
    // Detailed success message
    std::cout << "✅ Vehicle " << vehicleNumber 
              << " successfully cancelled request for slot " << req->getAllocatedSlotId()
              << " in zone " << req->getAllocatedZoneId() << "\n";
    
    return true;
}

// -------- Rollback --------
bool ParkingSystem::rollbackLast(int k) {
    if (rollbackManager.rollbackK(k)) {
        std::cout << "✅ Successfully rolled back " << k << " operation(s)\n";
        return true;
    } else {
        std::cout << "❌ Rollback failed - not enough operations to rollback\n";
        return false;
    }
}

// -------- Display Zone Status --------
void ParkingSystem::displayZoneStatus() const {
    std::cout << "\n========== ZONE STATUS ==========\n";
    for (auto z : zones) {
        std::cout << z->getZoneName()
                  << " | Total: " << z->getTotalSlots()
                  << " | Free: " << z->getFreeSlots()
                  << " | Utilization: " << z->getUtilizationRate() * 100 << "%\n";
    }
    std::cout << "================================\n";
}

// -------- Display Last Operations (NEW METHOD) --------
void ParkingSystem::displayLastOperations(int count) const {
    std::cout << "\n========== LAST " << count << " OPERATIONS ==========\n";
    
    if (requests.empty()) {
        std::cout << "No operations recorded yet.\n";
        return;
    }
    
    // Show last 'count' requests (most recent first)
    int start = std::max(0, (int)requests.size() - count);
    int shown = 0;
    
    for (int i = requests.size() - 1; i >= start && shown < count; i--, shown++) {
        ParkingRequest* req = requests[i];
        
        std::cout << "Operation #" << (shown + 1) << ":\n";
        std::cout << "  Vehicle: " << req->getVehicleNumber() << "\n";
        std::cout << "  Type: " << ((req->getVehicleType() == Vehicle::CAR) ? "Car" : "Bike") << "\n";
        std::cout << "  Status: " << req->getStateAsString() << "\n";
        
        if (req->getAllocatedSlotId() != -1) {
            std::cout << "  Slot: " << req->getAllocatedSlotId() 
                      << " (Zone " << req->getAllocatedZoneId() 
                      << ", Area " << req->getAllocatedAreaId() << ")\n";
        }
        
        // Show times
        time_t reqTime = req->getRequestTime();
        std::cout << "  Requested: " << ctime(&reqTime);
        
        // For RELEASED status only, show duration
        if (req->getState() == ParkingRequest::RELEASED) {
            if (req->getOccupyTime() > 0 && req->getReleaseTime() > 0) {
                double duration = req->getParkingDurationHours();
                std::cout << "  Duration: " << duration << " hours (";
                
                // Convert to hours, minutes, seconds
                int hours = static_cast<int>(duration);
                int minutes = static_cast<int>((duration - hours) * 60);
                int seconds = static_cast<int>((((duration - hours) * 60) - minutes) * 60);
                
                if (hours > 0) std::cout << hours << "h ";
                if (minutes > 0) std::cout << minutes << "m ";
                std::cout << seconds << "s)\n";
            }
        }
        
        // Show occupy time if occupied or released
        if (req->getOccupyTime() > 0) {
            time_t occTime = req->getOccupyTime();
            std::cout << "  Occupied: " << ctime(&occTime);
        }
        
        // Show release time if released
        if (req->getReleaseTime() > 0) {
            time_t relTime = req->getReleaseTime();
            std::cout << "  Released: " << ctime(&relTime);
        }
        
        std::cout << "-----------------------------------\n";
    }
}