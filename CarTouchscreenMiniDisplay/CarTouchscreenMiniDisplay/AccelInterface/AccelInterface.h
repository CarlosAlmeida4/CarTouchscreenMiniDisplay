// AccelInterface.h
#pragma once
#include "bsp_qmi8658.h"


class AccelInterface {
    private:
        static AccelInterface* instancePtr;
        inline float CalculateRoll(float AccY,float AccZ);
        AccelInterface() = default;

    public:
        struct RollPitch {
            float roll = 0.0f;  // Roll angle in degrees
            float pitch = 0.0f; // Pitch angle in degrees
        };   
        AccelInterface(const AccelInterface& AccInter) = delete;

       

        static AccelInterface* getInstance() {
            if(instancePtr == nullptr){
               instancePtr = new AccelInterface();
            }
            return instancePtr;
        };
        
        RollPitch getPitchAndRoll();
        bool initialize(); 
};