#include "ICM42670P.h"

void ICM42670P::serial_read(uint8_t reg, uint8_t *read_buffer, uint32_t len) {
    imu_spi->beginTransaction(SPISettings(spi_clock, MSBFIRST, SPI_MODE0_AND_MODE3));

    digitalWrite(imu_cs, LOW);

    imu_spi->transfer(reg | 0x80);

    for (int i = 0; i < len; i++) {
        read_buffer[i] = imu_spi->transfer(0);
    }
    digitalWrite(imu_cs, HIGH);

    imu_spi->endTransaction();

    if ((debug_output != nullptr) and (debug_level > DEBUG_LEVEL1)) {
        debug_output->println((__FlashStringHelper *)(__func__));
        for (int i = 0; i < len; i++) {
            debug_output->println(read_buffer[i]);
        }
    }
}

void ICM42670P::serial_write(uint8_t reg, const uint8_t *write_buffer, uint32_t len) {
    imu_spi->beginTransaction(SPISettings(spi_clock, MSBFIRST, SPI_MODE0_AND_MODE3));

    digitalWrite(imu_cs, LOW);

    if ((debug_output != nullptr) and (debug_level > DEBUG_LEVEL1)) {
        debug_output->println((__FlashStringHelper *)(__func__));
        debug_output->println("write data:");

        for (int i = 0; i < len; i++) {
            debug_output->println(write_buffer[i]);
        }
    }

    imu_spi->transfer(reg);

    for (uint8_t i = 0; i < len; i++) {
        imu_spi->transfer(write_buffer[i]);
    }

    digitalWrite(imu_cs, HIGH);

    imu_spi->endTransaction();
}

ICM42670P::ICM42670P(SPIClass &_spi, uint8_t _cs) {
    imu_spi = &_spi;
    imu_cs = _cs;
    spi_clock = 1000000;
    g_range = 2.0;
    dps_range = 200.0;
    debug_output = nullptr;
    debug_level = 0;
}

float ICM42670P::readings_2_g(int16_t value) { return g_range * float(value) / ACCEL_FULL_SACLE_READING; }
float ICM42670P::readings_2_dps(int16_t value) { return dps_range * float(value) / GYRO_FULL_SACLE_READING; }

float ICM42670P::get_range_g() { return g_range; }
float ICM42670P::get_range_dps() { return dps_range; }

ICM42670P::~ICM42670P() {}

bool ICM42670P::begin() {
    imu_spi->begin();

    pinMode(imu_cs, OUTPUT);
    digitalWrite(imu_cs, HIGH);

    if (debug_output != nullptr) {
        debug_output->println((__FlashStringHelper *)(__func__));
    }

    return true;
}

bool ICM42670P::get_imu_raw_data(imu_raw_data_t &data) {
    uint8_t temp_data_1 = 0;
    uint8_t temp_data_0 = 0;

    uint8_t xl_x_1 = 0;
    uint8_t xl_x_0 = 0;

    uint8_t xl_y_1 = 0;
    uint8_t xl_y_0 = 0;

    uint8_t xl_z_1 = 0;
    uint8_t xl_z_0 = 0;

    uint8_t gyro_x_1 = 0;
    uint8_t gyro_x_0 = 0;

    uint8_t gyro_y_1 = 0;
    uint8_t gyro_y_0 = 0;

    uint8_t gyro_z_1 = 0;
    uint8_t gyro_z_0 = 0;

    serial_read(TEMP_DATA1, &temp_data_1, 1);
    serial_read(TEMP_DATA0, &temp_data_0, 1);

    serial_read(ACCEL_DATA_X1, &xl_x_1, 1);
    serial_read(ACCEL_DATA_X0, &xl_x_0, 1);

    serial_read(ACCEL_DATA_Y1, &xl_y_1, 1);
    serial_read(ACCEL_DATA_Y0, &xl_y_0, 1);

    serial_read(ACCEL_DATA_Z1, &xl_z_1, 1);
    serial_read(ACCEL_DATA_Z0, &xl_z_0, 1);

    serial_read(GYRO_DATA_X1, &gyro_x_1, 1);
    serial_read(GYRO_DATA_X0, &gyro_x_0, 1);

    serial_read(GYRO_DATA_Y1, &gyro_y_1, 1);
    serial_read(GYRO_DATA_Y0, &gyro_y_0, 1);

    serial_read(GYRO_DATA_Z1, &gyro_z_1, 1);
    serial_read(GYRO_DATA_Z0, &gyro_z_0, 1);

    data.xl[0] = (xl_x_1 << 8) | (xl_x_0);
    data.xl[1] = (xl_y_1 << 8) | (xl_y_0);
    data.xl[2] = (xl_z_1 << 8) | (xl_z_0);

    data.gy[0] = (gyro_x_1 << 8) | (gyro_x_0);
    data.gy[1] = (gyro_y_1 << 8) | (gyro_y_0);
    data.gy[2] = (gyro_z_1 << 8) | (gyro_z_0);

    data.temp = temp_data_1 << 8 | (temp_data_0);

    return true;
}

bool ICM42670P::get_imu_data(imu_data_t &data) {
    memset((void *)&data, 0, sizeof(imu_data_t));

    imu_raw_data_t raw_data;
    memset((void *)&raw_data, 0, sizeof(imu_raw_data_t));

    get_imu_raw_data(raw_data);

    data.xl[0] = readings_2_g(raw_data.xl[0]);
    data.xl[1] = readings_2_g(raw_data.xl[1]);
    data.xl[2] = readings_2_g(raw_data.xl[2]);

    data.gy[0] = readings_2_dps(raw_data.gy[0]);
    data.gy[1] = readings_2_dps(raw_data.gy[1]);
    data.gy[2] = readings_2_dps(raw_data.gy[2]);

    data.temp = float(raw_data.temp) / (128.0) + 25.0;

    if (debug_output != nullptr) {
        debug_output->println((__FlashStringHelper *)(__func__));
        if (debug_level >= DEBUG_LEVEL1) {
            Serial.print("(Ax,Ay,Az): ");
            debug_output->print("(");
            debug_output->print(data.xl[0]);
            debug_output->print(",");
            debug_output->print(data.xl[1]);
            debug_output->print(",");
            debug_output->print(data.xl[2]);
            debug_output->println(")");

            debug_output->print("(Gx,Gy,Gz): ");
            debug_output->print("(");
            debug_output->print(data.gy[0]);
            debug_output->print(",");
            debug_output->print(data.gy[1]);
            debug_output->print(",");
            debug_output->print(data.gy[2]);
            debug_output->println(")");

            debug_output->print("t(⁰C) :");
            debug_output->println(data.temp);
        }
    }

    return true;
}

uint8_t ICM42670P::accel_map_fsr(uint16_t fsr) {
    uint8_t config;
    switch (fsr) {
        case 2:
            config |= ACCEL_UI_FS_G_2;
            break;
        case 8:
            config |= ACCEL_UI_FS_G_8;
            break;
        case 4:
            config |= ACCEL_UI_FS_G_4;
            break;
        case 16:
            config |= ACCEL_UI_FS_G_16;
            break;
        default:
            config |= ACCEL_UI_FS_G_2;
            break;
    }
    return config;
}

uint8_t ICM42670P::accel_map_odr(uint16_t odr) {
    uint8_t config;
    switch (odr) {
        case 1600:
            config |= ACCEL_ODR_LN_1_6_kHz;
            break;
        case 800:
            config |= ACCEL_ODR_LN_800_Hz;
            break;
        case 400:
            config |= ACCEL_ODR_LP_LN_400_Hz;
            break;
        case 200:
            config |= ACCEL_ODR_LP_LN_200_Hz;
            break;
        case 100:
            config |= ACCEL_ODR_LP_LN_100_Hz;
            break;
        case 50:
            config |= ACCEL_ODR_LP_LN_50_Hz;
            break;
        case 25:
            config |= ACCEL_ODR_LP_LN_25_Hz;
            break;
        case 12:
            config |= ACCEL_ODR_LP_LN_12_5_Hz;
            break;
        case 6:
            config |= ACCEL_ODR_LP_6_25_Hz;
            break;
        case 3:
            config |= ACCEL_ODR_LP_3_125_Hz;
            break;
        case 1:
            config |= ACCEL_ODR_LP_1_5625_Hz;
            break;

        default:
            config |= ACCEL_ODR_LN_1_6_kHz;
            break;
    }
    return config;
}

uint8_t ICM42670P::accel_map_lpf_bw(uint8_t lpf_bw) {
    uint8_t config = 0;

    switch (lpf_bw) {
        case 0:
            config |= ACCEL_UI_FILT_BW_LPF_BW_BYPASSED;
            break;
        case 180:
            config |= ACCEL_UI_FILT_BW_LPF_BW_180_HZ;
            break;
        case 121:
            config |= ACCEL_UI_FILT_BW_LPF_BW_121_HZ;
            break;
        case 73:
            config |= ACCEL_UI_FILT_BW_LPF_BW_73_HZ;
            break;
        case 53:
            config |= ACCEL_UI_FILT_BW_LPF_BW_53_HZ;
            break;
        case 34:
            config |= ACCEL_UI_FILT_BW_LPF_BW_34_HZ;
            break;
        case 25:
            config |= ACCEL_UI_FILT_BW_LPF_BW_25_HZ;
            break;
        case 16:
            config |= ACCEL_UI_FILT_BW_LPF_BW_16_HZ;
            break;
        default:
            config |= ACCEL_UI_FILT_BW_LPF_BW_BYPASSED;
            break;
    }
    return config;
}

uint8_t ICM42670P::accel_map_av_filter(uint8_t av_filter) {
    uint8_t config = 0;

    switch (av_filter) {
        case 2:
            config |= ACCEL_UI_AVG_AVG_2X;
        case 4:
            break;
            config |= ACCEL_UI_AVG_AVG_4X;
        case 8:
            break;
            config |= ACCEL_UI_AVG_AVG_8X;
        case 16:
            break;
            config |= ACCEL_UI_AVG_AVG_16X;
        case 32:
            break;
            config |= ACCEL_UI_AVG_AVG_32X;
        case 64:
            config |= ACCEL_UI_AVG_AVG_64X;
            break;
        default:
            config |= ACCEL_UI_AVG_AVG_16X;
    }
    return config;
}

bool ICM42670P::accel_set_fsr(uint16_t setting) {
    bool success = false;
    g_range = float(setting);
    uint8_t fsr_setting = accel_map_fsr(setting);
    uint8_t valid_setting = fsr_setting;
    uint8_t status = 0;

    serial_read(ACCEL_CONFIG0, &status, sizeof(status));
    valid_setting |= status;

    serial_write(ACCEL_CONFIG0, &valid_setting, sizeof(valid_setting));
    serial_read(ACCEL_CONFIG0, &status, sizeof(status));

    if (status & fsr_setting == fsr_setting) {
        success = true;
    }

    if (debug_output != nullptr) {
        debug_output->print((__FlashStringHelper *)(__func__));
        debug_output->print(" success:");
        debug_output->println(success);

        if (debug_level >= DEBUG_LEVEL1) {
            debug_output->print("set fsr value: ");
            debug_output->print(fsr_setting);
            debug_output->print("\tread fsr value: ");
            debug_output->println(status);
        }
    }

    return success;
}

bool ICM42670P::accel_set_odr(uint16_t setting) {
    bool success = false;

    uint8_t accel_mode_status = 0;
    uint8_t accel_config_status = 0;

    uint8_t odr_setting = accel_map_odr(setting);
    uint8_t valid_setting = odr_setting;

    serial_read(ACCEL_CONFIG0, &accel_config_status, sizeof(accel_config_status));
    serial_read(PWR_MGMT0, &accel_mode_status, sizeof(accel_mode_status));
    valid_setting |= accel_config_status;

    // Possible ODR in Low Power mode
    if (odr_setting >= ACCEL_ODR_LP_LN_400_Hz and ((accel_mode_status & ACCEL_MODE_ACCEL_LOW_POWER) == ACCEL_MODE_ACCEL_LOW_POWER)) {
        serial_write(ACCEL_CONFIG0, &valid_setting, sizeof(valid_setting));
    }

    else if ((odr_setting >= ACCEL_ODR_LN_1_6_kHz) and odr_setting <= ACCEL_ODR_LP_LN_12_5_Hz and ((accel_mode_status & ACCEL_MODE_ACCEL_LOW_NOISE) == ACCEL_MODE_ACCEL_LOW_NOISE)) {
        serial_write(ACCEL_CONFIG0, &valid_setting, sizeof(valid_setting));
    }

    else {
        // do not change the previous state of the register
        serial_write(ACCEL_CONFIG0, &accel_config_status, sizeof(accel_config_status));
    }

    serial_read(ACCEL_CONFIG0, &accel_config_status, sizeof(accel_config_status));
    if (accel_config_status & odr_setting == odr_setting) {
        success = true;
    }

    if (debug_output != nullptr) {
        debug_output->print((__FlashStringHelper *)(__func__));
        debug_output->print(" success:");
        debug_output->println(success);

        if (debug_level >= DEBUG_LEVEL1) {
            debug_output->print("set odr value: ");
            debug_output->print(odr_setting);
            debug_output->print("\tread odr value: ");
            debug_output->println(accel_config_status);
        }
    }

    return success;
}

bool ICM42670P::accel_set_average_filter(uint16_t setting) {
    bool success = false;
    uint8_t filter_setting = setting;
    uint8_t valid_setting = filter_setting;
    uint8_t odr_status = 0;
    uint8_t status = 0;

    uint8_t accel_mode = 0;
    serial_read(PWR_MGMT0, &accel_mode, sizeof(accel_mode));
    serial_read(ACCEL_CONFIG0, &odr_status, sizeof(odr_status));
    serial_read(ACCEL_CONFIG1, &status, sizeof(odr_status));

    valid_setting |= status;

    if ((odr_status & ACCEL_ODR_LN_1_6_kHz) == ACCEL_ODR_LN_1_6_kHz or (odr_status & ACCEL_ODR_LN_800_Hz) == ACCEL_ODR_LN_800_Hz) {
        // any averaging is not possible
        // status unchanged
        valid_setting = status;
    }
    if ((odr_status & ACCEL_ODR_LP_LN_400_Hz) == ACCEL_ODR_LP_LN_400_Hz) {
        // 16x,32x,64x not possible.
        // status unchanged
        valid_setting = status;
    }

    if ((odr_status & ACCEL_ODR_LP_LN_200_Hz) == ACCEL_ODR_LP_LN_200_Hz) {
        // 64x is not possible
        // status unchanged
        valid_setting = status;
    }

    serial_write(ACCEL_CONFIG1, &valid_setting, sizeof(valid_setting));
    serial_read(ACCEL_CONFIG1, &status, sizeof(status));

    if ((status & filter_setting) == filter_setting) {
        success = true;
    }

    if (debug_output != nullptr) {
        debug_output->print((__FlashStringHelper *)(__func__));
        debug_output->print(" success:");
        debug_output->println(success);

        if (debug_level >= DEBUG_LEVEL1) {
            debug_output->print("set average setting: ");
            debug_output->print(filter_setting);
            debug_output->print("\tread average setting: ");
            debug_output->println(status);
        }
    }
    return success;
}

bool ICM42670P::accel_set_lpf_bandwidth(uint8_t setting) {
    uint8_t valid_setting = accel_map_lpf_bw(setting);
    uint8_t status = 0;
    bool success = false;

    serial_read(ACCEL_CONFIG1, &status, sizeof(status));

    valid_setting |= status;
    serial_write(ACCEL_CONFIG1, &valid_setting, sizeof(valid_setting));

    if ((status & valid_setting) == valid_setting) {
        success = true;
    }

    if (debug_output != nullptr) {
        debug_output->print((__FlashStringHelper *)(__func__));
        debug_output->print(" success:");
        debug_output->println(success);

        if (debug_level >= DEBUG_LEVEL1) {
            debug_output->print("set lpf setting: ");
            debug_output->print(valid_setting);
            debug_output->print("\tread lpf setting: ");
            debug_output->println(status);
        }
    }
    return success;
}

bool ICM42670P::accel_set_mode(uint8_t setting) {
    uint8_t status = 0;
    uint8_t valid_setting = setting;
    bool success = false;

    serial_read(PWR_MGMT0, &status, sizeof(status));
    valid_setting |= status;

    // accelerometer needs at least 200us delay if beign turned on from the off state.
    /*Dont know why a delay is needed before writting the acceleration mode value*/
    if ((status & ACCEL_MODE_ACCEL_OFF) == ACCEL_MODE_ACCEL_OFF) {
        delay(1);
        serial_write(PWR_MGMT0, &valid_setting, sizeof(valid_setting));
        delay(1);

    }

    else {
        serial_write(PWR_MGMT0, &valid_setting, sizeof(valid_setting));
    }

    serial_read(PWR_MGMT0, &status, sizeof(status));
    if ((status & setting) == setting) {
        success = true;
    }

    if (debug_output != nullptr) {
        debug_output->print((__FlashStringHelper *)(__func__));
        debug_output->print(" success:");
        debug_output->println(success);

        if (debug_level >= DEBUG_LEVEL1) {
            debug_output->print("set accel mode: ");
            debug_output->print(setting);
            debug_output->print("\tread accel mode: ");
            debug_output->println(status);
        }
    }

    return success;
}

bool ICM42670P::gyro_set_fsr(uint16_t setting) {
    uint8_t fsr_setting = gyro_map_fsr(setting);
    uint8_t valid_setting = fsr_setting;

    uint8_t status = 0;
    bool success = false;
    dps_range = float(setting);

    serial_read(GYRO_CONFIG0, &status, sizeof(status));
    valid_setting |= status;

    serial_write(GYRO_CONFIG0, &valid_setting, sizeof(valid_setting));
    serial_read(GYRO_CONFIG0, &status, sizeof(status));

    if ((status & fsr_setting) == fsr_setting) {
        success = true;
    }
    if (debug_output != nullptr) {
        debug_output->print((__FlashStringHelper *)(__func__));
        debug_output->print(" success:");
        debug_output->println(success);

        if (debug_level >= DEBUG_LEVEL1) {
            debug_output->print("set fsr value: ");
            debug_output->print(fsr_setting);
            debug_output->print("\tread fsr value: ");
            debug_output->println(status);
        }
    }
    return success;
}

bool ICM42670P::gyro_set_odr(uint16_t setting) {
    bool success = false;

    uint8_t odr_setting = gyro_map_odr(setting);
    uint8_t valid_setting = odr_setting;
    uint8_t status = 0;

    serial_read(GYRO_CONFIG0, &status, sizeof(status));
    valid_setting |= status;

    serial_write(GYRO_CONFIG0, &valid_setting, sizeof(valid_setting));
    serial_read(GYRO_CONFIG0, &status, sizeof(status));
    if ((status & odr_setting) == odr_setting) {
        success = true;
    }
    if (debug_output != nullptr) {
        debug_output->println((__FlashStringHelper *)(__func__));

        if (debug_level >= DEBUG_LEVEL1) {
            debug_output->print("set odr value: ");
            debug_output->print(odr_setting);
            debug_output->print("\tread odr value: ");
            debug_output->println(status);
        }
    }
    return success;
}

bool ICM42670P::gyro_set_lpf_bandwidth(uint8_t setting) {
    bool success = false;

    uint8_t lpf_setting = gyro_map_lpf_bw(setting);
    uint8_t valid_setting = lpf_setting;
    uint8_t status = 0;

    serial_read(GYRO_CONFIG0, &status, sizeof(status));
    valid_setting |= status;

    serial_write(GYRO_CONFIG0, &valid_setting, sizeof(valid_setting));
    serial_read(GYRO_CONFIG0, &status, sizeof(status));
    if ((status & lpf_setting) == lpf_setting) {
        success = true;
    }
    if (debug_output != nullptr) {
        debug_output->println((__FlashStringHelper *)(__func__));

        if (debug_level >= DEBUG_LEVEL1) {
            debug_output->print("set lpf setting: ");
            debug_output->print(valid_setting);
            debug_output->print("\tread lpf setting: ");
            debug_output->println(status);
        }
    }
    return success;
}
/**
 * @brief Power setting of the gyroscope
 *
 * @param setting Low noise, standby or off settings
 * @return true if successful setting is done
 * @return false otherwise
 */
bool ICM42670P::gyro_set_mode(uint8_t setting) {
    bool success = false;
    uint8_t status = 0;
    uint8_t valid_setting = setting;

    serial_read(PWR_MGMT0, &status, sizeof(status));
    valid_setting |= status;
    // if gyro is initially off it has to be left on for minimun of 45ms
    // no writes to be issued for 200us, thus delay of 50ms
    if ((status & GYRO_OFF) == GYRO_OFF) {
        serial_write(PWR_MGMT0, &valid_setting, sizeof(setting));
        delay(50);
    } else {
        serial_write(PWR_MGMT0, &valid_setting, sizeof(setting));
    }

    serial_read(PWR_MGMT0, &status, sizeof(status));
    if ((status & setting) == setting) {
        success = true;
    }

    if (debug_output != nullptr) {
        debug_output->println((__FlashStringHelper *)(__func__));
        if (debug_level >= DEBUG_LEVEL1) {
            debug_output->print("set gyro mode: ");
            debug_output->print(setting);
            debug_output->print("\tread gyro mode: ");
            debug_output->println(status);
        }
    }
    return success;
}
/**
 * @brief Configures interrrupts INT1 and INT2
 *
 * @param setting Configuration byte for INT_CONFIG register
 * @return true if written successfully
 * @return false else
 */
bool ICM42670P::configure_interrupt(uint8_t setting) {
    bool success = false;
    uint8_t read_setting = 0;
    serial_write(INT_CONFIG, &setting, sizeof(setting));
    serial_read(INT_CONFIG, &read_setting, sizeof(read_setting));
    if ((read_setting & setting) == setting) {
        success = true;
    }
    if (debug_output != nullptr) {
        debug_output->println((__FlashStringHelper *)(__func__));
        if (debug_level >= DEBUG_LEVEL1) {
            debug_output->print(" success: ");
            debug_output->println(success);
        }
    }
    return success;
}
/**
 * @brief Checks WOM/SMD interrupt source
 *
 * @return Non zeros interrupt status byte based on INT_STATUS2 Register in case of event otherwise zero.
 *
 */
uint8_t ICM42670P::interrupt_status_smd_wom() {
    uint8_t int_status = 0;
    serial_read(INT_STATUS2, &int_status, sizeof(int_status));
    return int_status;
}
/**
 * @brief Set the Low Pass filter bandwith of the case temperature sensor
 * @param lpf_bw bandwidth parameter see TEMP_FILT_BWxx enum for the required parameters
 * @return true if written successfully
 * @return false otherwise
 */
bool ICM42670P ::temp_set_lpf(uint8_t lpf_bw) {
    bool success = false;
    uint8_t status = 0;
    serial_write(TEMP_CONFIG0, &lpf_bw, sizeof(lpf_bw));
    serial_read(TEMP_CONFIG0, &status, sizeof(status));

    if ((status & lpf_bw) == lpf_bw) {
        success = true;
    }

    if (debug_output != nullptr) {
        debug_output->println((__FlashStringHelper *)(__func__));
        if (debug_level >= DEBUG_LEVEL1) {
            debug_output->print(" set temp lpf: ");
            debug_output->print(lpf_bw);
            debug_output->print("\tread temp lpf: ");
            debug_output->println(status);
        }
    }
    return success;
}

/**
 * @brief Sets the full scale reading(fsr) and output data rate(odr)
 * Have to set_accel_mode() before using this
 * select the allowed odr and fsr values only, from the datasheet
 * @param fsr full sacle reading value
 * @param odr output data rate
 * @return true if success
 * @return false otherwise
 */
bool ICM42670P::accel_setting(uint16_t fsr, uint16_t odr) {
    bool success = false;
    if (debug_output != nullptr) {
        debug_output->println((__FlashStringHelper *)(__func__));
    }

    success = accel_set_fsr(fsr);
    success &= accel_set_odr(odr);

    return success;
}
/**
 * @brief Sets the gyro on in the low noise mode on default
 *
 * @param fsr full scale reading
 * @param odr output data rate
 * @return true if successfully value is set
 * @return false otherwise
 */
bool ICM42670P::gyro_setting(uint16_t fsr, uint16_t odr) {
    bool success = false;

    if (debug_output != nullptr) {
        debug_output->println((__FlashStringHelper *)(__func__));
    }

    success = gyro_set_mode(GYRO_LOWNOISE);
    success &= gyro_set_fsr(fsr);
    success &= gyro_set_odr(odr);

    return success;
}

/**
 * @brief Configures the wakeup on motion interrupts
 *
 * @param setting use the values for WOM_CONFIG register
 * @return true if written successful
 * @return false  otherwise
 */
bool ICM42670P::configure_wakeup_on_motion(uint8_t setting) {
    bool success = false;
    uint8_t read_setting = 0;
    // Enables wakeup on motion
    // return (write_register(WOM_CONFIG, setting));
    serial_write(WOM_CONFIG, &setting, sizeof(setting));
    serial_read(WOM_CONFIG, &read_setting, sizeof(read_setting));
    if ((read_setting & setting) == setting) {
        success = true;
    }
    if (debug_output != nullptr) {
        debug_output->print((__FlashStringHelper *)(__func__));
        if (debug_level >= DEBUG_LEVEL1) {
            debug_output->print(" success: ");
            debug_output->println(success);
        }
    }
    return success;
}

/**
 * @brief Maps the Wakeup on motion to INT1 or INT2 pin
 *
 * @param interrupt_no 1 to map to INT1 and 2 to map 2 INT2
 * @param settings use values according to INT_SOURCE1 register for making WoM to INT1
 * and INT_SOURCE3 for WoM mapped to INT2 *
 * @return true if settings written successfull
 * @return false otherwise
 */
bool ICM42670P::route_wom_smd_to_int(uint8_t interrupt_no, uint8_t settings) {
    // this will ensure WoM is enabled on call to this function
    // bool success = configure_wakeup_on_motion(WOM_CONFIG_WOM_ENABLE);
    bool success = false;
    uint8_t read_setting = 0;

    switch (interrupt_no) {
        case 1:
            serial_write(INT_SOURCE1, &settings, sizeof(settings));
            serial_read(INT_SOURCE1, &read_setting, sizeof(read_setting));
            break;

        case 2:
            serial_write(INT_SOURCE4, &settings, sizeof(settings));
            serial_read(INT_SOURCE4, &read_setting, sizeof(read_setting));
            break;

        default:
            break;
    }
    if (read_setting == settings) {
        success = true;
    }

    if (debug_output != nullptr) {
        debug_output->print((__FlashStringHelper *)(__func__));
        if (debug_level >= DEBUG_LEVEL1) {
            debug_output->print(" success: ");
            debug_output->println(success);
        }
    }
    return success;
}

/*Incomplete needs pedometer to be enabled to have SMD running */
bool ICM42670P::enable_significant_motion_detection(bool enable, uint8_t sensitivity) {
    bool success = false;

    return success;
}

bool ICM42670P::wom_x_threshold(float threshold) {
    bool success = false;
    uint8_t threshold_byte_read = 0;
    uint8_t threshold_byte = threshold_mg_to_byte(threshold);

    write_mreg1_register(ACCEL_WOM_X_THR_MREG1, &threshold_byte);
    read_mreg1_register(ACCEL_WOM_X_THR_MREG1, &threshold_byte_read);

    if (threshold_byte_read == threshold_byte) {
        success = true;
    }

    if (debug_output != nullptr) {
        debug_output->print((__FlashStringHelper *)(__func__));

        if (debug_level >= DEBUG_LEVEL1) {
            debug_output->print(F(" (mg):"));
            debug_output->print(threshold);

            debug_output->print("\tset threshold byte:");
            debug_output->print(threshold_byte);
            debug_output->print("\tread threshold byte:");
            debug_output->println(threshold_byte_read);
        }
    }
    return success;
}
bool ICM42670P::wom_y_threshold(float threshold) {
    bool success = false;
    // Also, limit the threshold [0-255]
    uint8_t threshold_byte_read = 0;
    uint8_t threshold_byte = threshold_mg_to_byte(threshold);

    write_mreg1_register(ACCEL_WOM_Y_THR_MREG1, &threshold_byte);
    read_mreg1_register(ACCEL_WOM_Y_THR_MREG1, &threshold_byte_read);

    if (threshold_byte_read == threshold_byte) {
        success = true;
    }

    if (debug_output != nullptr) {
        debug_output->print((__FlashStringHelper *)(__func__));

        if (debug_level >= DEBUG_LEVEL1) {
            debug_output->print(F(" (mg):"));
            debug_output->print(threshold);

            debug_output->print("\tset threshold byte:");
            debug_output->print(threshold_byte);
            debug_output->print("\tread threshold byte:");
            debug_output->println(threshold_byte_read);
        }
    }
    return success;
}
bool ICM42670P::wom_z_threshold(float threshold) {
    bool success = false;
    // Also, limit the threshold [0-255]
    uint8_t threshold_byte_read = 0;
    uint8_t threshold_byte = threshold_mg_to_byte(threshold);

    write_mreg1_register(ACCEL_WOM_Z_THR_MREG1, &threshold_byte);
    read_mreg1_register(ACCEL_WOM_Z_THR_MREG1, &threshold_byte_read);

    if (threshold_byte_read == threshold_byte) {
        success = true;
    }

    if (debug_output != nullptr) {
        debug_output->print((__FlashStringHelper *)(__func__));

        if (debug_level >= DEBUG_LEVEL1) {
            debug_output->print(F(" (mg):"));
            debug_output->print(threshold);

            debug_output->print("\tset threshold byte:");
            debug_output->print(threshold_byte);
            debug_output->print("\tread threshold byte:");
            debug_output->println(threshold_byte_read);
        }
    }
    return success;
}

/**
 * @brief Maps the gyroscopes ODR input to corresposnding byte to be written onto the register
 * @param odr Output data rate in dps
 * @return uint8_t byte to be written in the register
 */
uint8_t ICM42670P::gyro_map_odr(uint16_t odr) {
    uint8_t config = 0;

    switch (odr) {
        case 1600:
            config |= GYRO_ODR_ODR_1600HZ;

            break;
        case 800:
            config |= GYRO_ODR_ODR_800HZ;

            break;
        case 400:
            config |= GYRO_ODR_ODR_400HZ;

            break;
        case 200:
            config |= GYRO_ODR_ODR_200HZ;

            break;
        case 100:
            config |= GYRO_ODR_ODR_100HZ;

            break;
        case 50:
            config |= GYRO_ODR_ODR_50HZ;

            break;
        case 25:
            config |= GYRO_ODR_ODR_25HZ;

            break;
        case 12:
            config |= GYRO_ODR_ODR_12_5HZ;
            break;
    }
    return config;
}

uint8_t ICM42670P::gyro_map_fsr(uint16_t fsr) {
    uint8_t config = 0;

    switch (fsr) {
        case 2000:
            config |= GYRO_UI_FS_DPS_2000;
            break;

        case 1000:
            config |= GYRO_UI_FS_DPS_1000;
            break;

        case 500:
            config |= GYRO_UI_FS_DPS_500;
            break;

        case 250:
            config |= GYRO_UI_FS_DPS_250;
            break;
        default:
            config |= GYRO_UI_FS_DPS_250;
            break;
    }

    return config;
}

uint8_t ICM42670P::gyro_map_lpf_bw(uint8_t lpf_bw) {
    uint8_t config = 0;

    switch (lpf_bw) {
        case 0:
            config |= GYRO_UI_FILT_BW_LPF_BW_BYPASSED;
            break;
        case 180:
            config |= GYRO_UI_FILT_BW_LPF_BW_180_HZ;
            break;
        case 121:
            config |= GYRO_UI_FILT_BW_LPF_BW_121_HZ;
            break;
        case 73:
            config |= GYRO_UI_FILT_BW_LPF_BW_73_HZ;
            break;
        case 53:
            config |= GYRO_UI_FILT_BW_LPF_BW_53_HZ;
            break;
        case 34:
            config |= GYRO_UI_FILT_BW_LPF_BW_34_HZ;
            break;
        case 25:
            config |= GYRO_UI_FILT_BW_LPF_BW_25_HZ;
            break;
        case 16:
            config |= GYRO_UI_FILT_BW_LPF_BW_16_HZ;
            break;
        default:
            config |= GYRO_UI_FILT_BW_LPF_BW_BYPASSED;
            break;
    }
    return config;
}

uint8_t ICM42670P::threshold_mg_to_byte(float threshold) {
    uint8_t threshold_byte = 0;

    if (threshold >= 1000) {
        threshold_byte = 255;
    }

    else if (threshold < 0) {
    }

    else {
        threshold_byte = uint8_t(uint16_t(threshold * THRESHOLD_SENSITIVITY_MG_TO_BYTE) % 256);
    }

    return threshold_byte;
}
void ICM42670P::set_debug_stream(Stream *debug_stream, uint8_t level) {
    debug_output = debug_stream;
    debug_level = level;
}
/**
 * @brief Writes to the specified register preserving previous settings
 * Also, checks if the correct value is written
 * @param reg Rgister address to be written
 * @param setting value to be written to the register reg
 * @return true if written successfully
 * @return false
 */
bool ICM42670P::write_register(uint8_t reg, uint8_t setting) {
    bool success = false;
    uint8_t status = 0;
    uint8_t valid_setting = setting;

    serial_read(reg, &status, sizeof(status));
    valid_setting |= setting;
    serial_write(reg, &valid_setting, sizeof(valid_setting));

    serial_read(reg, &status, sizeof(status));
    if ((status & setting) == setting) {
        success = true;
    }

    return success;
}

/**
 * @brief Writes to the register in MREG1 register bank
 * @param reg MREG1 register address
 * @param write_value value to be written to MREG1 group register
 * @return true if written is successful
 * @return false otherwise
 */
bool ICM42670P::write_mreg1_register(uint8_t reg, uint8_t *write_value) { return (write_mreg_registers(USER_BANK_MREG_1, reg, write_value)); }

/**
 * @brief Writes to the register in MREG1 register bank
 * @param reg MREG1 register address
 * @param write_value value to be written to MREG1 group register
 * @return true if written is successful
 * @return false otherwise
 */
bool ICM42670P::read_mreg1_register(uint8_t reg, uint8_t *read_value) { return (read_mreg_registers(USER_BANK_MREG_1, reg, read_value)); }

/**
 * @brief Reads the value from the MREG registers
 * When in sleep mode or accelerometer LP mode with WUOSC,
 * MREG1, MREG2 or MREG3 read/write operations require the user to power on the RC oscillator
 * using register field IDLE from register PWR_MGMT0.
 * @param register_bank Register bank to be accessed
 * @param reg Register address inside the bank
 * @param read_value value that will be read
 * @return true if read is successfull
 * @return false otherwise
 */
bool ICM42670P::read_mreg_registers(uint8_t register_bank, uint8_t reg, uint8_t *read_value) {
    uint8_t mreg_blk_sel = register_bank;

    bool success = false;

    uint8_t mclk_status = 0;
    uint8_t accel_status = 0;

    serial_read(PWR_MGMT0, &accel_status, sizeof(accel_status));

    // If accelerometer is in LP or off use the IDLE register to power up RC oscillator
    if ((accel_status & ACCEL_MODE_ACCEL_LOW_NOISE) < ACCEL_MODE_ACCEL_LOW_NOISE) {
        uint8_t rc_osc_setting = IDLE_RC_OSC_ON;
        success = write_register(PWR_MGMT0, rc_osc_setting);
    }
    // Check MCLK_RDY to confirm internal clock is running before accessing MREG registers
    // confirmed by checking BIT 3 status
    serial_read(MCLK_RDY, &mclk_status, sizeof(mclk_status));

    if (mclk_status & (0b1 << 3)) {
        serial_write(BLK_SEL_R, &mreg_blk_sel, sizeof(mreg_blk_sel));
        serial_write(MADDR_R, &reg, sizeof(reg));
        delay(1);
        serial_read(M_R, read_value, sizeof(*read_value));
        delay(1);
        success = true;
    }

    if (debug_output != nullptr and debug_level > DEBUG_LEVEL1) {
        debug_output->println((__FlashStringHelper *)(__func__));
        debug_output->print(F("mclk_rdy status:"));
        debug_output->println(mclk_status);

        debug_output->print(F("accel_status:"));
        debug_output->println(accel_status);

        debug_output->print(F("M_R read value:"));

        for (int i = 0; i < sizeof(*read_value); i++) {
            debug_output->print(F("read_value["));
            debug_output->print(i);
            debug_output->print(F("]: "));
            debug_output->println(read_value[i], HEX);
        }
    }

    return success;
}

/**
 * @brief Reads the value from the MREG registers
 * When in sleep mode or accelerometer LP mode with WUOSC,
 * MREG1, MREG2 or MREG3 read/write operations require the user to power on the RC oscillator
 * using register field IDLE from register PWR_MGMT0.
 * @param register_bank Register bank to be accessed
 * @param reg Register address inside the bank
 * @param read_value value that will be read
 * @return true if read is successfull
 * @return false otherwise
 */
bool ICM42670P::write_mreg_registers(uint8_t register_bank, uint8_t reg, uint8_t *write_value) {
    uint8_t mreg_blk_sel = register_bank;

    bool success = false;

    uint8_t mclk_status = 0;
    uint8_t accel_status = 0;

    serial_read(PWR_MGMT0, &accel_status, sizeof(accel_status));

    // If accelerometer is in LP or off use the IDLE register to power up RC oscillator
    if ((accel_status & ACCEL_MODE_ACCEL_LOW_NOISE) < ACCEL_MODE_ACCEL_LOW_NOISE) {
        uint8_t rc_osc_setting = IDLE_RC_OSC_ON;
        success = write_register(PWR_MGMT0, rc_osc_setting);
    }
    // Check MCLK_RDY to confirm internal clock is running before accessing MREG registers
    // confirmed by checking BIT 3 status
    serial_read(MCLK_RDY, &mclk_status, sizeof(mclk_status));
    if (mclk_status & (0b1 << 3)) {
        serial_write(BLK_SEL_W, &mreg_blk_sel, sizeof(mreg_blk_sel));
        serial_write(MADDR_W, &reg, sizeof(reg));
        serial_write(M_W, write_value, sizeof(*write_value));

        delay(1);
        success = true;
    }
    if (debug_output != nullptr) {
        debug_output->println((__FlashStringHelper *)(__func__));

        if (debug_level > DEBUG_LEVEL1) {
            debug_output->print(":");
            debug_output->println(success);

            debug_output->print("written value:");

            for (int i = 0; i < sizeof(*write_value); i++) {
                debug_output->println(write_value[i]);
            }
        }
    }

    return success;
}
