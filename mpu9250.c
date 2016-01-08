/*
 * 
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf_drv_twi.h"
#include "mpu9250.h"
#include "mpu9250_register_map.h"

static nrf_drv_twi_t const * m_twi_instance;
static bool twi_tx_done = false;
static bool twi_rx_done = false;


void mpu9250_twi_event_handler(const nrf_drv_twi_evt_t *evt)
{ 
    switch(evt->type)
    {
        case NRF_DRV_TWI_TX_DONE:
            twi_tx_done = true;
            break;
        case NRF_DRV_TWI_RX_DONE:
            twi_rx_done = true;
            break;
        case NRF_DRV_TWI_ERROR:
            switch(evt->error_src)
            {
                case NRF_TWI_ERROR_ADDRESS_NACK:
                    break;
                case NRF_TWI_ERROR_OVERRUN_NACK:
                    break;
                case NRF_TWI_ERROR_DATA_NACK:
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }        
}


static uint32_t mpu9250_write_burst(uint8_t reg, uint8_t const * p_data, uint32_t length)
{
    uint32_t err_code;
    uint32_t timeout = MPU9250_TWI_TIMEOUT;

    err_code = nrf_drv_twi_tx(m_twi_instance, MPU9250_ADDRESS, &reg, 1, true);
    if(err_code != NRF_SUCCESS) return err_code;
    
    while((!twi_tx_done) && timeout--);  
    if(!timeout) return NRF_ERROR_TIMEOUT;
    twi_tx_done = false;
    
    err_code = nrf_drv_twi_tx(m_twi_instance, MPU9250_ADDRESS, p_data, length, false);
    if(err_code != NRF_SUCCESS) return err_code;
    
    timeout = MPU9250_TWI_TIMEOUT;
    while((!twi_tx_done) && timeout--);
    if(!timeout) return NRF_ERROR_TIMEOUT;
    twi_tx_done = false;
    
    return err_code;
}

static uint32_t mpu9250_write_single(uint8_t reg, uint8_t data)
{
    uint32_t err_code;
    uint32_t timeout = MPU9250_TWI_TIMEOUT;
    
    uint8_t packet[2] = {reg, data};
    
    err_code = nrf_drv_twi_tx(m_twi_instance, MPU9250_ADDRESS, packet, 2, false);
    if(err_code != NRF_SUCCESS) return err_code;
    
    while((!twi_tx_done) && timeout--);
    if(!timeout) return NRF_ERROR_TIMEOUT;
    
    twi_tx_done = false;
    
    return err_code;
}


uint32_t mpu9250_read_registers(uint8_t reg, uint8_t * p_data, uint32_t length)
{
    uint32_t err_code;
    uint32_t timeout = MPU9250_TWI_TIMEOUT;
    
    err_code = nrf_drv_twi_tx(m_twi_instance, MPU9250_ADDRESS, &reg, 1, false);
    if(err_code != NRF_SUCCESS) return err_code;
    
    while((!twi_tx_done) && timeout--);  
    if(!timeout) return NRF_ERROR_TIMEOUT;
    twi_tx_done = false;
    
    err_code = nrf_drv_twi_rx(m_twi_instance, MPU9250_ADDRESS, p_data, length, false);
    if(err_code != NRF_SUCCESS) return err_code;
    
    timeout = MPU9250_TWI_TIMEOUT;
    while((!twi_rx_done) && timeout--);
    if(!timeout) return NRF_ERROR_TIMEOUT;
    twi_rx_done = false;
    
    return err_code;
}


uint32_t mpu9250_config(mpu9250_config_t * config)
{
    uint8_t *data; 
    data = (uint8_t*)config;
    return mpu9250_write_burst(MPU9250_REG_SMPLRT_DIV, data, 4);
}

uint32_t mpu9250_int_cfg_pin(mpu9250_int_pin_cfg_t *cfg)
{
    uint8_t *data; 
    data = (uint8_t*)cfg;
    return mpu9250_write_single(MPU9250_REG_INT_PIN_CFG, *data);
}

uint32_t mpu9250_int_enable(mpu9250_int_enable_t *cfg)
{
    uint8_t *data; 
    data = (uint8_t*)cfg;
    return mpu9250_write_single(MPU9250_REG_INT_ENABLE, *data);
}
    
    

uint32_t mpu9250_init(nrf_drv_twi_t const * const p_instance)
{
    uint32_t err_code;
    m_twi_instance = p_instance;
    
    uint8_t reset_value = 7; // Resets gyro, accelerometer and temperature sensor signal paths.
    err_code = mpu9250_write_single(MPU9250_REG_SIGNAL_PATH_RESET, reset_value);
    if(err_code != NRF_SUCCESS) return err_code;
    
    // Power up MPU
    err_code = mpu9250_write_single(MPU9250_REG_PWR_MGMT_1, 1);
    if(err_code != NRF_SUCCESS) return err_code;
    
    return NRF_SUCCESS;
}

uint32_t mpu9250_read_accel(accel_values_t * accel_values)
{
    uint32_t err_code;
    uint8_t raw_values[6];
    err_code = mpu9250_read_registers(MPU9250_REG_ACCEL_XOUT_H, raw_values, 6);
    if(err_code != NRF_SUCCESS) 
			return err_code;
    
    // Reorganize read sensor values and put them into value struct
    uint8_t *data;
    data = (uint8_t*)accel_values;
    for(uint8_t i = 0; i<6; i++)
    {
        *data = raw_values[5-i];
        data++;
    }
    return NRF_SUCCESS;
}

uint32_t mpu9250_read_gyro(gyro_values_t * gyro_values)
{
    uint32_t err_code;
    uint8_t raw_values[6];
    err_code = mpu9250_read_registers(MPU9250_REG_GYRO_XOUT_H, raw_values, 6);
    if(err_code != NRF_SUCCESS) return err_code;
    
    // Reorganize read sensor values and put them into value struct
    uint8_t *data;
    data = (uint8_t*)gyro_values;
    for(uint8_t i = 0; i<6; i++)
    {
        *data = raw_values[5-i];
        data++;
    }
    return NRF_SUCCESS;
}

uint32_t mpu9250_read_mag(mag_values_t * gyro_values)
{
    uint32_t err_code;
    uint8_t raw_values[6];
    err_code = mpu9250_read_registers(MPU9250_REG_AK8963_XOUT_L, raw_values, 6);
    if(err_code != NRF_SUCCESS) return err_code;
    
    // Reorganize read sensor values and put them into value struct
    uint8_t *data;
    data = (uint8_t*)gyro_values;
    for(uint8_t i = 0; i<6; i++)
    {
        *data = raw_values[5-i];
        data++;
    }
    return NRF_SUCCESS;
}

uint32_t mpu9250_read_temp(temp_value_t * temperature)
{
    uint32_t err_code;
    uint8_t raw_values[2];
    err_code = mpu9250_read_registers(MPU9250_REG_TEMP_OUT_H, raw_values, 2);
    if(err_code != NRF_SUCCESS) return err_code;  
    
    *temperature = (temp_value_t)(raw_values[0] << 8) + raw_values[1];
    
    return NRF_SUCCESS;    
}

/**
  @}
*/
