#ifndef IMU_h
#define IMU_h

#include <Arduino.h>
#include <ICM_20948.h>
#include <Wire.h>

#define AD0_VAL 1

class IMU
{
    public:
        IMU(uint8_t SDA, uint8_t SCL);
        IMU(IMU* imu);
        uint8_t begin();
        double getPitch();
        double getRoll();
        double getYaw();
        int8_t read();

        double setZero();

    private:
        uint8_t _sda;
        uint8_t _scl;
        TwoWire wire = TwoWire(0);
        ICM_20948_I2C imuObj;
        double pitch;
        double roll;
        double yaw;

        double yawOffset = 0;
};

#endif