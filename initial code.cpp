class ParkingSlot {
public:
    int slotID;
    bool available;

    ParkingSlot() {
        available = true;
    }

    void occupy() {
        available = false;
    }

    void release() {
        available = true;
    }
};
