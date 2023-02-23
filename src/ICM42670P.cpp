#include "ICM42670P.h"

void ICM42670P::spi_read(uint8_t reg, uint8_t *read_buffer, uint32_t len) {
    imu_spi->beginTransaction(SPISettings(spi_clock, MSBFIRST, SPI_MODE0));

    digitalWrite(imu_cs, LOW);

    imu_spi->transfer(reg | 0x80);
    imu_spi->transfer(read_buffer, len);
    digitalWrite(imu_cs, HIGH);

    imu_spi->endTransaction();
}

void ICM42670P::spi_write(uint8_t reg, const uint8_t *write_buffer, uint32_t len) {
    imu_spi->beginTransaction(SPISettings(spi_clock, MSBFIRST, SPI_MODE0));

    digitalWrite(imu_cs, LOW);

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

    reading_2_g = 2.0 / 32767.0;
    reading_2_dps = 2000.0 / 32767.0;
}

ICM42670P::~ICM42670P() {}

int8_t ICM42670P::begin() {
    imu_spi->begin();
    pinMode(imu_cs, OUTPUT);
    digitalWrite(imu_cs, HIGH);

    // uint8_t read_value = 0;

    // uint8_t power_mgmt_config =
    //     uint8_t(PWR_MGMT0_T::GYRO_MODE::GYRO_LOWNOISE | PWR_MGMT0_T::ACCEL_LP_CLK_SEL::WAKEUP_OSCI | PWR_MGMT0_T::ACCEL_MODE::ACCEL_LOW_NOISE);

    // spi_write(user_bank_0_register_t::PWR_MGMT0, &power_mgmt_config, 1);
    // delay(100);
    // spi_read(user_bank_0_register_t::PWR_MGMT0, &read_value, 1);

    // Serial.print("PWR mgmt set_val:");
    // Serial.println(uint8_t(power_mgmt_config), BIN);

    // Serial.print("Read Val:");
    // Serial.println(read_value, BIN);
    // read_value = 0;

    // uint8_t gyro_config_0 = GYRO_CONFIG_0_T::GYRO_UI_FS_SEL::DPS_2000 | GYRO_CONFIG_0_T::GYRO_ODR::ODR_400HZ;
    // spi_write(user_bank_0_register_t::GYRO_CONFIG0, &gyro_config_0, 1);
    // delay(100);
    // spi_read(user_bank_0_register_t::GYRO_CONFIG0, &read_value, 1);

    // uint8_t accel_config_0 = ACCEL_CONFIG_0_T::ACCEL_UI_FS_SEL::G_2 | ACCEL_CONFIG_0_T::ACCEL_ODR::LP_LN_100_Hz;

    // spi_write(user_bank_0_register_t::ACCEL_CONFIG0, &accel_config_0, 1);
    // delay(100);
    // spi_read(user_bank_0_register_t::ACCEL_CONFIG0, &read_value, 1);

    // uint8_t temp_config_0 = TEMP_CONFIG0_T::TEMP_FILT_BW::DLPF_BW_16_HZ;
    // spi_write(user_bank_0_register_t::TEMP_CONFIG0, &temp_config_0, 1);
    // spi_write(user_bank_0_register_t::TEMP_CONFIG0, &read_value, 1);

    // uint8_t gyro_config_1 = GYRO_CONFIG1_T::GYRO_UI_FILT_BW::LPF_BW_180_HZ;

    // spi_write(user_bank_0_register_t::GYRO_CONFIG1, &gyro_config_1, 1);
    // delay(100);
    // spi_read(user_bank_0_register_t::GYRO_CONFIG1, &read_value, 1);

    // uint8_t accel_config_1 = ACCEL_CONFIG1_T::ACCEL_UI_AVG::AVG_2X | ACCEL_CONFIG1_T::ACCEL_UI_FILT_BW::LPF_BW_180_HZ;

    // spi_read(user_bank_0_register_t::ACCEL_CONFIG1, &read_value, 1);

    return 0;
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

    spi_read(user_bank_0_register_t::TEMP_DATA1, &temp_data_1, 1);
    spi_read(user_bank_0_register_t::TEMP_DATA0, &temp_data_0, 1);

    spi_read(user_bank_0_register_t::ACCEL_DATA_X1, &xl_x_1, 1);
    spi_read(user_bank_0_register_t::ACCEL_DATA_X0, &xl_x_0, 1);

    spi_read(user_bank_0_register_t::ACCEL_DATA_Y1, &xl_y_1, 1);
    spi_read(user_bank_0_register_t::ACCEL_DATA_Y0, &xl_y_0, 1);

    spi_read(user_bank_0_register_t::ACCEL_DATA_Z1, &xl_z_1, 1);
    spi_read(user_bank_0_register_t::ACCEL_DATA_Z0, &xl_z_0, 1);

    spi_read(user_bank_0_register_t::GYRO_DATA_X1, &gyro_x_1, 1);
    spi_read(user_bank_0_register_t::GYRO_DATA_X0, &gyro_x_0, 1);

    spi_read(user_bank_0_register_t::GYRO_DATA_Y1, &gyro_y_1, 1);
    spi_read(user_bank_0_register_t::GYRO_DATA_Y0, &gyro_y_0, 1);

    spi_read(user_bank_0_register_t::GYRO_DATA_Z1, &gyro_z_1, 1);
    spi_read(user_bank_0_register_t::GYRO_DATA_Z0, &gyro_z_0, 1);

    data.xl[0] = (xl_x_1 << 8) | (xl_x_0);
    data.xl[1] = (xl_y_1 << 8) | (xl_y_0);
    data.xl[2] = (xl_z_1 << 8) | (xl_z_0);

    data.gy[0] = (gyro_x_1 << 8) | (gyro_x_0);
    data.gy[1] = (gyro_y_1 << 8) | (gyro_y_0);
    data.gy[2] = (gyro_z_1 << 8) | (gyro_z_0);

    data.temp = float((temp_data_1 << 8) | (temp_data_0)) / (128.0) + 25.0;

    return true;
}

bool ICM42670P::get_imu_data(imu_data_t &data) {
    memset((void *)&data, 0, sizeof(imu_data_t));

    imu_raw_data_t raw_data;
    memset((void *)&raw_data, 0, sizeof(imu_raw_data_t));

    get_imu_raw_data(raw_data);

    data.xl[0] = unsigned_val_2_float(raw_data.xl[0]) * reading_2_g;
    data.xl[1] = unsigned_val_2_float(raw_data.xl[1]) * reading_2_g;
    data.xl[2] = unsigned_val_2_float(raw_data.xl[2]) * reading_2_g;

    data.gy[0] = unsigned_val_2_float(raw_data.gy[0]) * reading_2_dps;
    data.gy[1] = unsigned_val_2_float(raw_data.gy[1]) * reading_2_dps;
    data.gy[2] = unsigned_val_2_float(raw_data.gy[2]) * reading_2_dps;

    data.temp = raw_data.temp;

    return true;
}

float ICM42670P::unsigned_val_2_float(uint16_t val) {
    float return_value = 0;
    if (val > 0x7FFF) {
        return_value = float(val) - float(0xFFFF) - float(1);
    } else {
        return_value = float(val);
    }

    return return_value;
}

uint8_t ICM42670P::accel_map_fsr(uint16_t fsr) {
    uint8_t config;
    switch (fsr) {
        case 2:
            config |= ACCEL_CONFIG_0_T::ACCEL_UI_FS_SEL::G_2;
            reading_2_g = float(fsr) / float(32767);
            break;
        case 8:
            config |= ACCEL_CONFIG_0_T::ACCEL_UI_FS_SEL::G_8;
            reading_2_g = float(fsr) / float(32767);
            break;
        case 4:
            config |= ACCEL_CONFIG_0_T::ACCEL_UI_FS_SEL::G_4;
            reading_2_g = float(fsr) / float(32767);
            break;
        case 16:
            config |= ACCEL_CONFIG_0_T::ACCEL_UI_FS_SEL::G_16;
            reading_2_g = float(fsr) / float(32767);
            break;
        default:
            config |= ACCEL_CONFIG_0_T::ACCEL_UI_FS_SEL::G_2;
            reading_2_g = float(2.0) / float(32767);
            break;
    }
    Serial.print("FSR:");
    Serial.println(fsr);
    Serial.print("reading to g");
    Serial.println(reading_2_g, 10);
    return config;
}

uint8_t ICM42670P::accel_map_odr(uint16_t odr) {
    uint8_t config;
    switch (odr) {
        case 1600:
            config |= ACCEL_CONFIG_0_T::ACCEL_ODR::LN_1_6_kHz;
            break;
        case 800:
            config |= ACCEL_CONFIG_0_T::ACCEL_ODR::LN_800_Hz;
            break;
        case 400:
            config |= ACCEL_CONFIG_0_T::ACCEL_ODR::LP_LN_400_Hz;
            break;
        case 200:
            config |= ACCEL_CONFIG_0_T::ACCEL_ODR::LP_LN_200_Hz;
            break;
        case 100:
            config |= ACCEL_CONFIG_0_T::ACCEL_ODR::LP_LN_100_Hz;
            break;
        case 50:
            config |= ACCEL_CONFIG_0_T::ACCEL_ODR::LP_LN_50_Hz;
            break;
        case 25:
            config |= ACCEL_CONFIG_0_T::ACCEL_ODR::LP_LN_25_Hz;
            break;
        case 12:
            config |= ACCEL_CONFIG_0_T::ACCEL_ODR::LP_LN_12_5_Hz;
            break;
        case 6:
            config |= ACCEL_CONFIG_0_T::ACCEL_ODR::LP_6_25_Hz;
            break;
        case 3:
            config |= ACCEL_CONFIG_0_T::ACCEL_ODR::LP_3_125_Hz;
            break;
        case 1:
            config |= ACCEL_CONFIG_0_T::ACCEL_ODR::LP_1_5625_Hz;
            break;

        default:
            config |= ACCEL_CONFIG_0_T::ACCEL_ODR::LN_1_6_kHz;
            break;
    }
    return config;
}

uint8_t ICM42670P::accel_map_lpf_bw(uint8_t lpf_bw) {
    uint8_t config = 0;

    switch (lpf_bw) {
        case 0:
            config |= ACCEL_CONFIG1_T::ACCEL_UI_FILT_BW::LPF_BW_BYPASSED;
            break;
        case 180:
            config |= ACCEL_CONFIG1_T::ACCEL_UI_FILT_BW::LPF_BW_180_HZ;
            break;
        case 121:
            config |= ACCEL_CONFIG1_T::ACCEL_UI_FILT_BW::LPF_BW_121_HZ;
            break;
        case 73:
            config |= ACCEL_CONFIG1_T::ACCEL_UI_FILT_BW::LPF_BW_73_HZ;
            break;
        case 53:
            config |= ACCEL_CONFIG1_T::ACCEL_UI_FILT_BW::LPF_BW_53_HZ;
            break;
        case 34:
            config |= ACCEL_CONFIG1_T::ACCEL_UI_FILT_BW::LPF_BW_34_HZ;
            break;
        case 25:
            config |= ACCEL_CONFIG1_T::ACCEL_UI_FILT_BW::LPF_BW_25_HZ;
            break;
        case 16:
            config |= ACCEL_CONFIG1_T::ACCEL_UI_FILT_BW::LPF_BW_16_HZ;
            break;
        default:
            config |= ACCEL_CONFIG1_T::ACCEL_UI_FILT_BW::LPF_BW_BYPASSED;
            break;
    }
    return config;
}

uint8_t ICM42670P::accel_map_av_filter(uint8_t av_filter) {
    uint8_t config = 0;

    switch (av_filter) {
        case 2:
            config |= ACCEL_CONFIG1_T::ACCEL_UI_AVG::AVG_2X;
        case 4:
            break;
            config |= ACCEL_CONFIG1_T::ACCEL_UI_AVG::AVG_4X;
        case 8:
            break;
            config |= ACCEL_CONFIG1_T::ACCEL_UI_AVG::AVG_8X;
        case 16:
            break;
            config |= ACCEL_CONFIG1_T::ACCEL_UI_AVG::AVG_16X;
        case 32:
            break;
            config |= ACCEL_CONFIG1_T::ACCEL_UI_AVG::AVG_32X;
        case 64:
            config |= ACCEL_CONFIG1_T::ACCEL_UI_AVG::AVG_64X;
            break;
        default:
            config |= ACCEL_CONFIG1_T::ACCEL_UI_AVG::AVG_16X;
    }
    return config;
}

bool ICM42670P::accel_setting(uint16_t fsr, uint16_t odr, uint8_t av_filter, uint8_t lpf_bw) {
    bool valid_accel_settings = validate_accel_settings(odr, av_filter);

    // clang-format off
    uint8_t config_0 = accel_map_fsr(fsr) |
                       accel_map_odr(odr);
                    
    uint8_t config_1 =  accel_map_av_filter(av_filter) | 
                        accel_map_lpf_bw(lpf_bw);
    // clang-format on

    // if the accel settings not valid
    // Turn off the accelerometer before writting the other settings.
    if (!valid_accel_settings) {
        uint8_t pwr_config = 0;
        spi_read(user_bank_0_register_t::PWR_MGMT0, &pwr_config, 1);

        pwr_config |= PWR_MGMT0_T::ACCEL_MODE::ACCEL_OFF;
        spi_write(user_bank_0_register_t::PWR_MGMT0, &pwr_config, 1);
        delay(50);
    }

    spi_write(user_bank_0_register_t::ACCEL_CONFIG0, &config_0, 1);
    spi_write(user_bank_0_register_t::ACCEL_CONFIG1, &config_1, 1);

    if (!valid_accel_settings) {
        uint8_t pwr_config = 0;
        spi_read(user_bank_0_register_t::PWR_MGMT0, &pwr_config, 1);

        pwr_config |= PWR_MGMT0_T::ACCEL_MODE::ACCEL_LOW_POWER;
        spi_write(user_bank_0_register_t::PWR_MGMT0, &pwr_config, 1);
    }

    return 0;
}

bool ICM42670P::gyro_setting(uint16_t fsr, uint16_t odr, uint8_t lpf_bw) {
    uint8_t gyro_config0 = gyro_map_odr(odr) | gyro_map_fsr(fsr);
    spi_write(user_bank_0_register_t::GYRO_CONFIG0, &gyro_config0, 1);

    uint8_t gyro_config1 = gyro_map_lpf_bw(lpf_bw);
    spi_write(user_bank_0_register_t::GYRO_CONFIG0, &gyro_config1, 1);

    uint8_t pwr_config = 0;
    spi_read(user_bank_0_register_t::PWR_MGMT0, &pwr_config, 1);

    // check if the gyro is on
    if ((pwr_config & PWR_MGMT0_T::GYRO_MODE::GYRO_LOWNOISE) == 0) {
        // Gyro off. Turn it on
        uint8_t gyro_config = pwr_config | PWR_MGMT0_T::GYRO_MODE::GYRO_LOWNOISE;

        spi_write(user_bank_0_register_t::PWR_MGMT0, &gyro_config, 1);

        delay(50);
    }
    return true;
}

uint8_t ICM42670P::gyro_map_odr(uint16_t odr) {
    uint8_t config = 0;

    switch (odr) {
        case 1600:
            config |= GYRO_CONFIG_0_T::GYRO_ODR::ODR_1600HZ;

            break;
        case 800:
            config |= GYRO_CONFIG_0_T::GYRO_ODR::ODR_800HZ;

            break;
        case 400:
            config |= GYRO_CONFIG_0_T::GYRO_ODR::ODR_400HZ;

            break;
        case 200:
            config |= GYRO_CONFIG_0_T::GYRO_ODR::ODR_200HZ;

            break;
        case 100:
            config |= GYRO_CONFIG_0_T::GYRO_ODR::ODR_100HZ;

            break;
        case 50:
            config |= GYRO_CONFIG_0_T::GYRO_ODR::ODR_50HZ;

            break;
        case 25:
            config |= GYRO_CONFIG_0_T::GYRO_ODR::ODR_25HZ;

            break;
        case 12:
            config |= GYRO_CONFIG_0_T::GYRO_ODR::ODR_12_5HZ;
            break;
    }
    return config;
}

uint8_t ICM42670P::gyro_map_fsr(uint16_t fsr) {
    uint8_t config = 0;

    switch (fsr) {
        case 2000:
            config |= GYRO_CONFIG_0_T::GYRO_UI_FS_SEL::DPS_2000;
            reading_2_dps = float(fsr) / float(32767);
            break;

        case 1000:
            config |= GYRO_CONFIG_0_T::GYRO_UI_FS_SEL::DPS_1000;
            reading_2_dps = float(fsr) / float(32767);
            break;

        case 500:
            config |= GYRO_CONFIG_0_T::GYRO_UI_FS_SEL::DPS_500;
            reading_2_dps = float(fsr) / float(32767);
            break;

        case 250:
            config |= GYRO_CONFIG_0_T::GYRO_UI_FS_SEL::DPS_250;
            reading_2_dps = float(fsr) / float(32767);
            break;
        default:
            config |= GYRO_CONFIG_0_T::GYRO_UI_FS_SEL::DPS_250;
            reading_2_dps = float(250.0) / float(32767);
            break;
    }

    return config;
}

uint8_t ICM42670P::gyro_map_lpf_bw(uint8_t lpf_bw) {
    uint8_t config = 0;

    switch (lpf_bw) {
        case 0:
            config |= GYRO_CONFIG1_T::GYRO_UI_FILT_BW::LPF_BW_BYPASSED;
            break;
        case 180:
            config |= GYRO_CONFIG1_T::GYRO_UI_FILT_BW::LPF_BW_180_HZ;
            break;
        case 121:
            config |= GYRO_CONFIG1_T::GYRO_UI_FILT_BW::LPF_BW_121_HZ;
            break;
        case 73:
            config |= GYRO_CONFIG1_T::GYRO_UI_FILT_BW::LPF_BW_73_HZ;
            break;
        case 53:
            config |= GYRO_CONFIG1_T::GYRO_UI_FILT_BW::LPF_BW_53_HZ;
            break;
        case 34:
            config |= GYRO_CONFIG1_T::GYRO_UI_FILT_BW::LPF_BW_34_HZ;
            break;
        case 25:
            config |= GYRO_CONFIG1_T::GYRO_UI_FILT_BW::LPF_BW_25_HZ;
            break;
        case 16:
            config |= GYRO_CONFIG1_T::GYRO_UI_FILT_BW::LPF_BW_16_HZ;
            break;
        default:
            config |= GYRO_CONFIG1_T::GYRO_UI_FILT_BW::LPF_BW_BYPASSED;
            break;
    }
    return config;
}

bool ICM42670P::validate_accel_settings(float odr, uint8_t av_filter) {
    bool valid = true;

    uint8_t pwr_mgmt_config = 0;

    spi_read(user_bank_0_register_t::PWR_MGMT0, &pwr_mgmt_config, 1);

    // check accel mode given by bit 1:0 of register PWR_MGMT0
    uint8_t accel_mode = (pwr_mgmt_config & 0b00000011);

    if (accel_mode == PWR_MGMT0_T::ACCEL_MODE::ACCEL_LOW_POWER) {
        if (odr == 1600 or odr == 800) {
            valid = false;
        }

        else if (odr == 400 and (av_filter == 16 or av_filter == 32 or av_filter == 64)) {
            valid = false;

        }

        else if (odr == 200 and av_filter == 64) {
            valid = false;
        }
    }

    return valid;
}

void ICM42670P::user_bank_0_read(uint8_t reg, uint8_t *read_buffer, uint32_t len) { spi_read(reg, read_buffer, len); }
void ICM42670P::user_bank_0_write(uint8_t reg, const uint8_t *write_buffer, uint32_t len) { spi_write(reg, write_buffer, len); }