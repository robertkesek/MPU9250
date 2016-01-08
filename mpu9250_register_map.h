
#define MPU9250_REG_AK8963_ST1       0x02  // data ready status bit 0
#define MPU9250_REG_AK8963_XOUT_L    0x03  // data
#define MPU9250_REG_AK8963_XOUT_H    0x04
#define MPU9250_REG_AK8963_YOUT_L    0x05
#define MPU9250_REG_AK8963_YOUT_H    0x06
#define MPU9250_REG_AK8963_ZOUT_L    0x07
#define MPU9250_REG_AK8963_ZOUT_H    0x08
#define MPU9250_REG_AK8963_ST2       0x09  // Data overflow bit 3 and data read error status bit 2
#define MPU9250_REG_AK8963_CNTL      0x0A  // Power down (0000), single-measurement (0001), self-test (1000) and Fuse ROM (1111) modes on bits 3:0
#define MPU9250_REG_AK8963_ASTC      0x0C  // Self test control
#define MPU9250_REG_AK8963_I2CDIS    0x0F  // I2C disable
#define MPU9250_REG_AK8963_ASAX      0x10  // Fuse ROM x-axis sensitivity adjustment value
#define MPU9250_REG_AK8963_ASAY      0x11  // Fuse ROM y-axis sensitivity adjustment value
#define MPU9250_REG_AK8963_ASAZ      0x12  // Fuse ROM z-axis sensitivity adjustment value

#define MPU9250_REG_SMPLRT_DIV       0x19


#define MPU9250_REG_INT_PIN_CFG      0x37
#define MPU9250_REG_INT_ENABLE       0x38

#define MPU9250_REG_TEMP_OUT_H       0x41
#define MPU9250_REG_TEMP_OUT_L       0x42

#define MPU9250_REG_GYRO_XOUT_H      0x43
#define MPU9250_REG_GYRO_XOUT_L      0x44
#define MPU9250_REG_GYRO_YOUT_H      0x45
#define MPU9250_REG_GYRO_YOUT_L      0x46
#define MPU9250_REG_GYRO_ZOUT_H      0x47
#define MPU9250_REG_GYRO_ZOUT_L      0x48

#define MPU9250_REG_ACCEL_XOUT_H     0x3B
#define MPU9250_REG_ACCEL_XOUT_L     0x3C
#define MPU9250_REG_ACCEL_YOUT_H     0x3D
#define MPU9250_REG_ACCEL_YOUT_L     0x3E
#define MPU9250_REG_ACCEL_ZOUT_H     0x3F
#define MPU9250_REG_ACCEL_ZOUT_L     0x40

#define MPU9250_REG_SIGNAL_PATH_RESET  0x68
#define MPU9250_REG_MOT_DETECT_CTRL  0x69
#define MPU9250_REG_USER_CTRL        0x6A  // Bit 7 enable DMP, bit 3 reset DMP
#define MPU9250_REG_PWR_MGMT_1       0x6B // Device defaults to the SLEEP mode
#define MPU9250_REG_PWR_MGMT_2       0x6C

#define MPU9250_TWI_TIMEOUT 5000 
#define MPU9250_ADDRESS     0x68 