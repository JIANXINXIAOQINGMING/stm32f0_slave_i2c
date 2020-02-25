#ifndef F3_I2C_MASTER_TEST_H
#define F3_I2C_MASTER_TEST_H

#define MYSLAVE_ADDRESS 0x32

void i2c2_master_setup(void);
void i2c2_slave_setup(void);

void i2c2_master_test(void);

#endif