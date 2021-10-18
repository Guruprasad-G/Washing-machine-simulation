#include <xc.h>
#include "digital_keypad.h"

void init_digital_keypad(void)
{
    /* Set Keypad Port as input */
    KEYPAD_PORT_DDR = KEYPAD_PORT_DDR | INPUT_LINES; //TRISB = TRISB | 0x3F
    
}

unsigned char read_digital_keypad(unsigned char mode)
{
    static unsigned char once = 1;
     static unsigned char pre_key;
     static unsigned char longpress;
    
    if (mode == LEVEL_DETECTION)
    {
        return KEYPAD_PORT & INPUT_LINES; //PORTB & 0x3F
    }
    else
    {
        if (((KEYPAD_PORT & INPUT_LINES) != ALL_RELEASED) && once)
        {
            once = 0;
            longpress=0;
            pre_key= KEYPAD_PORT & INPUT_LINES;
        }
               else if (!once && (pre_key == (KEYPAD_PORT & INPUT_LINES)) && longpress < 30)
        {
            longpress++;
            //clcd_putch(((longpress /100)+'0', LINE1(13)));
           // clcd_putch(((longpress /10)% 10) +  '0' , LINE4(14));
           // clcd_putch((longpress %10) +  '0', LINE4(15));
            
        }
        else if (longpress == 30)
        {
            longpress++;
            return 0x80 | pre_key;
        }
        else if ((KEYPAD_PORT & INPUT_LINES)== ALL_RELEASED && ! once)
        {
            once=1;
            if( longpress < 30)
            {
                return pre_key;
            }
        }
    }
    
    return ALL_RELEASED;
}

