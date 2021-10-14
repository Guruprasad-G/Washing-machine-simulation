/*
 * File:   main.c
 * Author: Anusha 
 *
 * Created on 11 October, 2021, 5:46 PM
 */


#include <xc.h>
#pragma config WDTE =OFF
#include "main.h"
static void init_config(void) {
    //initialization code 
    BUZZER_DDR=0;
    BUZZER=OFF;
    
}

void main(void) {
    init_config();
    while (1) 
    {
        BUZZER=ON;
        __delay_ms(2000);
        BUZZER=OFF;
        __delay_ms(3000);
    }

}

