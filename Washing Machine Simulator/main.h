/* 
 * File:   main.h
 * Author: carol sanjana p
 *
 * Created on 14 October, 2021, 5:10 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>
#include"clcd.h"
#include "digital_keypad.h"
#include "timers_2.h"

#define WASHING_PROGRAM_DISPLAY       0X01
#define WATER_LEVEL                   0X02
#define START_STOP_SCREEN             0X03
#define START_PROGRAM                 0X04
#define PAUSE                         0X05




#define WASHING_PROGRAM_DISPLAY_RESET 0X10
#define WATER_LEVEL_RESET             0x11
#define START_PROGRAM_RESET           0x12
#define RESET_NOTHING                 0X00


#define  FAN                           RC2
#define BUZZER                         RC1
#define BUZZER_DDR                     TRISC1
#define  FAN_DDR                       TRISC2
#define ON                              1
#define OFF                             0


void power_on_screen(void);
void clear_screen(void);
void  washing_program_display(unsigned char key);
void  water_level_display(unsigned char key);
void set_time_for_program(void);
void run_program(unsigned char key);

#endif	/* MAIN_H */

