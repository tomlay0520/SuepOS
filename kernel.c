extern void uart0_init(void);
extern void uart0_put_string(char *s);

void start_kernel(void)
{
    uart0_init();
    uart0_put_string("Hello, SUEP!\n");
    while(1);
}