#ifndef __TYPES_H__
#define __TYPES_H__


typedef unsigned int  uint32_t;
typedef unsigned short uint16_t;

typedef unsigned long long uint64_t;
typedef unsigned char uint8_t;

typedef uint32_t ptr;
typedef uint32_t reg;

typedef struct command
{
    /* data */
    const char* name;
    void (*func)(int argc, char* argv[]);
    const char* help;
}command;



#endif 