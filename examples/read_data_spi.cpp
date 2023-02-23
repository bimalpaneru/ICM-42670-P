
#include "ICM42670P.h"

ICM42670P imu(SPI, 23);

imu_data_t reading;
void setup() {
    Serial.begin(115200);

    delay(1000);
    imu.begin();

    imu.accel_setting(2, 100);
    imu.gyro_setting();
    Serial.print("Setting done");
    delay(100);
}
void loop() {
    imu.get_imu_data(reading);

    Serial.print("\nx  | y | z\n");
    Serial.print(reading.xl[0]);
    Serial.print("  |  ");
    Serial.print(reading.xl[1]);
    Serial.print("  |  ");
    Serial.print(reading.xl[2]);

    Serial.print("\ng_x  | g_y | g_z\n");
    Serial.print(reading.gy[0]);
    Serial.print("  |  ");
    Serial.print(reading.gy[1]);
    Serial.print("  |  ");
    Serial.print(reading.gy[2]);

    Serial.print("\nTemp :");
    Serial.print(reading.temp);
    delay(1000);
}