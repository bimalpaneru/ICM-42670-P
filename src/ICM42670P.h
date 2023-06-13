#ifndef ICM42670_H
#define ICM42670_H

#include <Arduino.h>
#include <SPI.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const float ACCEL_FULL_SACLE_READING = 32767.0;
const float GYRO_FULL_SACLE_READING = 32767.0;
const uint8_t DEBUG_LEVEL0 = 0;
const uint8_t DEBUG_LEVEL1 = 1;
const uint8_t DEBUG_LEVEL2 = 2;
const float THRESHOLD_SENSITIVITY_MG_TO_BYTE = 256.0 / 1000.0;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------Registers--------------------------------------

// clang-format off

typedef enum{
    USER_BANK_MREG_1 = 0x00,
    USER_BANK_MREG_2 = 0x28,
    USER_BANK_MREG_3 = 0x50
}register_banks_t;

typedef enum {
    MCLK_RDY          = 0x00,
    DEVICE_CONFIG     = 0x01,
    SIGNAL_PATH_RESET = 0x02,
    DRIVE_CONFIG1     = 0x03,
    DRIVE_CONFIG2     = 0x04,
    DRIVE_CONFIG3     = 0x05,
    INT_CONFIG        = 0x06,
    TEMP_DATA1        = 0x09,
    TEMP_DATA0        = 0x0a,
    ACCEL_DATA_X1     = 0x0b,
    ACCEL_DATA_X0     = 0x0c,
    ACCEL_DATA_Y1     = 0x0d,
    ACCEL_DATA_Y0     = 0x0e,
    ACCEL_DATA_Z1     = 0x0f,
    ACCEL_DATA_Z0     = 0x10,
    GYRO_DATA_X1      = 0x11,
    GYRO_DATA_X0      = 0x12,
    GYRO_DATA_Y1      = 0x13,
    GYRO_DATA_Y0      = 0x14,
    GYRO_DATA_Z1      = 0x15,
    GYRO_DATA_Z0      = 0x16,
    TMST_FSYNCH       = 0x17,
    TMST_FSYNCL       = 0x18,
    APEX_DATA4        = 0x1d,
    APEX_DATA5        = 0x1e,
    PWR_MGMT0         = 0x1f,
    GYRO_CONFIG0      = 0x20,
    ACCEL_CONFIG0     = 0x21,
    TEMP_CONFIG0      = 0x22,
    GYRO_CONFIG1      = 0x23,
    ACCEL_CONFIG1     = 0x24,
    APEX_CONFIG0      = 0x25,
    APEX_CONFIG1      = 0x26,
    WOM_CONFIG        = 0x27,
    FIFO_CONFIG1      = 0x28,
    FIFO_CONFIG2      = 0x29,
    FIFO_CONFIG3      = 0x2a,
    INT_SOURCE0       = 0x2b,
    INT_SOURCE1       = 0x2c,
    INT_SOURCE3       = 0x2d,
    INT_SOURCE4       = 0x2e,
    FIFO_LOST_PKT0    = 0x2f,
    FIFO_LOST_PKT1    = 0x30,
    APEX_DATA0        = 0x31,
    APEX_DATA1        = 0x32,
    APEX_DATA2        = 0x33,
    APEX_DATA3        = 0x34,
    INTF_CONFIG0      = 0x35,
    INTF_CONFIG1      = 0x36,
    INT_STATUS_DRDY   = 0x39,
    INT_STATUS        = 0x3a,
    INT_STATUS2       = 0x3b,
    INT_STATUS3       = 0x3c,
    FIFO_COUNTH       = 0x3d,
    FIFO_COUNTL       = 0x3e,
    FIFO_DATA         = 0x3f,
    WHO_AM_I          = 0x75,
    BLK_SEL_W         = 0x79,
    MADDR_W           = 0x7a,
    M_W               = 0x7b,
    BLK_SEL_R         = 0x7c,
    MADDR_R           = 0x7d,
    M_R               = 0x7e,
} user_bank_0_register_t;

typedef enum {
    /* MREG1 */
    TMST_CONFIG1_MREG1    = 0x00,
    FIFO_CONFIG5_MREG1    = 0x01,
    FIFO_CONFIG6_MREG1    = 0x02,
    FSYNC_CONFIG_MREG1    = 0x03,
    INT_CONFIG0_MREG1     = 0x04,
    INT_CONFIG1_MREG1     = 0x05,
    SENSOR_CONFIG3_MREG1  = 0x06,
    ST_CONFIG_MREG1       = 0x13,
    SELFTEST_MREG1        = 0x14,
    INTF_CONFIG6_MREG1    = 0x23,
    INTF_CONFIG10_MREG1   = 0x25,
    INTF_CONFIG7_MREG1    = 0x28,
    OTP_CONFIG_MREG1      = 0x2b,
    INT_SOURCE6_MREG1     = 0x2f,
    INT_SOURCE7_MREG1     = 0x30,
    INT_SOURCE8_MREG1     = 0x31,
    INT_SOURCE9_MREG1     = 0x32,
    INT_SOURCE10_MREG1    = 0x33,
    APEX_CONFIG2_MREG1    = 0x44,
    APEX_CONFIG3_MREG1    = 0x45,
    APEX_CONFIG4_MREG1    = 0x46,
    APEX_CONFIG5_MREG1    = 0x47,
    APEX_CONFIG9_MREG1    = 0x48,
    APEX_CONFIG10_MREG1   = 0x49,
    APEX_CONFIG11_MREG1   = 0x4a,
    ACCEL_WOM_X_THR_MREG1 = 0x4b,
    ACCEL_WOM_Y_THR_MREG1 = 0x4c,
    ACCEL_WOM_Z_THR_MREG1 = 0x4d,
    OFFSET_USER0_MREG1    = 0x4e,
    OFFSET_USER1_MREG1    = 0x4f,
    OFFSET_USER2_MREG1    = 0x50,
    OFFSET_USER3_MREG1    = 0x51,
    OFFSET_USER4_MREG1    = 0x52,
    OFFSET_USER5_MREG1    = 0x53,
    OFFSET_USER6_MREG1    = 0x54,
    OFFSET_USER7_MREG1    = 0x55,
    OFFSET_USER8_MREG1    = 0x56,
    ST_STATUS1_MREG1      = 0x63,
    ST_STATUS2_MREG1      = 0x64,
    FDR_CONFIG_MREG1      = 0x66,
    APEX_CONFIG12_MREG1   = 0x67,
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


/*--------------------------------------------DEVICE_CONFIG--------------------------------------------*/
typedef enum{
    SPI_AP_3WIRE = 0b0 << 2,
    SPI_AP_4WIRE = 0b1 << 2

}DEVICE_CONFIG_SPI_AP_4WIRE_t;
typedef enum{
    SPI_MODE0_AND_MODE3 = 0b0 << 0,
    SPI_MODE1_AND_MODE2 = 0b1 << 0
}DEVICE_CONFIG_SPI_MODE_t;
/*----------------------------------------------------------------------------------------*/

/*--------------------------------------------SIGNAL_PATH_RESET--------------------------------------------*/

typedef enum{
    SOFTWARE_RESET_ENABLE   = 0b0 << 4,
    SOFTWARE_RESET_DISBALE  = 0b1 << 4
}SIGNAL_PATH_RESET_SOFT_RESET_DEVICE_CONFIG_t;

typedef enum{
    FIFO_FLUSH_READ  = 0b0 << 2,
    FIFO_FLUSH       = 0b1 << 2
}SIGNAL_PATH_RESET_FIFO_FLUSH_t;
/*----------------------------------------------------------------------------------------*/

/*--------------------------------------------DRIVE_CONFIG1--------------------------------------------*/
typedef enum{
    I3C_DDR_SLEW_RATE_MIN_20NS_MAX_60NS = 0b000 << 3,
    I3C_DDR_SLEW_RATE_MIN_12NS_MAX_36NS = 0b001 << 3,
    I3C_DDR_SLEW_RATE_MIN_6NS_MAX_19NS  = 0b010 << 3,
    I3C_DDR_SLEW_RATE_MIN_4NS_MAX_14NS  = 0b011 << 3,
    I3C_DDR_SLEW_RATE_MIN_2NS_MAX_8NS   = 0b100 << 3,
    I3C_DDR_SLEW_RATE_MAX_60NS          = 0b101 << 3
    // RESERVED = 0b110 << 5,
    // RESERVED = 0b111 << 5,
}DRIVE_CONFIG1_I3C_DDR_SLEW_RATE_t;
typedef enum{
    I3C_SDR_SLEW_RATE_MIN_20NS_MAX_60NS = 0b000 << 0,
    I3C_SDR_SLEW_RATE_MIN_12NS_MAX_36NS = 0b001 << 0,
    I3C_SDR_SLEW_RATE_MIN_6NS_MAX_19NS  = 0b010 << 0,
    I3C_SDR_SLEW_RATE_MIN_4NS_MAX_14NS  = 0b011 << 0,
    I3C_SDR_SLEW_RATE_MIN_2NS_MAX_8NS   = 0b100 << 0,
    I3C_SDR_SLEW_RATE_MAX_60NS          = 0b101 << 0
    // RESERVED = 0b110 << 5,
    // RESERVED = 0b111 << 5,
}DRIVE_CONFIG1_I3C_SDR_SLEW_RATE_t;
/*----------------------------------------------------------------------------------------*/

/*--------------------------------------------DRIVE_CONFIG2--------------------------------------------*/
typedef enum{
    I2C_SLEW_RATE_MIN_20NS_MAX_60NS = 0b000 << 3,
    I2C_SLEW_RATE_MIN_12NS_MAX_36NS = 0b001 << 3,
    I2C_SLEW_RATE_MIN_6NS_MAX_19NS  = 0b010 << 3,
    I2C_SLEW_RATE_MIN_4NS_MAX_14NS  = 0b011 << 3,
    I2C_SLEW_RATE_MIN_2NS_MAX_8NS   = 0b100 << 3,
    I2C_SLEW_RATE_MAX_60NS          = 0b101 << 3
    // RESERVED = 0b110 << 5,
    // RESERVED = 0b111 << 5,
}DRIVE_CONFIG2_I2C_SLEW_RATE_t;

typedef enum{
    ALL_SLEW_RATE_MIN_20NS_MAX_60NS = 0b000 << 0,
    ALL_SLEW_RATE_MIN_12NS_MAX_36NS = 0b001 << 0,
    ALL_SLEW_RATE_MIN_6NS_MAX_19NS  = 0b010 << 0,
    ALL_SLEW_RATE_MIN_4NS_MAX_14NS  = 0b011 << 0,
    ALL_SLEW_RATE_MIN_2NS_MAX_8NS   = 0b100 << 0,
    ALL_SLEW_RATE_MAX_60NS          = 0b101 << 0
    // RESERVED = 0b110 << 5,
    // RESERVED = 0b111 << 5,
}DRIVE_CONFIG2_ALL_SLEW_RATE_t;
/*----------------------------------------------------------------------------------------*/

/*--------------------------------------------DRIVE_CONFIG3--------------------------------------------*/
typedef enum{
    SPI_SLEW_RATE_MIN_20NS_MAX_60NS = 0b000 << 0,
    SPI_SLEW_RATE_MIN_12NS_MAX_36NS = 0b001 << 0,
    SPI_SLEW_RATE_MIN_6NS_MAX_19NS  = 0b010 << 0,
    SPI_SLEW_RATE_MIN_4NS_MAX_14NS  = 0b011 << 0,
    SPI_SLEW_RATE_MIN_2NS_MAX_8NS   = 0b100 << 0,
    SPI_SLEW_RATE_MAX_60NS          = 0b101 << 0
    // RESERVED = 0b110 << 5,
    // RESERVED = 0b111 << 5,
}DRIVE_CONFIG3_SPI_SLEW_RATE_t;
/*----------------------------------------------------------------------------------------*/

/*--------------------------------------------INT_CONFIG--------------------------------------------*/
typedef enum{
    INT2_MODE_PULSED  = 0b0 << 5,
    INT2_MODE_LATCHED = 0b1 << 5
}INT_CONFIG_INT2_MODE_t;

typedef enum{
    INT2_DRIVE_CIRCUIT_OPEN_DRAIN = 0b0 << 4,
    INT2_DRIVE_CIRCUIT_PUSH_PULL  = 0b1 << 4
}INT_CONFIG_INT2_DRIVE_CIRCUIT_t;

typedef enum{
    INT2_POLARITY_ACTIVE_LOW   = 0b0 << 3,
    INT2_POLARITY_ACTIVE_HIGH  = 0b1 << 3
}INT_CONFIG_INT2_POLARITY_t;

typedef enum{
    INT1_MODE_PULSED  = 0b0 << 2,
    INT1_MODE_LATCHED = 0b1 << 2
}INT_CONFIG_INT1_MODE_t;

typedef enum{
    INT1_DRIVE_OPEN_DRAIN = 0b0 << 1,
    INT1_DRIVE_PUSH_PULL  = 0b1 << 1
}INT_CONFIG_INT1_DRIVE_CIRCUIT_t;

typedef enum{
    INT1_POLARITY_ACTIVE_LOW   = 0b0,
    INT1_POLARITY_ACTIVE_HIGH  = 0b1
}INT_CONFIG_INT1_POLARITY_t;
/*----------------------------------------------------------------------------------------*/


/*----------------------------------PWR_MGMT0----------------------------------*/
typedef enum {
    // MASK = 0x07,
    ACCEL_LP_CLK_SEL_WAKEUP_OSCI     = 0b1 << 7,
    ACCEL_LP_CLK_SEL_WAKEUP_RC       = 0b1 << 7
} PWR_MGMT0_ACCEL_LP_CLK_SEL_t;

typedef enum {
    IDLE_RC_OSC_OFF =  0b0 << 4,
    IDLE_RC_OSC_ON  =  0b1 << 4
} PWR_MGMT0_IDLE_t;


typedef enum {
    // MASK = 0x02,
    GYRO_OFF        = 0b00 << 2,
    GYRO_STANDBY    = 0b01 << 2,
    GYRO_LOWNOISE   = 0b11 << 2
} PWR_MGMT0_GYRO_t;

typedef enum {
    // MASK = 0x00,
    ACCEL_MODE_ACCEL_OFF       = 0b00,
    ACCEL_MODE_ACCEL_LOW_POWER = 0b10,
    ACCEL_MODE_ACCEL_LOW_NOISE = 0b11
} PWR_MGMT0_ACCEL_MODE_t;

/*--------------------------------------------------------------------*/

/*----------------------------------GYRO_CONFIG0----------------------------------*/

typedef  enum{
    // MASK = 0x05,
    GYRO_UI_FS_DPS_2000 = 0b00 << 5, 
    GYRO_UI_FS_DPS_1000 = 0b01 << 5,
    GYRO_UI_FS_DPS_500  = 0b10 << 5,
    GYRO_UI_FS_DPS_250  = 0b11 << 5
}GYRO_CONFIG0_GYRO_UI_FS_SEL_t;

typedef  enum{
    // MASK = 0x00 << 5,
    GYRO_ODR_ODR_1600HZ  = 0b0101,
    GYRO_ODR_ODR_800HZ   = 0b0110,
    GYRO_ODR_ODR_400HZ   = 0b0111,
    GYRO_ODR_ODR_200HZ   = 0b1000,
    GYRO_ODR_ODR_100HZ   = 0b1001,
    GYRO_ODR_ODR_50HZ    = 0b1010,
    GYRO_ODR_ODR_25HZ    = 0b1011,
    GYRO_ODR_ODR_12_5HZ  = 0b1110,
}GYRO_CONFIG0_GYRO_ODR_t;
/*----------------------------------------------------------------------------*/



/*----------------------------------ACCEL_CONFIG0----------------------------------*/
typedef  enum {
    // MASK =  0x05,
    ACCEL_UI_FS_G_16 =  0b00 << 5,
    ACCEL_UI_FS_G_8  =  0b01 << 5,
    ACCEL_UI_FS_G_4  =  0b10 << 5,
    ACCEL_UI_FS_G_2  =  0b11 << 5
}ACCEL_CONFIG0_ACCEL_UI_FS_SEL_t;

typedef  enum {
    // MASK =0x00,
    ACCEL_ODR_LN_1_6_kHz    = 0b0101,
    ACCEL_ODR_LN_800_Hz     = 0b0110, 
    ACCEL_ODR_LP_LN_400_Hz  = 0b0111, 
    ACCEL_ODR_LP_LN_200_Hz  = 0b1000, 
    ACCEL_ODR_LP_LN_100_Hz  = 0b1001, 
    ACCEL_ODR_LP_LN_50_Hz   = 0b1010, 
    ACCEL_ODR_LP_LN_25_Hz   = 0b1011, 
    ACCEL_ODR_LP_LN_12_5_Hz = 0b1100,
    ACCEL_ODR_LP_6_25_Hz    = 0b1101,
    ACCEL_ODR_LP_3_125_Hz   = 0b1110,
    ACCEL_ODR_LP_1_5625_Hz  = 0b1111
}ACCEL_CONFIG0_ACCEL_ODR_t;

/*--------------------------------------------------------------------*/

/*----------------------------------TEMP_CONFG0----------------------------------*/
typedef  enum  {
    // MASK= 0x04,
    TEMP_FILT_BW_DLPF_BYPASSED  = 0b000 << 4,
    TEMP_FILT_BW_DLPF_BW_180_HZ = 0b001 << 4,
    TEMP_FILT_BW_DLPF_BW_72_HZ  = 0b010 << 4,
    TEMP_FILT_BW_DLPF_BW_34_HZ  = 0b011 << 4,
    TEMP_FILT_BW_DLPF_BW_16_HZ  = 0b100 << 4,
    TEMP_FILT_BW_DLPF_BW_8_HZ   = 0b101 << 4,
    TEMP_FILT_BW_DLPF_BW_4_HZ   = 0b110 << 4
    // DLPF_BW_4_HZ = 0b111 << 4
}TEMP_CONFG0_TEMP_FILT_BW_t;

/*--------------------------------------------------------------------*/

/*----------------------------------GYRO_CONFIG1----------------------------------*/
typedef  enum {
    GYRO_UI_FILT_BW_LPF_BW_BYPASSED = 0b000,
    GYRO_UI_FILT_BW_LPF_BW_180_HZ   = 0b001,
    GYRO_UI_FILT_BW_LPF_BW_121_HZ   = 0b010,
    GYRO_UI_FILT_BW_LPF_BW_73_HZ    = 0b011,
    GYRO_UI_FILT_BW_LPF_BW_53_HZ    = 0b100,
    GYRO_UI_FILT_BW_LPF_BW_34_HZ    = 0b101,
    GYRO_UI_FILT_BW_LPF_BW_25_HZ    = 0b110,
    GYRO_UI_FILT_BW_LPF_BW_16_HZ    = 0b111
}GYRO_CONFIG1_GYRO_UI_FILT_BW_t;
/*--------------------------------------------------------------------*/

/*----------------------------------ACCEL_CONFIG1----------------------------------*/
typedef  enum  {
    ACCEL_UI_AVG_AVG_2X  = (0b000 << 4), 
    ACCEL_UI_AVG_AVG_4X  = (0b001 << 4), 
    ACCEL_UI_AVG_AVG_8X  = (0b010 << 4), 
    ACCEL_UI_AVG_AVG_16X = (0b011 << 4), 
    ACCEL_UI_AVG_AVG_32X = (0b100 << 4), 
    ACCEL_UI_AVG_AVG_64X = (0b101 << 4), 
}ACCEL_CONFIG1_ACCEL_UI_AVG_t;

typedef  enum {
    ACCEL_UI_FILT_BW_LPF_BW_BYPASSED = (0b000 << 0), 
    ACCEL_UI_FILT_BW_LPF_BW_180_HZ   = (0b001 << 0), 
    ACCEL_UI_FILT_BW_LPF_BW_121_HZ   = (0b010 << 0), 
    ACCEL_UI_FILT_BW_LPF_BW_73_HZ    = (0b011 << 0), 
    ACCEL_UI_FILT_BW_LPF_BW_53_HZ    = (0b100 << 0), 
    ACCEL_UI_FILT_BW_LPF_BW_34_HZ    = (0b101 << 0), 
    ACCEL_UI_FILT_BW_LPF_BW_25_HZ    = (0b110 << 0), 
    ACCEL_UI_FILT_BW_LPF_BW_16_HZ    = (0b111 << 0) 
}ACCEL_CONFIG1_ACCEL_UI_FILT_BW_t;



/*--------------------------------------------APEX_CONFIG0--------------------------------------------*/
typedef enum{
    _DMP_POWER_SAVE_EN = 0b1 << 3 
}APEX_CONFIG0_DMP_POWER_SAVE_EN_t;


typedef enum{
    DMP_INIT_EN = 0b1 << 2 
}APEX_CONFIG0_DMP_INIT_EN_t;

typedef enum{
    DMP_MEM_RESET_EN = 0b1 
}APEX_CONFIG0_DMP_MEM_RESET_EN_t;

/*----------------------------------------------------------------------------------------*/

/*--------------------------------------------APEX_CONFIG1--------------------------------------------*/
typedef enum{
    SMD_ENABLE = 0b1 << 6
}APEX_CONFIG1_SMD_ENABLE_t;

 typedef enum{
    FF_DISABLE = 0b0 << 5,
    FF_ENABLE  = 0b1 << 5
 }APEX_CONFIG1_FF_ENABLE_t;

typedef enum{
    TILT_DISABLE = 0b0 << 4,
    TILT_ENABLE  = 0b1 << 4
}APEX_CONFIG1_TILT_ENABLE_t;

typedef enum{
    PED_DISABLE = 0b0 << 3,
    PED_ENABLE  = 0b1 << 3
}APEX_CONFIG1_PED_ENABLE_t;


typedef enum{
    DMP_ODR_25    = 0b00,
    DMP_ODR_400   = 0b01, 
    DMP_ODR_50    = 0b10, 
    DMP_ODR_100   = 0b11 
}APEX_CONFIG1_DMP_ODR_t;
/*----------------------------------------------------------------------------------------*/

/*------------------------------------------WOM_CONFIG------------------------------------------*/
typedef enum{
    WOM_INT_DUR_INT_1ST_EVENT = 0b00 << 3,
    WOM_INT_DUR_INT_2ND_EVENT = 0b01 << 3,
    WOM_INT_DUR_INT_3RD_EVENT = 0b10 << 3,
    WOM_INT_DUR_INT_4TH_EVENT = 0b11 << 3,
}WOM_CONFIG_WOM_INT_DUR_t;

typedef enum{
    WOM_INT_MODE_OR_OF_EN_THRESHOLD  = 0b0 << 2,
    WOM_INT_MODE_AND_OF_EN_THRESHOLD = 0b1 << 2
}WOM_CONFIG_WOM_INT_MODE_t;

typedef enum{
    WOM_MODE_COMPARE_FUTURE_TO_INITIAL     =  0b0 << 1,
    WOM_MODE_COMPARE_CURRENT_TO_PREVIOUS   =  0b1 << 1
}WOM_CONFIG_WOM_MODE_t;


typedef enum{
    WOM_CONFIG_WOM_DISBALE =  0b0,
    WOM_CONFIG_WOM_ENABLE  =  0b1
}WOM_CONFIG_WOM_EN_t;

/*--------------------------------------------FIFO_CONFIG1--------------------------------------------*/
typedef enum{
    FIFO_MODE_STREAM_TO_FIFO = 0b0 << 1,
    FIFO_MODE_STOP_ON_FULL   = 0b1 << 1
}FIFO_CONFIG1_FIFO_MODE_t;
typedef enum{
    FIFO_NOT_BYPASSED = 0b0,
    FIFO_BYPASSED     = 0b1
}FIFO_CONFIG1_FIFO_BYPASS_t;
/*----------------------------------------------------------------------------------------*/

/*--------------------------------------------INT_SOURCE0--------------------------------------------*/

typedef enum{
    INT_SOURCE0_ST_INT1_NOT_ROUTED  = 0b0 << 7,
    INT_SOURCE0_ST_INT1_ROUTED      = 0b1 << 7
}INT_SOURCE0_ST_INT1_EN_t;

typedef enum{
    INT_SOURCE0_FSYNC_INT1_NOT_ROUTED  = 0b0 << 6,
    INT_SOURCE0_FSYNC_INT1_ROUTED      = 0b1 << 6
}INT_SOURCE0_FSYNC_INT1_EN_t;

typedef enum{
    INT_SOURCE0_PLL_RDY_INT1_NOT_ROUTED  = 0b0 << 5,
    INT_SOURCE0_PLL_RDY_INT1_ROUTED      = 0b1 << 5
}INT_SOURCE0_PLL_RDY_INT1_EN_t;

typedef enum{
    INT_SOURCE0_RESET_DONE_INT1_NOT_ROUTED  = 0b0 << 4,
    INT_SOURCE0_RESET_DONE_INT1_ROUTED      = 0b1 << 4
}INT_SOURCE0_RESET_DONE_INT1_EN_t;

typedef enum{
    INT_SOURCE0_DRDY_INT1_NOT_ROUTED  = 0b0 << 3,
    INT_SOURCE0_DRDY_INT1_ROUTED      = 0b1 << 3
}INT_SOURCE0_DRDY_INT1_EN_t;
typedef enum{
    INT_SOURCE0_FIFO_THS_INT1_NOT_ROUTED  = 0b0 << 2,
    INT_SOURCE0_FIFO_THS_INT1_ROUTED      = 0b1 << 2
}INT_SOURCE0_FIFO_THS_INT1_EN_t;

typedef enum{
    INT_SOURCE0_FIFO_FULL_INT1_NOT_ROUTED  = 0b0 << 1,
    INT_SOURCE0_FIFO_FULL_INT1_ROUTED      = 0b1 << 1
}INT_SOURCE0_FIFO_FULL_INT1_EN_t;

typedef enum{
    INT_SOURCE0_AGC_RDY_INT1_NOT_ROUTED  = 0b0,
    INT_SOURCE0_AGC_RDY_INT1_ROUTED      = 0b1
}INT_SOURCE0_AGC_RDY_INT1_EN_t;

/*----------------------------------------------------------------------------------------*/

/*--------------------------------------------INT_SOURCE1--------------------------------------------*/
typedef enum{
    I3C_PROTOCOL_ERROR_INT1_NOT_ROUTED = 0b0 << 6,
    I3C_PROTOCOL_ERROR_INT1_ROUTED = 0b1 << 6
}INT_SOURCE1_I3C_PROTOCOL_ERROR_INT1_EN_t;

typedef enum{
    SMD_INT1_NOT_ROUTED = 0b0 << 3,
    SMD_INT1_ROUTED     = 0b1 << 3
}INT_SOURCE1_SMD_INT1_EN_t;


typedef enum{
    WOM_Z_INT1_NOT_ROUTED = 0b0 << 2,
    WOM_Z_INT1_ROUTED     = 0b1 << 2
}INT_SOURCE1_WOM_Z_INT1_EN_t;

typedef enum{
    WOM_Y_INT1_NOT_ROUTED = 0b0 << 1,
    WOM_Y_INT1_ROUTED     = 0b1 << 1
}INT_SOURCE1_WOM_Y_INT1_EN_t;

typedef enum{
    WOM_X_INT1_NOT_ROUTED = 0b0,
    WOM_X_INT1_ROUTED     = 0b1
}INT_SOURCE1_WOM_x_INT1_EN_t;

/*----------------------------------------------------------------------------------------*/


/*--------------------------------------------INT_SOURCE3--------------------------------------------*/

typedef enum{
    INT_SOURCE3_ST_INT2_NOT_ROUTED          = 0b0 << 7,
    INT_SOURCE3_ST_INT2_ROUTED              = 0b1 << 7
}INT_SOURCE3_ST_INT2_EN_t;

typedef enum{
    INT_SOURCE3_FSYNC_INT2_NOT_ROUTED       = 0b0 << 6,
    INT_SOURCE3_FSYNC_INT2_ROUTED           = 0b1 << 6
}INT_SOURCE3_FSYNC_INT2_EN_t;

typedef enum{
    INT_SOURCE3_PLL_RDY_INT2_NOT_ROUTED     = 0b0 << 5,
    INT_SOURCE3_PLL_RDY_INT2_ROUTED         = 0b1 << 5
}INT_SOURCE3_PLL_RDY_INT2_EN_t;

typedef enum{
    INT_SOURCE3_RESET_DONE_INT2_NOT_ROUTED  = 0b0 << 4,
    INT_SOURCE3_RESET_DONE_INT2_ROUTED      = 0b1 << 4
}INT_SOURCE3_RESET_DONE_INT2_EN_t;

typedef enum{
    INT_SOURCE3_DRDY_INT2_NOT_ROUTED        = 0b0 << 3,
    INT_SOURCE3_DRDY_INT2_ROUTED            = 0b1 << 3
}INT_SOURCE3_DRDY_INT2_EN_t;

typedef enum{
    INT_SOURCE3_FIFO_THS_INT2_NOT_ROUTED    = 0b0 << 2,
    INT_SOURCE3_FIFO_THS_INT2_ROUTED        = 0b1 << 2
}INT_SOURCE3_FIFO_THS_INT2_EN_t;

typedef enum{
    INT_SOURCE3_FIFO_FULL_INT2_NOT_ROUTED   = 0b0 << 1,
    INT_SOURCE3_FIFO_FULL_INT2_ROUTED       = 0b1 << 1
}INT_SOURCE3_FIFO_FULL_INT2_EN_t;

typedef enum{
    INT_SOURCE3_AGC_RDY_INT2_NOT_ROUTED     = 0b0,
    INT_SOURCE3_AGC_RDY_INT2_ROUTED         = 0b1
}INT_SOURCE3_AGC_RDY_INT2_EN_t;

/*----------------------------------------------------------------------------------------*/

/*--------------------------------------------INT_SOURCE4--------------------------------------------*/

typedef enum{
    I3C_PROTOCOL_ERROR_INT2_NOT_ROUTED  = 0b0 << 6,
    I3C_PROTOCOL_ERROR_INT2_ROUTED      = 0b1 << 6
}INT_SOURCE_I3C_PROTOCOL_ERROR_INT2_EN_t;

typedef enum{
    SMD_INT2_EN_NOT_ROUTED  = 0b0 << 3,
    SMD_INT2_EN_ROUTED      = 0b1 << 3
}INT_SOURCE4_SMD_INT2_EN_t;

typedef enum{
    WOM_Z_INT2_EN_NOT_ROUTED  = 0b0 << 2,
    WOM_Z_INT2_EN_ROUTED      = 0b1 << 2
}INT_SOURCE4_WOM_Z_INT2_EN_t;

typedef enum{
    WOM_Y_INT2_EN_NOT_ROUTED  = 0b0 << 1,
    WOM_Y_INT2_EN_ROUTED      = 0b1 << 1
}INT_SOURCE4_WOM_Y_INT2_EN_t;

typedef enum{
    WOM_X_INT2_EN_NOT_ROUTED  = 0b0 << 0,
    WOM_X_INT2_EN_ROUTED      = 0b1 << 0
}INT_SOURCE4_WOM_X_INT2_EN_t;
/*----------------------------------------------------------------------------------------*/


/*--------------------------------------------INTF_CONFIG0--------------------------------------------*/
typedef enum{
    FIFO_COUNT_FORMAT_BYTES     = 0b0 << 6,          
    FIFO_COUNT_FORMAT_RECORDS   = 0b1 << 6
}INTF_CONFIG0_FIFO_COUNT_FORMAT_t;


typedef enum{
    FIFO_COUNT_REPORTED_ENDAIN_LITTLE   = 0b0 << 5,          
    FIFO_COUNT_REPORTED_ENDAIN_BIG      = 0b1 << 5
}INTF_CONFIG0_FIFO_COUNT_ENDAIN_t;

typedef enum{
    SENSOR_DATA_REPORTED_ENDAIN_LITTLE   = 0b0 << 4,          
    SENSOR_DATA_REPORTED_ENDAIN_BIG      = 0b1 << 4
}INTF_CONFIG0_SENSOR_DATA_ENDAIN_t;

/*----------------------------------------------------------------------------------------*/

/*--------------------------------------------INTF_CONFIG1--------------------------------------------*/
typedef enum{
    I3C_SDR_MODE_DISABLE = 0b0 << 3,
    I3C_SDR_MODE_ENABLE  = 0b1 << 3
}INTF_CONFIG1_I3C_SDR_EN_t;

typedef enum{
    I3C_DDR_MODE_DISABLE = 0b0 << 2,
    I3C_DDR_MODE_ENABLE  = 0b1 << 2
}INTF_CONFIG1_I3C_DDR_EN_t;

typedef enum{
    CLKSEL_INTERNAL_RC_OSCI = 0b00,
    CLKSEL_PPL_ELSE_RC_OSCI = 0b01,
    CLKSEL_DISABLE_ALL      = 0b11
}INTF_CONFIG1_CLKSEL_EN_t;
/*----------------------------------------------------------------------------------------*/

/*--------------------------------------------TMST_CONFIG1--------------------------------------------*/
typedef enum{
    REPORT_DEL_TIME_FSYNC_TO_NEXT_ODR       = 0b0 << 4,
    REPORT_DEL_TIME_FSYNC_TO_NEXT_ODR_EVENT = 0b1 << 4
}TMST_CONFIG1_TMST_ON_SREG_EN_t;

typedef enum{
    TMST_RES_1us    = 0b0 << 3,
    TMST_RES_16us   = 0b1 << 3
}TMST_CONFIG1_TMST_RES_t;

typedef enum{
    TMST_DELTA_DISBALE = 0b0 << 2,
    TMST_DELTA_ENABLE  = 0b1 << 2
}TMST_CONFIG1_TMST_DELTA_EN_t;

typedef enum{
    TMST_FSYNC_DISBALE = 0b0 << 1,
    TMST_FSYNC_ENABLE  = 0b1 << 1
}TMST_CONFIG1_TMST_FSYNC_EN_t;

typedef enum{
    TMST_REGISTER_DISABLE = 0b0,
    TMST_REGISTER_ENABLE  = 0b1
}TMST_CONFIG1_TMST_EN_t;
/*----------------------------------------------------------------------------------------*/


/*--------------------------------------------FIFO_CONFIG5--------------------------------------------*/
typedef enum{
    WM_INT_FIFO_COUNT_EQ_WM     = 0b0 << 5,
    WM_INT_ODR_FIFO_COUNT_EQ_WM = 0b1 << 5
}FIFO_CONFIG5_FIFO_WM_GT_TH_t;

typedef enum{
    FIFO_RESUME_PARTIAL_RD_DISABLE  = 0b0 << 4,
    FIFO_RESUME_PARTIAL_RD_ENABLE   = 0b1 << 4  
}FIFO_CONFIG5_FIFO_RESUME_PARTIAL_RD_t;
typedef enum{
    FIFO_HI_RES_DISABLE = 0b0 << 3,
    FIFO_HI_RES_ENABLE  = 0b1 << 3
}FIFO_CONFIG5_FIFO_HIRES_EN_t;
typedef enum{
    FIFO_TMST_FSYNC_DISABLE =   0b0 << 2,
    FIFO_TMST_FSYNC_ENABLE  =   0b1 << 2
}FIFO_CONFIG5_FIFO_TMST_FSYNC_EN_t;
typedef enum{
    FIFO_GYRO_DISABLE   = 0b0 << 1, 
    FIFO_GYRO_ENABLE    = 0b1 << 1
}FIFO_CONFIG5_FIFO_GYRO_EN_t;

typedef enum{
    FIFO_ACCEL_DISABLE   = 0b0, 
    FIFO_ACCEL_ENABLE    = 0b1
}FIFO_CONFIG5_FIFO_ACCEL_EN_t;
/*----------------------------------------------------------------------------------------*/

/*--------------------------------------------FIFO_CONFIG5--------------------------------------------*/
 typedef enum{
    FIFO_EMPTY_INDICATOR_DISABLE = 0b0 << 4,
    FIFO_EMPTY_INDICATOR_ENABLE  = 0b1 << 4

 }FIFO_CONFIG5_FIFO_EMPTY_INDICATOR_DIS_t;

 typedef enum{
    RCOSC_REQ_ON_FIFO_THS_DISABLE = 0b0,
    RCOSC_REQ_ON_FIFO_THS_ENABLE  = 0b1,
 }FIFO_CONFIG5_RCOSC_REQ_ON_FIFO_THS_DIS_t;
/*----------------------------------------------------------------------------------------*/

/*--------------------------------------------FSYNC_CONFIG--------------------------------------------*/
typedef enum{
    FSYNC_FLAG_NO_TAG           = 0b000 << 4,
    FSYNC_FLAG_TEMP_OUT_LSB     = 0b001 << 4,
    FSYNC_FLAG_GYRO_XOUT_LSB    = 0b010 << 4,
    FSYNC_FLAG_GYRO_YOUT_LSB    = 0b011 << 4,
    FSYNC_FLAG_GYRO_ZOUT_LSB    = 0b100 << 4,
    FSYNC_FLAG_ACCEL_XOUT_LSB   = 0b101 << 4,
    FSYNC_FLAG_ACCEL_YOUT_LSB   = 0b110 << 4,
    FSYNC_FLAG_ACCEL_ZOUT_LSB   = 0b111 << 4
}FSYNC_CONFIG_FSYNC_UI_SEL_t;
typedef enum{
    FSYNC_UI_FLAG_CLEAR_REG_UPDATE  = 0b0 << 1,  
    FSYNC_UI_FLAG_CLEAR_REG_READ    = 0b1 << 1
}FSYNC_CONFIG_FSYNC_UI_FLAG_CLEAR_SEL_t;
typedef enum{
    FSYNC_POLARITY_RISING_EDGE  = 0b0,
    FSYNC_POLARITY_FALLING_EDGE = 0b1
}FSYNC_CONFIG_FSYNC_POLARITY_t;
/*----------------------------------------------------------------------------------------*/
/*--------------------------------------------INT_CONFIG0--------------------------------------------*/
typedef enum{
    UI_DRDY_INT_CLEAR_ON_STAT_BIT_READ   = 0b00 << 4,      
    UI_DRDY_INT_CLEAR_ON__STAT_BIT_READ  = 0b01 << 4,    
    UI_DRDY_INT_CLEAR_ON_SENSOR_REG_READ = 0b10 << 4,        
    UI_DRDY_INT_CLEAR_ON_STAT_REG_READ   = 0b11 << 4       
}INT_CONFIG_UI_DRDY_INT_CLEAR_t;
typedef enum{
    FIFO_THS_INT_CLEAR_ON_STAT_BIT_READ  = 0b00 << 2, 
    FIFO_THS_INT_CLEAR__ON_STAT_BIT_READ = 0b01 << 2, 
    FIFO_THS_INT_CLEAR_ON_FIFO_1BYT_READ = 0b10 << 2,
    FIFO_THS_INT_CLEAR_ON_STAT_FIFO_READ = 0b11 << 2
}INT_CONFIG_FIFO_THS_INT_CLEAR_t;
typedef enum{
    FIFO_FULL_INT_CLEAR_ON_STAT_BIT_READ  = 0b00 << 0, 
    FIFO_FULL_INT_CLEAR__ON_STAT_BIT_READ = 0b01 << 0, 
    FIFO_FULL_INT_CLEAR_ON_FIFO_1BYT_READ = 0b10 << 0,
    FIFO_FULL_INT_CLEAR_ON_STAT_FIFO_READ = 0b11 << 0
}INT_CONFIG_FIFO_FULL_INT_CLEAR_t;
/*----------------------------------------------------------------------------------------*/

/*--------------------------------------------INT_CONFIG1--------------------------------------------*/
typedef enum {
    INT_TPULSE_DURATION_100us = 0b0 << 6,  
    INT_TPULSE_DURATION_8us   = 0b1 << 6 
}INT_CONFIG1_INT_TPULSE_DURATION_t;
typedef enum {
    INT_ASYNC_RESET_ON_STAT_REG_READ = 0b0 << 4, 
    INT_ASYNC_RESET_SUSTAIN          = 0b1 << 4 
}INT_CONFIG1_INT_ASYNC_RESET_t;
/*----------------------------------------------------------------------------------------*/

/*--------------------------------------------SENSOR_CONFIG3--------------------------------------------*/
typedef enum {
    APEX_DISABLE = 0b1 << 6,
    APEX_ENABLE  = 0b0 << 6
}SENSOR_CONFIG3_APEX_DISABLE_t;
/*----------------------------------------------------------------------------------------*/

/*-------------------------------------------- APEX_CONFIG9--------------------------------------------*/
typedef enum{
    SMD_SENSITIVITY_LEVEL_0 = 0b000 << 1,
    SMD_SENSITIVITY_LEVEL_1 = 0b001 << 1,
    SMD_SENSITIVITY_LEVEL_2 = 0b010 << 1,
    SMD_SENSITIVITY_LEVEL_3 = 0b011 << 1,
    SMD_SENSITIVITY_LEVEL_4 = 0b100 << 1
}SMD_SENSITIVITY_SEL_t;

/*----------------------------------------------------------------------------------------*/
// clang-format on
typedef struct {
    uint16_t xl[3];
    uint16_t gy[3];
    uint16_t temp;

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
    bool begin();
    bool get_imu_data(imu_data_t &data);
    bool get_imu_raw_data(imu_raw_data_t &data);

    void user_bank_0_read(uint8_t reg, uint8_t *read_buffer, uint32_t len);
    void user_bank_0_write(uint8_t reg, const uint8_t *write_buffer, uint32_t len);

    void user_bank_mreg_0_read();
    void user_bank_mreg_0_write();

    void user_bank_mreg_1_read();
    void user_bank_mreg_1_write();

    bool accel_setting(uint16_t fsr = 2, uint16_t odr = 1600);
    bool gyro_setting(uint16_t fsr = 250, uint16_t odr = 1600);

    bool accel_set_fsr(uint16_t setting);
    bool accel_set_odr(uint16_t setting);
    bool accel_set_average_filter(uint16_t setting);
    bool accel_set_lpf_bandwidth(uint8_t setting);
    bool accel_set_mode(uint8_t setting);

    bool gyro_set_fsr(uint16_t setting);
    bool gyro_set_odr(uint16_t setting);
    bool gyro_set_lpf_bandwidth(uint8_t setting);
    bool gyro_set_mode(uint8_t setting);

    bool configure_interrupt(uint8_t setting);
    uint8_t interrupt_status_smd_wom();

    /*Wakeup on motion setting functions*/
    bool configure_wakeup_on_motion(uint8_t setting = 0);
    bool route_wom_smd_to_int(uint8_t interrupt_no = 1, uint8_t settings = 0);

    bool enable_significant_motion_detection(bool enable = true, uint8_t sensitivity = 1);

    bool wom_x_threshold(float threshold = 3.9);
    bool wom_y_threshold(float threshold = 3.9);
    bool wom_z_threshold(float threshold = 3.9);

    bool temp_set_lpf(uint8_t lpf_bw = 0);

    void set_debug_stream(Stream *debug_stream, uint8_t level = 0);

    float get_range_g();
    float get_range_dps();

   private:
    uint8_t imu_cs;
    SPIClass *imu_spi;
    uint32_t spi_clock;
    Stream *debug_output;
    uint8_t debug_level;
    float g_range;
    float dps_range;

    float readings_2_g(int16_t value);
    float readings_2_dps(int16_t value);

    bool write_register(uint8_t reg, uint8_t setting);
    void serial_read(uint8_t reg, uint8_t *read_buffer, uint32_t len);
    void serial_write(uint8_t reg, const uint8_t *write_buffer, uint32_t len);

    bool write_mreg1_register(uint8_t reg, uint8_t *write_value);
    bool read_mreg1_register(uint8_t reg, uint8_t *write_value);

    bool read_mreg_registers(uint8_t register_bank, uint8_t reg, uint8_t *read_value);
    bool write_mreg_registers(uint8_t register_bank, uint8_t reg, uint8_t *write_value);

    uint8_t accel_map_fsr(uint16_t fsr);
    uint8_t accel_map_odr(uint16_t odr);

    uint8_t accel_map_lpf_bw(uint8_t lpf_bw);
    uint8_t accel_map_av_filter(uint8_t av_filter);

    uint8_t gyro_map_fsr(uint16_t fsr);
    uint8_t gyro_map_odr(uint16_t odr);
    uint8_t gyro_map_lpf_bw(uint8_t lpf_bw);
    uint8_t threshold_mg_to_byte(float threshold);
};
#endif
