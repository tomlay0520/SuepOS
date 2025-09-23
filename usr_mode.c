#include "kernel_func.h"
extern void process_exit(void);

void test_task01(void){
        mini_printf("test01\n");
        delay(5000);
        process_give_up(); 
}

void test_task02(void){
    process_give_up();
        mini_printf("test02\n");
        delay(5000);
}

void test_task03(void){
        mini_printf("hello world!\n");
        delay(5000);
        process_give_up();
        mini_printf("hello world no.2 \n");
}