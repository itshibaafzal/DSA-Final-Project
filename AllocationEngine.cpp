#include "AllocationEngine.h"
#include "Zone.h"
#include "ParkingArea.h"
#include "ParkingSlot.h"
#include "ParkingRequest.h"
#include "RollbackManager.h"
#include "Vehicle.h"
#include <iostream>

// -------- Constructor --------
AllocationEngine::AllocationEngine(const std::vector<Zone*>& z, RollbackManager* rb)
    : zones(z), rollbackManager(rb) {}

// -------- Base Fee --------
int AllocationEngine::calculateBaseFee(int type) const {
    return (type == 0) ? 100 : 50;  // 0 for CAR, 1 for BIKE
}

// -------- Allocate in a specific zone --------
bool AllocationEngine::allocateInZone(Zone* zone, ParkingRequest& request, int& fee) {
    for (auto area : zone->getParkingAreas()) {
        if (!area->isFull()) {
            for (auto slot : area->getSlots()) {
                if (slot->isAvailable()) {
                    if (request.allocateSlot(slot)) {
                        fee = calculateBaseFee(static_cast<int>(request.getVehicleType()));
                        if (rollbackManager) {
                            rollbackManager->recordAllocation(&request, slot);
                        }
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

// -------- NEW: Allocate in specific area of a zone --------
bool AllocationEngine::allocateInSpecificArea(Zone* zone, int areaId, ParkingRequest& request, int& fee) {
    for (auto area : zone->getParkingAreas()) {
        if (area->getAreaId() == areaId) {
            // Check if area exists
            if (!area->isFull()) {
                for (auto slot : area->getSlots()) {
                    if (slot->isAvailable()) {
                        if (request.allocateSlot(slot)) {
                            fee = calculateBaseFee(static_cast<int>(request.getVehicleType()));
                            if (rollbackManager) {
                                rollbackManager->recordAllocation(&request, slot);
                            }
                            return true;
                        }
                    }
                }
            }
            // Area found but either full or no available slots
            return false;
        }
    }
    // Area not found in this zone
    return false;
}

// -------- Original Allocation (Auto) --------
bool AllocationEngine::allocateSlot(ParkingRequest& request, int& totalFee, bool& crossZoneUsed) {
    crossZoneUsed = false;

    int baseFee = calculateBaseFee(static_cast<int>(request.getVehicleType()));
    totalFee = 0;

    // 1ï¸âƒ£ Same-zone first
    for (auto zone : zones) {
        if (zone->getZoneId() == request.getRequestedZoneId()) {
            if (allocateInZone(zone, request, totalFee)) {
                return true;
            }
            break;
        }
    }

    // 2ï¸âƒ£ Cross-zone allocation (penalty applied)
    for (auto zone : zones) {
        if (zone->getZoneId() != request.getRequestedZoneId()) {
            if (allocateInZone(zone, request, totalFee)) {
                crossZoneUsed = true;
                totalFee = baseFee + 50; // penalty
                return true;
            }
        }
    }

    return false; // No slot anywhere
}

// -------- Allocation with specific area preference --------
bool AllocationEngine::allocateSlotWithArea(ParkingRequest& request, int preferredArea, 
                                          int& totalFee, bool& crossZoneUsed) {
    crossZoneUsed = false;
    int baseFee = calculateBaseFee(static_cast<int>(request.getVehicleType()));
    totalFee = 0;

    // 1ï¸âƒ£ Try exact zone and exact area first
    for (auto zone : zones) {
        if (zone->getZoneId() == request.getRequestedZoneId()) {
            if (allocateInSpecificArea(zone, preferredArea, request, totalFee)) {
                return true;
            }
            // Exact area not available, try other areas in same zone
            break;
        }
    }

    // 2ï¸âƒ£ Try same zone, different area
    for (auto zone : zones) {
        if (zone->getZoneId() == request.getRequestedZoneId()) {
            for (auto area : zone->getParkingAreas()) {
                if (area->getAreaId() != preferredArea) {
                    for (auto slot : area->getSlots()) {
                        if (slot->isAvailable()) {
                            if (request.allocateSlot(slot)) {
                                totalFee = calculateBaseFee(static_cast<int>(request.getVehicleType()));
                                if (rollbackManager) {
                                    rollbackManager->recordAllocation(&request, slot);
                                }
                                std::cout << "âš  Preferred area full, allocated in different area in same zone\n";
                                return true;
                            }
                        }
                    }
                }
            }
            break;
        }
    }

    // 3ï¸âƒ£ Try cross-zone, same area number (with penalty)
    for (auto zone : zones) {
        if (zone->getZoneId() != request.getRequestedZoneId()) {
            if (allocateInSpecificArea(zone, preferredArea, request, totalFee)) {
                crossZoneUsed = true;
                totalFee = baseFee + 50; // penalty
                std::cout << "âš  Zone full, allocated in different zone (penalty applied)\n";
                return true;
            }
        }
    }

    // 4ï¸âƒ£ Fallback: Try any available slot anywhere (original logic)
    std::cout << "âš  Could not find slot in preferred area, trying auto-allocation...\n";
    return allocateSlot(request, totalFee, crossZoneUsed);
}