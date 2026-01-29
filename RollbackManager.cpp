#include "RollbackManager.h"

// -------- Record Allocation --------
void RollbackManager::recordAllocation(ParkingRequest* request, ParkingSlot* slot) {
    RollbackEntry entry;
    entry.request = request;
    entry.slot = slot;
    entry.previousState = ParkingRequest::REQUESTED;

    rollbackStack.push(entry);
}


// -------- Record Cancellation --------
void RollbackManager::recordCancellation(ParkingRequest* request) {
    RollbackEntry entry;
    entry.request = request;
    entry.slot = request->getAllocatedSlot();
    entry.previousState = ParkingRequest::ALLOCATED;

    rollbackStack.push(entry);
}


// -------- Rollback Last Operation --------
bool RollbackManager::rollbackLast() {
    if (rollbackStack.empty())
        return false;

    RollbackEntry entry = rollbackStack.top();
    rollbackStack.pop();

    // Restore slot
    if (entry.slot != nullptr) {
        entry.slot->markFree();
    }

    // Restore request state
    if (entry.request != nullptr) {
        if (entry.previousState == ParkingRequest::REQUESTED) {
            // undo allocation
            entry.request->cancel(); // safe reset
        } else if (entry.previousState == ParkingRequest::ALLOCATED) {
            // undo cancellation
            entry.request->allocateSlot(entry.slot);
        }
    }

    return true;
}


// -------- Rollback K Operations --------
bool RollbackManager::rollbackK(int k) {
    if (k <= 0 || rollbackStack.size() < static_cast<size_t>(k))
        return false;

    for (int i = 0; i < k; i++) {
        rollbackLast();
    }
    return true;
}


// -------- Utility --------
bool RollbackManager::isEmpty() const {
    return rollbackStack.empty();
}