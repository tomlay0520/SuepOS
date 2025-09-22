#include "kernel_func.h"

void test_task01(void){
    while(1) {
        mini_printf("test01\n");
        delay(5000);
        process_give_up(); 
    }
}

void test_task02(void){
    while(1) {
        mini_printf("test02\n");
        delay(5000);
        process_give_up();
    }
}

void test_task03(void){
    while(1) {
        mini_printf("hello world!\n");
        delay(5000);
        process_give_up();
    }
}