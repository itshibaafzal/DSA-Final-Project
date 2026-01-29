#include <iostream>
#include "ParkingSystem.h"

using namespace std;

void showMenu() {
    cout << "\n========== SMART PARKING SYSTEM ==========\n";
    cout << "1. Create Parking Request (Auto Allocation)\n";
    cout << "2. Create Parking Request (Select Zone & Area)\n";
    cout << "3. Occupy Parking\n";
    cout << "4. Release Parking\n";
    cout << "5. Cancel Parking Request\n";
    cout << "6. View Zone Status\n";
    cout << "7. View Last 5 Operations\n";  
    cout << "8. Rollback Last Operation(s)\n";  
    cout << "0. Exit\n";
    cout << "=========================================\n";
    cout << "Enter choice: ";
}

bool inputVehicleType(Vehicle::VehicleType& type) {
    int choice;
    cout << "Select Vehicle Type:\n";
    cout << "1. Car\n";
    cout << "2. Bike\n";
    cout << "Choice: ";
    cin >> choice;
    
    if (choice == 1) {
        type = Vehicle::CAR;
        return true;
    } else if (choice == 2) {
        type = Vehicle::BIKE;
        return true;
    } else {
        cout << "❌ Invalid vehicle type selected!\n";
        return false;
    }
}

int inputParkingArea() {
    int area;
    do {
        cout << "Select Parking Area (1-3): ";
        cin >> area;
        if (area < 1 || area > 3) {
            cout << "❌ Invalid area! Please enter 1, 2, or 3.\n";
        }
    } while (area < 1 || area > 3);
    return area;
}

int main() {
    ParkingSystem system;
    int choice;

    do {
        showMenu();
        cin >> choice;

        switch(choice) {
            case 1: {
                // Auto allocation
                string vehicleNumber;
                int zoneId;
                int fee;
                bool crossZone;
                Vehicle::VehicleType type;

                cout << "Enter vehicle number: ";
                cin >> vehicleNumber;

                if (!inputVehicleType(type)) {
                    continue;
                }

                cout << "Enter preferred zone (1 - 15): ";
                cin >> zoneId;

                if (zoneId < 1 || zoneId > 15) {
                    cout << "❌ Invalid zone selected. Must be between 1-15.\n";
                    continue;
                }

                if (system.createParkingRequest(vehicleNumber, type, zoneId, fee, crossZone)) {
                    cout << "✅ Request created successfully\n";
                    cout << "Allocated Fee: Rs " << fee << "\n";
                    if (crossZone)
                        cout << "⚠ Cross-zone allocation penalty applied\n";
                }
                break;
            }
            
            case 2: {  // Select zone and area
                string vehicleNumber;
                int zoneId, areaId;
                int fee;
                bool crossZone;
                Vehicle::VehicleType type;

                cout << "Enter vehicle number: ";
                cin >> vehicleNumber;

                if (!inputVehicleType(type)) {
                    continue;
                }

                cout << "Enter preferred zone (1 - 15): ";
                cin >> zoneId;

                if (zoneId < 1 || zoneId > 15) {
                    cout << "❌ Invalid zone selected. Must be between 1-15.\n";
                    continue;
                }

                areaId = inputParkingArea();

                if (system.createParkingRequestWithArea(vehicleNumber, type, zoneId, areaId, fee, crossZone)) {
                    cout << "✅ Request created successfully\n";
                    cout << "Allocated Fee: Rs " << fee << "\n";
                    if (crossZone)
                        cout << "⚠ Cross-zone allocation penalty applied\n";
                }
                break;
            }
            
            case 3: {  // Occupy Parking
                string vehicleNumber;
                Vehicle::VehicleType type;
                
                cout << "Enter vehicle number: ";
                cin >> vehicleNumber;
                
                if (inputVehicleType(type)) {
                    system.occupyParking(vehicleNumber, type);
                }
                break;
            }
            
            case 4: {  // Release Parking
                string vehicleNumber;
                Vehicle::VehicleType type;
                
                cout << "Enter vehicle number: ";
                cin >> vehicleNumber;
                
                if (inputVehicleType(type)) {
                    system.releaseParking(vehicleNumber, type);
                }
                break;
            }
            
            case 5: {  // Cancel Request
                string vehicleNumber;
                Vehicle::VehicleType type;
                
                cout << "Enter vehicle number: ";
                cin >> vehicleNumber;
                
                if (inputVehicleType(type)) {
                    system.cancelRequest(vehicleNumber, type);
                }
                break;
            }
            
            case 6:  // View Zone Status
                system.displayZoneStatus();
                break;
                
            case 7: {  
                system.displayLastOperations(5);
                break;
            }
                
            case 8: {  
                int k;
                cout << "Enter number of operations to rollback: ";
                cin >> k;
                
                if (system.rollbackLast(k))
                    cout << "✅ Rollback successful\n";
                else
                    cout << "❌ Rollback failed\n";
                break;
            }
                
            case 0:
                cout << "Exiting system. Goodbye!\n";
                break;
                
            default:
                cout << "❌ Invalid option. Please try again.\n";
        }
        
    } while (choice != 0);

    return 0;
}