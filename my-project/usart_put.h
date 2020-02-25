#ifndef USART_STDOUT_H
#define USART_STDOUT_H

#define MAX_PRINT_BUF_SIZE  64

void usart1_init(void);
int usart1_printf(const char *fmt, ...);


#endif
