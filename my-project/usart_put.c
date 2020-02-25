#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/i2c.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "usart_put.h"

volatile uint32_t compl[11];

void usart1_init(void)
{
	/* Enable GPIOC clock for USARTs. */
	rcc_periph_clock_enable(RCC_GPIOA);
	/* Enable clocks for USART2. */
	rcc_periph_clock_enable(RCC_USART1);

	/* Setup GPIO pin GPIO8/9 on GPIO port C for LEDs. */
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO8 | GPIO9);
	/* Setup GPIO pins for USART2 transmit. */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);
	/* Setup USART1 TX pin as alternate function. */
	gpio_set_af(GPIOA, GPIO_AF1, GPIO9);

	/* Setup USART2 parameters. */
	usart_set_baudrate(USART1, 115200);
	usart_set_databits(USART1, 8);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_stopbits(USART1, USART_CR2_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(USART1);
}

static inline void usart_put_string(uint32_t usart, char *s, int len)
{
	int i;

	for (i = 0; i < len; i++)
	{
		usart_send_blocking(usart, s[i]);
	}
}

int usart1_printf(const char *fmt, ...)
{
	int i;
	char buf[MAX_PRINT_BUF_SIZE];
	va_list args;
	int n;

	va_start(args, fmt);		  //初始化参数指针
	n = vsprintf(buf, fmt, args); /*函数放回已经处理的字符串长度*/
	va_end(args);				  //与va_start 配对出现，处理ap指针

	i = vsprintf(buf, fmt, args);
	usart_put_string(USART1, buf, strlen(buf));

	return i;
}

void print_register(void)
{
	if (compl[0] == I2C2_CR1)
		;
	else
	{
		compl[0] = I2C2_CR1;
		usart1_printf("CR1=%x\n\r", compl[0]);
	}
	if (compl[1] == I2C2_CR2)
		;
	else
	{
		compl[1] = I2C2_CR2;
		usart1_printf("CR2=%x\n\r", compl[1]);
	}
	if (compl[2] == I2C2_OAR1)
		;
	else
	{
		compl[2] = I2C2_OAR1;
		usart1_printf("OAR1=%x\n\r", compl[2]);
	}
	if (compl[3] == I2C2_OAR2)
		;
	else
	{
		compl[3] = I2C2_OAR2;
		usart1_printf("OAR2=%x\n\r", compl[3]);
	}
	if (compl[4] == I2C2_TIMINGR)
		;
	else
	{
		compl[4] = I2C2_TIMINGR;
		usart1_printf("TIMINGR=%x\n\r", compl[4]);
	}
	if (compl[5] == I2C2_TIMEOUTR)
		;
	else
	{
		compl[5] = I2C2_TIMEOUTR;
		usart1_printf("TIMEOUTR=%x\n\r", compl[5]);
	}
	if (compl[6] == I2C2_ISR)
		;
	else
	{
		compl[6] = I2C2_ISR;
		usart1_printf("ISR=%x\n\r", compl[6]);
	}
	if (compl[7] == I2C2_ICR)
		;
	else
	{
		compl[7] = I2C2_ICR;
		usart1_printf("ICR=%x\n\r", compl[7]);
	}
	if (compl[8] == I2C2_PECR)
		;
	else
	{
		compl[8] = I2C2_PECR;
		usart1_printf("PECR=%x\n\r", compl[8]);
	}
	if (compl[9] == I2C2_RXDR)
		;
	else
	{
		compl[9] = I2C2_RXDR;
		usart1_printf("RXDR=%x\n\r", compl[9]);
	}
	if (compl[10] == I2C2_TXDR)
		;
	else
	{
		compl[10] = I2C2_TXDR;
		usart1_printf("TXDR=%x\n\r", compl[10]);
	}
}