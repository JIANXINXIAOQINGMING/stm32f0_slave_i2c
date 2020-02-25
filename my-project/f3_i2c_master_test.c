#include <errno.h>
#include <stdio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/common.h>
#include <libopencm3/cm3/nvic.h>

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "usart_put.h"

#include "f3_i2c_master_test.h"

void i2c2_master_setup(void)
{
	usart1_printf("l:%d\n\r", __LINE__);

	rcc_periph_clock_enable(RCC_I2C2);
	rcc_periph_clock_enable(RCC_GPIOB);
	usart1_printf("l:%d\n\r", __LINE__);
	rcc_set_i2c_clock_hsi(I2C2);

	usart1_printf("l:%d\n\r", __LINE__);

	i2c_reset(I2C2);
	/* Setup GPIO pin GPIO_USART2_TX/GPIO9 on GPIO port A for transmit. */
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10 | GPIO11);
	gpio_set_af(GPIOB, GPIO_AF1, GPIO10 | GPIO11);
	i2c_peripheral_disable(I2C2);
	//configure ANFOFF DNF[3:0] in CR1
	i2c_enable_analog_filter(I2C2);
	i2c_set_digital_filter(I2C2, 0);
	/* HSI is at 8Mhz */
	i2c_set_speed(I2C2, i2c_speed_sm_100k, 8);
	//configure No-Stretch CR1 (only relevant in slave mode)
	i2c_enable_stretching(I2C2);
	//addressing mode
	i2c_set_7bit_addr_mode(I2C2);
	i2c_peripheral_enable(I2C2);

	usart1_printf("l:%d\n\r", __LINE__);
}

void i2c2_slave_setup(void)
{
	volatile uint32_t regtmp;
	rcc_periph_clock_enable(RCC_I2C2);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_set_i2c_clock_hsi(I2C2);

	nvic_enable_irq(NVIC_I2C2_IRQ);

	i2c_reset(I2C2);
	/* Setup GPIO pin GPIO_USART2_TX/GPIO9 on GPIO port A for transmit. */
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10 | GPIO11);
	gpio_set_af(GPIOB, GPIO_AF1, GPIO10 | GPIO11);

	i2c_peripheral_disable(I2C2);
	//-----------------------------------------------------------------

	//configure ANFOFF DNF[3:0] in CR1
	i2c_enable_analog_filter(I2C2);
	i2c_set_digital_filter(I2C2, 0);
	/* HSI is at 8Mhz */
	i2c_set_speed(I2C2, i2c_speed_sm_100k, 8);
	//configure No-Stretch CR1 (only relevant in slave mode)
	i2c_enable_stretching(I2C2);
	//addressing mode

	//-----------------------------------------------------------------

	// regtmp = *(volatile uint32_t *)(0x40005808);
	// usart1_printf("OAR1=%x\n\r", regtmp);

	*(volatile uint32_t *)(0x40005808) = 0x00000;
	// regtmp = *(volatile uint32_t *)(0x40005808);
	// usart1_printf("OAR1=%x\n\r", regtmp);
#if 0 
	*(volatile uint32_t *)(0x40005808) |= (uint32_t)(0x53 << 1);
	regtmp = *(volatile uint32_t *)(0x40005808);
	usart1_printf("OAR1=%x\n\r", regtmp);
	*(volatile uint32_t *)(0x40005808) |= I2C_OAR1_OA1EN_ENABLE;
	regtmp = *(volatile uint32_t *)(0x40005808);
	usart1_printf("OAR1=%x\n\r", regtmp);

	regtmp = *(volatile uint32_t *)(0x40005808);
	usart1_printf("OAR1=%x\n\r", regtmp);

#endif
#define F0_I2C_SLAVE_ADDR 0x60

	*(volatile uint32_t *)(0x40005808) |= (uint32_t)(F0_I2C_SLAVE_ADDR << 1);
	// regtmp = *(volatile uint32_t *)(0x40005808);
	// usart1_printf("OAR1=%x\n\r", regtmp);

	usart1_printf("OAR1_W=%x\n\r", (0x8000 | (F0_I2C_SLAVE_ADDR << 1)));
	*(volatile uint32_t *)(0x40005808) = (0x8000 | (F0_I2C_SLAVE_ADDR << 1));

	// regtmp = *(volatile uint32_t *)(0x40005808);
	// usart1_printf("OAR1=%x\n\r", regtmp);
	//*(volatile uint32_t *)(0x40005808) = 0x8066;
	///regtmp = *(volatile uint32_t *)(0x40005808);
	//usart1_printf("OAR1=%x\n\r", regtmp);

	//*(volatile uint32_t *)(0x40005808) = 0x8066;
	///regtmp = *(volatile uint32_t *)(0x40005808);
	//usart1_printf("OAR1=%x\n\r", regtmp);

	I2C_TIMINGR(I2C2) = (uint32_t)0x00B00000;
	i2c_enable_interrupt(I2C2, I2C_CR1_GCEN |
								   //    I2C_CR1_ERRIE |
								   //    I2C_CR1_TCIE |
								   I2C_CR1_STOPIE |
								   //    I2C_CR1_NACKIE |
								   I2C_CR1_ADDRIE |
								   I2C_CR1_RXIE |
								   I2C_CR1_TXIE |
								   //    I2C_CR1_NOSTRETCH | //slave clock stretching
								   I2C_CR1_PE);
	i2c_peripheral_enable(I2C2);
}

void i2c2_master_test(void)
{

	uint8_t txbuf[3] = {0x00, 0x00, 0x00};
	uint8_t rxbuf[3] = {0x00, 0x00, 0x00};

	usart1_printf("l:%d\n\r", __LINE__);

	i2c_transfer7(I2C2, 0x50, txbuf, 1, rxbuf, 3);
	usart1_printf("l:%d\n\r", __LINE__);
	for (int i = 0; i < 3; i++)
	{
		usart1_printf("rxbuf[%d]=%x\n\r", i, rxbuf[i]);
	}

	usart1_printf("l:%d\n\r", __LINE__);
}