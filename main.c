#define UART_BASE	0x09000000

int puts(const char *str)
{
	while (*str) {
		*((unsigned int *) UART_BASE) = *str++;
	}
	return 0;
}

void main(void)
{
	puts("Hello from adrianlshaw\n");
	while (1);
}
