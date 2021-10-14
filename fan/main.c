/*
 * File:   main.c
 * Author: Anusha 
 *
 * Created on 11 October, 2021, 6:25 PM
 */


#include <xc.h>
#pragma config WDTE =OFF

#include "main.h"
static void init_config(void) {
    //initialization code 
    FAN_DDR=0;
    FAN= OFF;
    
}

void main(void) {
    init_config();
    while (1) 
    {
        FAN= ON;
        __delay_ms(2000);
        FAN= OFF;
        __delay_ms(3000);
    }

}
