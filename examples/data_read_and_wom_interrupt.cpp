
#include "ICM42670P.h"

ICM42670P imu(SPI, SDIO_DATA3);
uint32_t int_count = 0;
imu_data_t reading;
bool read_int = false;

void imu_int1_isr() {
    int_count++;
    read_int = true;
}

void setup() {
    Serial.begin(115200);

    delay(1000);
    imu.begin();
    pinMode(G10, INPUT);
    attachInterrupt(G10, imu_int1_isr, LOW);

    pinMode(G1, OUTPUT);
    digitalWrite(G1, HIGH);

    imu.set_debug_stream(&Serial, DEBUG_LEVEL1);

    bool success = false;
    success = imu.accel_set_mode(ACCEL_MODE_ACCEL_LOW_NOISE);

    success = imu.accel_setting(2, 800);

    success = imu.gyro_setting(2000, 200);

    success = imu.temp_set_lpf(TEMP_FILT_BW_DLPF_BYPASSED);

    success = imu.configure_interrupt(INT1_MODE_LATCHED | INT1_DRIVE_OPEN_DRAIN | INT1_POLARITY_ACTIVE_LOW);

    success = imu.wom_x_threshold(10);

    success = imu.wom_y_threshold(10);

    success = imu.wom_z_threshold(10);
    delay(1);

    // clang-format off
    success = imu.configure_wakeup_on_motion( WOM_INT_DUR_INT_1ST_EVENT |
                                              WOM_INT_MODE_AND_OF_EN_THRESHOLD |
                                              WOM_MODE_COMPARE_CURRENT_TO_PREVIOUS |
                                              WOM_CONFIG_WOM_ENABLE);

    // success = imu.configure_wakeup_on_motion( WOM_INT_DUR_INT_1ST_EVENT |
    //                                           WOM_INT_AND_OF_EN_THRESHOLD |
    //                                           WOM_MODE_COMPARE_FUTURE_TO_INITIAL|
    //                                           WOM_CONFIG_WOM_ENABLE);

    // success = imu.configure_wakeup_on_motion( WOM_INT_DUR_INT_1ST_EVENT |
    //                                           WOM_INT_AND_OF_EN_THRESHOLD |
    //                                           WOM_MODE_COMPARE_FUTURE_TO_INITIAL|
    //                                           WOM_CONFIG_WOM_ENABLE);
    // clang-format on

    delay(1);
    success = imu.route_wom_smd_to_int(1, WOM_Z_INT1_ROUTED | WOM_Y_INT1_ROUTED | WOM_X_INT1_ROUTED);
    delay(3000);
}
void loop() {
    imu.get_imu_data(reading);

    Serial.printf("\nInt count: %d\n", int_count);
    uint8_t int_status = imu.interrupt_status_smd_wom();
    Serial.printf("Int read status : %X\n", int_status);

    if (int_status > 0) {
        am_hal_interrupt_master_disable();

        /* Do other stuff after the interrupt*/
    }
    am_hal_interrupt_master_enable();
    delay(1000);
}