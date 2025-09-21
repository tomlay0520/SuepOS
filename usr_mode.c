#include "kernel_func.h"

void test_task01(void){
    mini_printf("test01\n");
    delay(5000);
    process_give_up();
}

void test_task02(void){
    mini_printf("test02\n");
    delay(5000);
    process_give_up();
}