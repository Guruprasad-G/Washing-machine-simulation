/* 
 * File:   main.h
 * Author: Anusha
 *
 * Created on 11 October, 2021, 6:25 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#define _XTAL_FREQ  2000000
#define FAN         RC2
#define FAN_DDR     TRISC2

#define ON    1
#define OFF   0

void fan_on_and_off(void);
static void init_config_fan(void);

#endif	/* MAIN_H */

