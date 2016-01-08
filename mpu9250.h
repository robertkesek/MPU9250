/*
 * Made by Robert Kesek
 */

#ifndef MPU9250_H
#define MPU9250_H


#include <stdbool.h>
#include <stdint.h>
#include "nrf_drv_twi.h"

enum Ascale {
  AFS_2G = 0,
  AFS_4G,
  AFS_8G,
  AFS_16G
};

enum Gscale {
  GFS_250DPS = 0,
  GFS_500DPS,
  GFS_1000DPS,
  GFS_2000DPS
};

enum Mscale {
  MFS_14BITS = 0, // 0.6 mG per LSB
  MFS_16BITS      // 0.15 mG per LSB
};

typedef struct
{
    int16_t z;
    int16_t y;
    int16_t x;
}accel_values_t;

typedef struct
{
    int16_t z;
    int16_t y;
    int16_t x;
}gyro_values_t;

typedef struct
{
    int16_t z;
    int16_t y;
    int16_t x;
}mag_values_t;

typedef int16_t temp_value_t;

/**@brief MPU9250 driver digital low pass filter and external Frame Synchronization (FSYNC) pin sampling configuration structure */
typedef struct
{
    uint8_t dlpf_cfg     :3; // 3-bit unsigned value. Configures the Digital Low Pass Filter setting.
    uint8_t ext_sync_set :3; // 3-bit unsigned value. Configures the external Frame Synchronization (FSYNC) pin sampling.
    uint8_t              :2;
}sync_dlpf_config_t;

/**@brief MPU9250 driver gyro configuration structure. */
typedef struct
{
    uint8_t                 :3;
    uint8_t fs_sel          :2; // FS_SEL 2-bit unsigned value. Selects the full scale range of gyroscopes.
    uint8_t                 :3;
}gyro_config_t;

/**@brief MPU9250 driver accelerometer configuration structure. */
typedef struct
{
    uint8_t accel_hpf       :3; // 3-bit unsigned value. Selects the Digital High Pass Filter configuration.
    uint8_t afs_sel         :2; // 2-bit unsigned value. Selects the full scale range of accelerometers.
    uint8_t za_st           :1; // When set to 1, the Z- Axis accelerometer performs self test.
    uint8_t ya_st           :1; // When set to 1, the Y- Axis accelerometer performs self test.
    uint8_t xa_st           :1; // When set to 1, the X- Axis accelerometer performs self test.
}accel_config_t;

typedef struct
{
    uint8_t             smplrt_div;         // Divider from the gyroscope output rate used to generate the Sample Rate for the MPU-9250. Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
    sync_dlpf_config_t  sync_dlpf_gonfig;   // Digital low pass fileter and external Frame Synchronization (FSYNC) configuration structure
    gyro_config_t       gyro_config;        // Gyro configuration structure
    accel_config_t      accel_config;       // Accelerometer configuration structure
}mpu9250_config_t;


/**@brief MPU9250 instance default configuration. */
#define MPU9250_DEFAULT_CONFIG()                          \
    {                                                     \
        .smplrt_div                     = 7,              \
        .sync_dlpf_gonfig.dlpf_cfg      = 1,              \
        .sync_dlpf_gonfig.ext_sync_set  = 0,              \
        .gyro_config.fs_sel             = GFS_2000DPS,    \
        .accel_config.accel_hpf         = 0,              \
        .accel_config.afs_sel           = AFS_16G,        \
        .accel_config.za_st             = 0,              \
        .accel_config.ya_st             = 0,              \
        .accel_config.xa_st             = 0,              \
    }

/**@brief MPU9250 driver interrupt pin configuration structure. */    
typedef struct
{
    uint8_t clkout_en       :1;  // When this bit is equal to 1, a reference clock output is provided at the CLKOUT pin. When this bit is equal to 0, the clock output is disabled. For further information regarding CLKOUT, please refer to the MPU-9250 Product Specification document.
    uint8_t i2c_bypass_en   :1;  // When this bit is equal to 1 and I2C_MST_EN (Register 106 bit[5]) is equal to 0, the host application processor will be able to directly access the auxiliary I2C bus of the MPU-9250. When this bit is equal to 0, the host application processor will not be able to directly access the auxiliary I2C bus of the MPU-9250 regardless of the state of I2C_MST_EN (Register 106 bit[5]).
    uint8_t fsync_int_en    :1;  // When equal to 0, this bit disables the FSYNC pin from causing an interrupt to the host processor. When equal to 1, this bit enables the FSYNC pin to be used as an interrupt to the host processor.
    uint8_t fsync_int_level :1;  // When this bit is equal to 0, the logic level for the FSYNC pin (when used as an interrupt to the host processor) is active high. When this bit is equal to 1, the logic level for the FSYNC pin (when used as an interrupt to the host processor) is active low.
    uint8_t int_rd_clear    :1;  // When this bit is equal to 0, interrupt status bits are cleared only by reading INT_STATUS (Register 58). When this bit is equal to 1, interrupt status bits are cleared on any read operation.
    uint8_t latch_int_en    :1;  // When this bit is equal to 0, the INT pin emits a 50us long pulse. When this bit is equal to 1, the INT pin is held high until the interrupt is cleared.
    uint8_t int_open        :1;  // When this bit is equal to 0, the INT pin is configured as push-pull. When this bit is equal to 1, the INT pin is configured as open drain.
    uint8_t int_level       :1; // When this bit is equal to 0, the logic level for the INT pin is active high. When this bit is equal to 1, the logic level for the INT pin is active low.
}mpu9250_int_pin_cfg_t;

/**@brief MPU9250 interrupt pin default configuration. */
#define MPU9250_DEFAULT_INT_PIN_CONFIG()    \
{                                       \
    .clkout_en          = 0,    \
    .i2c_bypass_en      = 0,    \
    .fsync_int_en       = 0,    \
    .fsync_int_level    = 0,    \
    .int_rd_clear       = 1,    \
    .latch_int_en       = 0,    \
    .int_open           = 0,    \
    .int_level          = 0,    \
}

/**@brief MPU9250 driver interrupt source configuration structure. */
typedef struct
{
    uint8_t data_rdy_en     :1; // When set to 1, this bit enables the Data Ready interrupt, which occurs each time a write operation to all of the sensor registers has been completed.
    uint8_t                 :2; // 
    uint8_t i2c_mst_int_en  :1; // When set to 1, this bit enables any of the I2C Master interrupt sources to generate an interrupt
    uint8_t fifo_oflow_en   :1; // When set to 1, this bit enables a FIFO buffer overflow to generate an interrupt.
    uint8_t zmot_en         :1; // When set to 1, this bit enables Zero Motion detection to generate an interrupt.
    uint8_t mot_en          :1; // When set to 1, this bit enables Motion detection to generate an interrupt.
    uint8_t ff_en           :1; // When set to 1, this bit enables Free Fall detection to generate an interrupt.
}mpu9250_int_enable_t;

/**@brief MPU9250 interrupt sources default configuration. */
#define MPU9250_DEFAULT_INT_ENABLE_CONFIG() \
{                           \
    .data_rdy_en    = 0,    \
    .i2c_mst_int_en = 0,    \
    .fifo_oflow_en  = 0,    \
    .zmot_en        = 0,    \
    .mot_en         = 0,    \
    .ff_en          = 0,    \
}


/**@brief Event handler used to pass TWI events from main application to the MPU library
 *
 * @param[in]   evt             TWI driver event
 * @retval      uint32_t        Error code
 */
void mpu9250_twi_event_handler(const nrf_drv_twi_evt_t *evt);
  

/**@brief Function for initiating MPU and MPU library
 * 
 * Resets gyro, accelerometer and temperature sensor signal paths.
 * Function resets the analog and digital signal paths of the gyroscope, accelerometer,
 * and temperature sensors.
 * The reset will revert the signal path analog to digital converters and filters to their power up
 * configurations.
 *
 * @param[in]   p_instance      Instance of nRF5x TWI module
 * @retval      uint32_t        Error code
 */
uint32_t mpu9250_init(nrf_drv_twi_t const * const p_instance);
    

/**@brief Function for basic configuring of the MPU
 * 
 * Register 25 – Sample Rate Divider SMPRT_DIV. This register specifies 
 * the divider from the gyroscope output rate used to generate the Sample Rate for the MPU-9250.
 * 
 * Register 26 – Configuration CONFIG. This register configures 
 * the external Frame Synchronization (FSYNC) pin sampling and the Digital Low Pass Filter (DLPF) 
 * setting for both the gyroscopes and accelerometers.
 * 
 * Register 27 – Gyroscope Configuration GYRO_CONFIG. This register is used to configure the gyroscopes’ full scale range.
 * 
 * Register 28 – Accelerometer Configuration ACCEL_CONFIG. This register is 
 * used to trigger accelerometer self test and configure the accelerometer full scale range. 
 * This register also configures the Digital High Pass Filter (DHPF).
 *
 * @param[in]   config          Pointer to configuration structure
 * @retval      uint32_t        Error code
 */
uint32_t mpu9250_config(mpu9250_config_t * config);


/**@brief Function for configuring the behaviour of the interrupt pin of the MPU
 *
 * @param[in]   config          Pointer to configuration structure
 * @retval      uint32_t        Error code
 */
uint32_t mpu9250_int_cfg_pin(mpu9250_int_pin_cfg_t *cfg);


/**@brief Function for eneabling interrupts sources in MPU
 *
 * @param[in]   config          Pointer to configuration structure
 * @retval      uint32_t        Error code
 */
uint32_t mpu9250_int_enable(mpu9250_int_enable_t *cfg);
    

/**@brief Function for reading MPU accelerometer data.
 *
 * @param[in]   accel_values    Pointer to variable to hold accelerometer data
 * @retval      uint32_t        Error code
 */
uint32_t mpu9250_read_accel(accel_values_t * accel_values);


/**@brief Function for reading MPU gyroscope data.
 *
 * @param[in]   gyro_values     Pointer to variable to hold gyroscope data
 * @retval      uint32_t        Error code
 */
uint32_t mpu9250_read_gyro(gyro_values_t * gyro_values);


/**@brief Function for reading MPU magnetometr data.
 *
 * @param[in]   mag_values     Pointer to variable to hold magnetometer data
 * @retval      uint32_t        Error code
 */
uint32_t mpu9250_read_mag(mag_values_t * mag_values);


/**@brief Function for reading MPU temperature data.
 *
 * @param[in]   temp_values     Pointer to variable to hold temperature data
 * @retval      uint32_t        Error code
 */
uint32_t mpu9250_read_temp(temp_value_t * temp_values);

#endif /* MPU9250_H */
