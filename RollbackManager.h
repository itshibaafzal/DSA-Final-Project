#ifndef ROLLBACK_MANAGER_H
#define ROLLBACK_MANAGER_H

#include <stack>
#include "ParkingRequest.h"
#include "ParkingSlot.h"

class RollbackManager {
private:
    struct RollbackEntry {
        ParkingRequest* request;
        ParkingSlot* slot;
        ParkingRequest::RequestState previousState;
    };

    std::stack<RollbackEntry> rollbackStack;

public:
    // -------- Recording Operations --------
    void recordAllocation(ParkingRequest* request, ParkingSlot* slot);
    void recordCancellation(ParkingRequest* request);

    // -------- Rollback --------
    bool rollbackLast();
    bool rollbackK(int k);

    // -------- Utility --------
    bool isEmpty() const;
};

#endif