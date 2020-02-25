#include "api.h"
#include "api-asm.h"

#include <libopencm3/cm3/common.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/cm3/nvic.h>

#include "usart_put.h"
#include "f3_i2c_master_test.h"
static void my_delay_1(void)
{
	int i = 48e6 / 2 / 4;

	while (i > 0)
	{
		i--;
		__asm__("nop");
	}
}

//Set Commands
#define MYSLAVE_SET_REG 0x01
//GET commands
#define MYSLAVE_GET_ADD_RESULT 0x02
#define MYSLAVE_GET_SUB_RESULT 0x03
#define MYSLAVE_GET_MUL_RESULT 0x04

#define DATA_NUM 32

volatile uint8_t addr = 0;
volatile uint8_t read_p = 0;
volatile uint8_t useless_data = 0;
volatile uint8_t write_num = 0;
volatile uint8_t write_addr = 0;
volatile uint8_t reading = 0;
// volatile uint8_t Read_Buff[DATA_NUM] = {0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
volatile uint8_t ReadWrite_Buff[DATA_NUM] = {0x85, 0x56, 0x78, 0x92, 0x00, 0x00, 0x00, 0x00};
// volatile uint8_t tmp_buff[16];

void i2c2_isr(void)
{
	volatile uint32_t isr;
	isr = I2C_ISR(I2C2);
	// Address matched (Slave)
	if ((isr & I2C_ISR_ADDR) == I2C_ISR_ADDR)
	{
		//Clear the ADDR sequence by reading ICR.
		I2C_ICR(I2C2) |= I2C_CR1_ADDRIE;
		addr = i2c_get_data(I2C2);
	}
	else if ((isr & I2C_ISR_RXNE) == I2C_ISR_RXNE)
	{
		if (write_num == 0)
		{
			write_addr = i2c_get_data(I2C2);
		}
		if ((isr & I2C_ISR_DIR_READ) == I2C_ISR_DIR_WRITE)
		{
			if ((write_addr + write_num) >= 16 && (write_addr + write_num) < 32)
			{
				ReadWrite_Buff[write_addr + write_num] = i2c_get_data(I2C2);
				write_num++;
			}
			else
			{
				useless_data = i2c_get_data(I2C2);
			}
		}
	}
	else if ((isr & I2C_ISR_TXIS) == I2C_ISR_TXIS)
	{
		reading = i2c_get_data(I2C2) + read_p;
		if (reading < 32)
		{
			i2c_send_data(I2C2, ReadWrite_Buff[reading]);
			read_p++;
		}
		else
		{
			read_p = 0;
		}
		//send data to master in MSB order
	}
	else if ((isr & I2C_ISR_STOPF) == I2C_ISR_STOPF)
	{
		read_p = 0;
		write_num = 0;
		i2c_peripheral_disable(I2C2);
		if (I2C_CR1(I2C2) && 0x01 == 0)
		{
			i2c_peripheral_disable(I2C2);
		}
		else
		{
			i2c_peripheral_disable(I2C2);
		}
		i2c_peripheral_enable(I2C2);
	}
}

int main(void)
{
	int i;
	// rcc_clock_setup_in_hse_8mhz_out_48mhz();
	rcc_clock_setup_in_hsi_out_48mhz();

	usart1_init();
	// usart1_printf("M0 MCU Running .... \n");
	i2c2_slave_setup();
	while (1)
	{
		my_delay_1();
		usart1_printf("\n");
	}
}
