#ifndef ICM42670_H
#define ICM42670_H

#include <Arduino.h>
#include <SPI.h>
//------------------------------------------Registers--------------------------------------
typedef enum {
    MCLK_RDY = 0x00,
    DEVICE_CONFIG = 0x01,
    SIGNAL_PATH_RESET = 0x02,
    DRIVE_CONFIG1 = 0x03,
    DRIVE_CONFIG2 = 0x04,
    DRIVE_CONFIG3 = 0x05,
    INT_CONFIG = 0x06,
    TEMP_DATA1 = 0x09,
    TEMP_DATA0 = 0x0a,
    ACCEL_DATA_X1 = 0x0b,
    ACCEL_DATA_X0 = 0x0c,
    ACCEL_DATA_Y1 = 0x0d,
    ACCEL_DATA_Y0 = 0x0e,
    ACCEL_DATA_Z1 = 0x0f,
    ACCEL_DATA_Z0 = 0x10,
    GYRO_DATA_X1 = 0x11,
    GYRO_DATA_X0 = 0x12,
    GYRO_DATA_Y1 = 0x13,
    GYRO_DATA_Y0 = 0x14,
    GYRO_DATA_Z1 = 0x15,
    GYRO_DATA_Z0 = 0x16,
    TMST_FSYNCH = 0x17,
    TMST_FSYNCL = 0x18,
    APEX_DATA4 = 0x1d,
    APEX_DATA5 = 0x1e,
    PWR_MGMT0 = 0x1f,
    GYRO_CONFIG0 = 0x20,
    ACCEL_CONFIG0 = 0x21,
    TEMP_CONFIG0 = 0x22,
    GYRO_CONFIG1 = 0x23,
    ACCEL_CONFIG1 = 0x24,
    APEX_CONFIG0 = 0x25,
    APEX_CONFIG1 = 0x26,
    WOM_CONFIG = 0x27,
    FIFO_CONFIG1 = 0x28,
    FIFO_CONFIG2 = 0x29,
    FIFO_CONFIG3 = 0x2a,
    INT_SOURCE0 = 0x2b,
    INT_SOURCE1 = 0x2c,
    INT_SOURCE3 = 0x2d,
    INT_SOURCE4 = 0x2e,
    FIFO_LOST_PKT0 = 0x2f,
    FIFO_LOST_PKT1 = 0x30,
    APEX_DATA0 = 0x31,
    APEX_DATA1 = 0x32,
    APEX_DATA2 = 0x33,
    APEX_DATA3 = 0x34,
    INTF_CONFIG0 = 0x35,
    INTF_CONFIG1 = 0x36,
    INT_STATUS_DRDY = 0x39,
    INT_STATUS = 0x3a,
    INT_STATUS2 = 0x3b,
    INT_STATUS3 = 0x3c,
    FIFO_COUNTH = 0x3d,
    FIFO_COUNTL = 0x3e,
    FIFO_DATA = 0x3f,
    WHO_AM_I = 0x75,
    BLK_SEL_W = 0x79,
    MADDR_W = 0x7a,
    M_W = 0x7b,
    BLK_SEL_R = 0x7c,
    MADDR_R = 0x7d,
    M_R = 0x7e,
} user_bank_0_register_t;

typedef enum {
    /* MREG1 */
    TMST_CONFIG1_MREG1 = 0x00,
    FIFO_CONFIG5_MREG1 = 0x01,
    FIFO_CONFIG6_MREG1 = 0x02,
    FSYNC_CONFIG_MREG1 = 0x03,
    INT_CONFIG0_MREG1 = 0x04,
    INT_CONFIG1_MREG1 = 0x05,
    SENSOR_CONFIG3_MREG1 = 0x06,
    ST_CONFIG_MREG1 = 0x13,
    SELFTEST_MREG1 = 0x14,
    INTF_CONFIG6_MREG1 = 0x23,
    INTF_CONFIG10_MREG1 = 0x25,
    INTF_CONFIG7_MREG1 = 0x28,
    OTP_CONFIG_MREG1 = 0x2b,
    INT_SOURCE6_MREG1 = 0x2f,
    INT_SOURCE7_MREG1 = 0x30,
    INT_SOURCE8_MREG1 = 0x31,
    INT_SOURCE9_MREG1 = 0x32,
    INT_SOURCE10_MREG1 = 0x33,
    APEX_CONFIG2_MREG1 = 0x44,
    APEX_CONFIG3_MREG1 = 0x45,
    APEX_CONFIG4_MREG1 = 0x46,
    APEX_CONFIG5_MREG1 = 0x47,
    APEX_CONFIG9_MREG1 = 0x48,
    APEX_CONFIG10_MREG1 = 0x49,
    APEX_CONFIG11_MREG1 = 0x4a,
    ACCEL_WOM_X_THR_MREG1 = 0x4b,
    ACCEL_WOM_Y_THR_MREG1 = 0x4c,
    ACCEL_WOM_Z_THR_MREG1 = 0x4d,
    OFFSET_USER0_MREG1 = 0x4e,
    OFFSET_USER1_MREG1 = 0x4f,
    OFFSET_USER2_MREG1 = 0x50,
    OFFSET_USER3_MREG1 = 0x51,
    OFFSET_USER4_MREG1 = 0x52,
    OFFSET_USER5_MREG1 = 0x53,
    OFFSET_USER6_MREG1 = 0x54,
    OFFSET_USER7_MREG1 = 0x55,
    OFFSET_USER8_MREG1 = 0x56,
    ST_STATUS1_MREG1 = 0x63,
    ST_STATUS2_MREG1 = 0x64,
    FDR_CONFIG_MREG1 = 0x66,
    APEX_CONFIG12_MREG1 = 0x67,
} user_bank_mreg_1_register_t;

typedef enum { OTP_CTRL7_MREG2 = 0x2806 } user_bank_mreg_2_register_t;

typedef enum {
    XA_ST_DATA_MREG3 = 0x5000,
    YA_ST_DATA_MREG3 = 0x5001,
    ZA_ST_DATA_MREG3 = 0x5002,
    XG_ST_DATA_MREG3 = 0x5003,
    YG_ST_DATA_MREG3 = 0x5004,
    ZG_ST_DATA_MREG3 = 0x5005,
} user_bank_mreg_3_register_t;
//-----------------------------------------------------------------------------------------

//--------------------------------PWR_MGMT0-------------------
// clang-format off
typedef struct{
    
    typedef enum{
        // MASK = 0x02,
        GYRO_OFF = 0b00 << 2, 
        GYRO_STANDBY = 0b01 << 2, 
        GYRO_LOWNOISE = 0b11 << 2 
    }GYRO_MODE;
    // clang-format on

    // clang-format off
    typedef  enum{
        // MASK = 0x07,
        WAKEUP_OSCI = 0b1 << 7, 
        WAKEUP_RC = 0b1 << 7, 
    }ACCEL_LP_CLK_SEL;
    // clang-format on

    // clang-format off
    typedef  enum{
        // MASK = 0x00,
        ACCEL_OFF = 0b00, 
        ACCEL_LOW_POWER = 0b10, 
        ACCEL_LOW_NOISE = 0b11 
    }ACCEL_MODE;

}PWR_MGMT0_T;
// clang-format on

//--------------------------------GYRO_CONFIG0-------------------
// clang-format off
typedef struct{    
    typedef  enum{
        // MASK = 0x05,
        DPS_2000 = 0b00 << 5, 
        DPS_1000 = 0b01 << 5,
        DPS_500 = 0b10 << 5,
        DPS_250 = 0b11 << 5
    }GYRO_UI_FS_SEL;

    typedef  enum{
        // MASK = 0x00 << 5,
        ODR_1600HZ = 0b0101,
        ODR_800HZ = 0b0110,
        ODR_400HZ = 0b0111,
        ODR_200HZ = 0b1000,
        ODR_100HZ = 0b1001,
        ODR_50HZ = 0b1010,
        ODR_25HZ = 0b1011,
        ODR_12_5HZ = 0b1110,
    }GYRO_ODR;
}GYRO_CONFIG_0_T;



//------------------------ACCEL_CONFIG0----------------------------
typedef struct{
    typedef  enum {
        // MASK =  0x05,
        G_16 =  0b00 << 5,
        G_8  =  0b01 << 5,
        G_4  =  0b10 << 5,
        G_2  =  0b11 << 5
    }ACCEL_UI_FS_SEL;

    typedef  enum {
        // MASK =0x00,
        LN_1_6_kHz = 0b0101,
        LN_800_Hz = 0b0110, 
        LP_LN_400_Hz = 0b0111, 
        LP_LN_200_Hz = 0b1000, 
        LP_LN_100_Hz = 0b1001, 
        LP_LN_50_Hz = 0b1010, 
        LP_LN_25_Hz = 0b1011, 
        LP_LN_12_5_Hz = 0b1100,
        LP_6_25_Hz = 0b1101,
        LP_3_125_Hz = 0b1110,
        LP_1_5625_Hz = 0b1111
    }ACCEL_ODR;
}ACCEL_CONFIG_0_T;    



//-----------------------TEMP_CONFG0-------------------------------
typedef struct{
    typedef  enum  {
        // MASK= 0x04,
        DLPF_BYPASSED = 0b000 << 4,
        DLPF_BW_180_HZ = 0b001 << 4,
        DLPF_BW_72_HZ = 0b010 << 4,
        DLPF_BW_34_HZ = 0b011 << 4,
        DLPF_BW_16_HZ = 0b100 << 4,
        DLPF_BW_8_HZ = 0b101 << 4,
        DLPF_BW_4_HZ = 0b110 << 4
        // DLPF_BW_4_HZ = 0b111 << 4
    }TEMP_FILT_BW;
}TEMP_CONFIG0_T;

//-----------------GYRO_CONFIG1--------------------------------------
typedef struct{
    typedef  enum {
        LPF_BW_BYPASSED = 0b000,
        LPF_BW_180_HZ = 0b001,
        LPF_BW_121_HZ = 0b010,
        LPF_BW_73_HZ = 0b011,
        LPF_BW_53_HZ = 0b100,
        LPF_BW_34_HZ = 0b101,
        LPF_BW_25_HZ = 0b110,
        LPF_BW_16_HZ = 0b111
    }GYRO_UI_FILT_BW;

}GYRO_CONFIG1_T;

//----------------------ACCEL_CONFIG1---------------------------------------------
typedef struct {
    typedef  enum  {
        AVG_2X = (0b000 << 4), 
        AVG_4X = (0b001 << 4), 
        AVG_8X = (0b010 << 4), 
        AVG_16X = (0b011 << 4), 
        AVG_32X = (0b100 << 4), 
        AVG_64X = (0b101 << 4), 
    }ACCEL_UI_AVG;

    typedef  enum {
        LPF_BW_BYPASSED = (0b000<<0), 
        LPF_BW_180_HZ = (0b001<<0), 
        LPF_BW_121_HZ = (0b010<<0), 
        LPF_BW_73_HZ = (0b011<<0), 
        LPF_BW_53_HZ = (0b100<<0), 
        LPF_BW_34_HZ = (0b101<<0), 
        LPF_BW_25_HZ = (0b110<<0), 
        LPF_BW_16_HZ = (0b111<<0) 
    }ACCEL_UI_FILT_BW;

}ACCEL_CONFIG1_T;
// clang-format on

typedef struct {
    uint16_t xl[3];
    uint16_t gy[3];
    float temp;

} imu_raw_data_t;

typedef struct {
    float xl[3];
    float gy[3];
    float temp;

} imu_data_t;

class ICM42670P {
   public:
    ICM42670P(SPIClass &_spi, uint8_t _cs);
    ~ICM42670P();
    int8_t begin();
    bool get_imu_data(imu_data_t &data);
    bool get_imu_raw_data(imu_raw_data_t &data);

    void user_bank_0_read(uint8_t reg, uint8_t *read_buffer, uint32_t len);
    void user_bank_0_write(uint8_t reg, const uint8_t *write_buffer, uint32_t len);

    void user_bank_mreg_0_read();
    void user_bank_mreg_0_write();

    void user_bank_mreg_1_read();
    void user_bank_mreg_1_write();

    bool accel_setting(uint16_t fsr = 2, uint16_t odr = 1600, uint8_t av_filter = 16, uint8_t lpf_bw = 0);
    bool gyro_setting(uint16_t fsr = 250, uint16_t odr = 1600, uint8_t lpf_bw = 0);

    bool temp_filter_settings(uint8_t lpf_bw = 0);

   private:
    uint8_t imu_cs;
    SPIClass *imu_spi;
    uint32_t spi_clock;
    float reading_2_g;
    float reading_2_dps;

    void spi_read(uint8_t reg, uint8_t *read_buffer, uint32_t len);
    void spi_write(uint8_t reg, const uint8_t *write_buffer, uint32_t len);

    uint8_t accel_map_fsr(uint16_t fsr);
    uint8_t accel_map_odr(uint16_t odr);

    uint8_t accel_map_lpf_bw(uint8_t lpf_bw);
    uint8_t accel_map_av_filter(uint8_t av_filter);

    uint8_t gyro_map_odr(uint16_t odr);
    uint8_t gyro_map_fsr(uint16_t fsr);

    uint8_t gyro_map_lpf_bw(uint8_t lpf_bw);

    bool validate_accel_settings(float odr, uint8_t av_filter);

    float unsigned_val_2_float(uint16_t);
};
#endif
