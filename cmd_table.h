#ifndef __CMD_TABLE_H__
#define __CMD_TABLE_H__
#include "kernel_func.h"

#define MAX_CMD_LENGTH 128
#define MAX_ARGS 10

void cmd_help(int argc, char *argv[]);
void cmd_echo(int argc, char *argv[]);
void cmd_clear(int argc, char *argv[]);
void cmd_info(int argc, char *argv[]);

// TABLE OF REGESTRATION FOR COMMANDS
command commands[] = {
    {"help", cmd_help, "show help info"},
    {"echo", cmd_echo, "retrun parameters"},
    {"clear", cmd_clear, "clear all indfo"},
    {"info", cmd_info, "view the system-info"},
    {NULL, NULL, NULL} 
};

extern command commands[];
#endif