#include "cmd_table.h"
#include "kernel_func.h"


// all command should be registered in cmd_table.h
// DON'T MODIFY ANY CHAR OF PARSE & EXECUTE
int parse_command(char *line, char *argv[])
{
    int argc = 0;
    char *p = line;
    
    while (*p && argc < MAX_ARGS) {
        while (*p == ' ' || *p == '\t') {
            p++;
        }
        
        if (*p == '\0') break;
        
        argv[argc++] = p;
        
        while (*p && *p != ' ' && *p != '\t') {
            p++;
        }

        if (*p) {
            *p++ = '\0';
        }
    }
    
    return argc;
}

void execute_command(int argc, char *argv[])
{
    if (argc == 0) return;
    
    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcmp(argv[0], commands[i].name) == 0) {
            commands[i].func(argc, argv);
            return;
        }
    }
    
    uart0_put_string("command not found, please do resgistration.\n");
    uart0_put_string(argv[0]);
    uart0_put_string("\ninput 'help' to check all commands.\n");
}



// OF COURSE, ALL IMPLEMENTATION SHOULD BE WRITTEN HERE.
void cmd_help(int argc, char *argv[])
{
    uart0_put_string("you can input:\n");
    for (int i = 0; commands[i].name != NULL; i++) {
        uart0_put_string("  ");
        uart0_put_string(commands[i].name);
        uart0_put_string(" - ");
        uart0_put_string(commands[i].help);
        uart0_put_string("\n");
    }
}

void cmd_echo(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++) {
        uart0_put_string(argv[i]);
        if (i < argc - 1) uart0_put_string(" ");
    }
    uart0_put_string("\n");
}

void cmd_clear(int argc, char *argv[])
{
    uart0_put_string("\033[2J\033[H");  
}

void cmd_info(int argc, char *argv[])
{
    uart0_put_string("RISC-V SUEP OS Shell\n");
    uart0_put_string("BASED ON QEMU VIRT-MACHINE\n");
    uart0_put_string("ARCH: RV64\n");
}

void shell(void)
{
    char line[MAX_CMD_LENGTH];
    char *argv[MAX_ARGS];
    int argc;
    
    // 初始化UART
    uart0_init();
    
    uart0_put_string("\nRISC-V OS Shell v1.0\n");
    uart0_put_string("input 'help' to cheack all commands.\n");
    
    while (1) {
        uart0_put_string("kernel@SuepOS ");
        readline(line, MAX_CMD_LENGTH);
        
        argc = parse_command(line, argv);
        if (argc > 0) {
            execute_command(argc, argv);
        }
    }
}
