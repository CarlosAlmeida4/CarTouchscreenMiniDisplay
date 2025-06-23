#include "bsp_i2c.h"
#include "hardware/i2c.h"

#define I2C_TIMEOUT_US 500000 

void bsp_i2c_write(uint8_t device_addr, uint8_t *buffer, size_t len)
{
//    i2c_write_blocking(BSP_I2C_NUM, device_addr, buffer, len, false);
    i2c_write_timeout_us(BSP_I2C_NUM,device_addr,buffer,len,false,I2C_TIMEOUT_US);
}

void bsp_i2c_write_reg8(uint8_t device_addr, uint8_t reg_addr, uint8_t *buffer, size_t len)
{
    uint8_t write_buffer[len + 1];
    write_buffer[0] = reg_addr;
    memcpy(write_buffer + 1, buffer, len);
    //i2c_write_blocking(i2c1, device_addr, write_buffer, len + 1, false);
    i2c_write_timeout_us(BSP_I2C_NUM,device_addr,write_buffer,len + 1,false,I2C_TIMEOUT_US);
}

void bsp_i2c_read_reg8(uint8_t device_addr, uint8_t reg_addr, uint8_t *buffer, size_t len)
{
    i2c_write_timeout_us(BSP_I2C_NUM,device_addr,&reg_addr,1,true,I2C_TIMEOUT_US);
    //i2c_write_blocking(BSP_I2C_NUM, device_addr, &reg_addr, 1, true);
    i2c_read_blocking(BSP_I2C_NUM, device_addr, buffer, len, false);
}

void bsp_i2c_write_reg16(uint8_t device_addr, uint16_t reg_addr, uint8_t *buffer, size_t len)
{
    uint8_t write_buffer[len + 2];
    write_buffer[0] = (uint8_t)(reg_addr >> 8);
    write_buffer[1] = (uint8_t)(reg_addr);
    memcpy(write_buffer + 2, buffer, len);
    i2c_write_timeout_us(BSP_I2C_NUM,device_addr,write_buffer,len + 2,false,I2C_TIMEOUT_US);
    //i2c_write_blocking(BSP_I2C_NUM, device_addr, write_buffer, len + 2, false);
}

void bsp_i2c_read_reg16(uint8_t device_addr, uint16_t reg_addr, uint8_t *buffer, size_t len)
{
    uint8_t write_buffer[2];
    write_buffer[0] = (uint8_t)(reg_addr >> 8);
    write_buffer[1] = (uint8_t)(reg_addr);

    i2c_write_timeout_us(BSP_I2C_NUM,device_addr,write_buffer,2,true,I2C_TIMEOUT_US);
    //i2c_write_blocking(BSP_I2C_NUM, device_addr, write_buffer, 2, true);
    i2c_read_blocking(BSP_I2C_NUM, device_addr, buffer, len, false);
}

void bsp_i2c_init(void)
{

    i2c_init(BSP_I2C_NUM, 100 * 1000);
    gpio_set_function(BSP_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(BSP_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(BSP_I2C_SDA_PIN);
    gpio_pull_up(BSP_I2C_SCL_PIN);
}