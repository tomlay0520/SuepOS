extern void uart0_init(void);
extern void uart0_put_string(char *s);


void start_kernel(void)
{
    // kernel的中文是：内核
    uart0_init();
    uart0_put_string("Hello, SUEP!\n");
    uart0_put_string("suep@risc-v$ ");
    while(1);
}