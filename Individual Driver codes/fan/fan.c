/*
 * File:   main.c
 * Author: Anusha 
 *
 * Created on 11 October, 2021, 6:25 PM
 */


#include <xc.h>
#include "fan.h"
static void init_config_fan(void) 
{
    FAN_DDR = 0;
    FAN = OFF;
}

void fan_on_and_off(void) 
{
    FAN = ON;
    __delay_ms(3000);
    FAN = OFF;
}

