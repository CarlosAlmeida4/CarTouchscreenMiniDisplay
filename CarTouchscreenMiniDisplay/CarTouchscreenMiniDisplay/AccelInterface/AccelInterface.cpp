#include "AccelInterface.h"
//#include "pico/stdlib.h"
#include "bsp_qmi8658.h"

bool AccelInterface::initialize() {
    // Default implementation can be empty or provide common initialization logic
    bsp_qmi8658_init();
    return true;
}

AccelInterface::RollPitch AccelInterface::getPitchAndRoll() {
        RollPitch RP;
        qmi8658_data_t data;
        bsp_qmi8658_read_data(&data);
        RP.pitch = data.AngleX; // Roll
        RP.roll = CalculateRoll(data.acc_y,data.acc_z);  // Pitch
        return RP;
}

inline float AccelInterface::CalculateRoll(float AccY,float AccZ){
    return atan2(AccZ,-AccY) * 57.3;
}