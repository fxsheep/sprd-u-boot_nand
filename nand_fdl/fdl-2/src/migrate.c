#include "asm/arch/sci_types.h"
#include <common.h>
#include <stdarg.h>
#include <malloc.h>
#include <asm/arch/bits.h>
#include <asm/arch/chip_drv_config_extern.h>

char * version_string="fdl2";

int printf(const char *fmt, ...)
{
#if 1
	va_list args;
	unsigned int i;
	char printbuffer[CONFIG_SYS_PBSIZE];

	va_start(args, fmt);
	i = vsprintf(printbuffer, fmt, args);
	serial_puts(printbuffer);
	return i;
#endif
	return 0;
}
void hang(void)
{
	printf("###ERROR####Please RESET the board###\n");
}

#if defined (CONFIG_SYS_SC8800X_UART0)
#define UART_PHYS ARM_UART0_BASE
#elif defined (CONFIG_SYS_SC8800X_UART1)
#define UART_PHYS ARM_UART1_BASE
#elif defined (CONFIG_SYS_SC8800X_UART2)
#define UART_PHYS ARM_UART2_BASE
#elif defined (CONFIG_SYS_SC8800X_UART3)
#define UART_PHYS ARM_UART3_BASE
#else
#error "define CONFIG_SYS_SC88000X_UARTx in configs file to use UART driver"
#endif

#define __REG(x)     (*((volatile u32 *)(x)))

int board_init(void)
{
	return 0;
}

int vprintf(const char *fmt, va_list args)
{
	uint i;
	char printbuffer[CONFIG_SYS_PBSIZE];

	/* For this to work, printbuffer must be larger than
	 *	 * anything we ever want to print.
	 *		 */
	i = vsprintf(printbuffer, fmt, args);

	/* Print the string */
	serial_puts(printbuffer);
	return i;
}

void putc(const char c)
{
	serial_putc(c);
}

void puts(const char *str)
{
//	serial_puts(str);
}

int ctrlc(void)
{
	return 0;
}

int do_reset(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	return 0;
}

void hw_watchdog_reset(void)
{
}

uint32 SCI_GetTickCount(void)
{
	volatile uint32 tmp_tick1;
	volatile uint32 tmp_tick2;

	tmp_tick1 = SYSTEM_CURRENT_CLOCK;
	tmp_tick2 = SYSTEM_CURRENT_CLOCK;

	while (tmp_tick1 != tmp_tick2)
  {
	  tmp_tick1 = tmp_tick2;
	  tmp_tick2 = SYSTEM_CURRENT_CLOCK;
	  }

	return tmp_tick1;
}
