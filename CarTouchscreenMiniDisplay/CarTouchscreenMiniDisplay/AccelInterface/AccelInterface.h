// AccelInterface.h
#pragma once
#include "bsp_qmi8658.h"

class AccelInterface {
public:
    struct RollPitch {
        float roll = 0.0f;  // Roll angle in degrees
        float pitch = 0.0f; // Pitch angle in degrees
    };    

    virtual ~AccelInterface() = default;

    // Pure virtual function to initialize the accelerometer
    bool initialize();

    /**
     * @brief Get the pitch and roll angles.
     */
    RollPitch getPitchAndRoll();


    private:

    inline float CalculateRoll(float AccY,float AccZ);
};