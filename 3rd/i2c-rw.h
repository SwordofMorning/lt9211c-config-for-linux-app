
#ifndef __I2C_RW_H__
#define __I2C_RW_H__
int i2c_check_funcs(int file);

int i2csend_addr8_value8(int file, char dev_addr, char reg_addr, char reg_value);
int i2crecv_addr8_value8(int file, char dev_addr, char reg_addr);
int i2csend_addr16_value8(int file, unsigned char dev_addr, unsigned short reg_addr, unsigned char reg_value);
unsigned char i2crecv_addr16_value8(int file, unsigned char dev_addr, unsigned short reg_addr);
int i2csend_addr16_value16(int file, unsigned char dev_addr, unsigned short reg_addr, unsigned short reg_value);
unsigned short i2crecv_addr16_value16(int file, unsigned char dev_addr, unsigned short reg_addr);
int i2crecv_addr8_valuex(int file, unsigned char dev_addr, unsigned char reg_addr, unsigned char *values, unsigned int read_cnt);
int i2csend_addr8_value0(int file, unsigned char dev_addr, unsigned char reg_addr);
int i2csend_addr8_valuex(int file, unsigned char dev_addr, unsigned char reg_addr, unsigned char *values, unsigned int value_cnt);

int open_i2c_dev(int i2cbus, char *filename, size_t size, int quiet);

#endif /* __I2C_RW_H__ */
