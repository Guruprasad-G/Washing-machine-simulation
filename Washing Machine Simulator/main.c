#include "main.h"

#pragma config WDTE = OFF

unsigned char operation_mode;
int wash_time, rinse_time, spin_time;
unsigned char reset_mode, program_no=0, water_level_index=0;
char *washing_prog[] = {"Daily","Heavy","Delicates","Whites","Stain Wash","EcoCottons","Woollens","BedSheets","Rinse+Dry","Dry only","Wash Only","Aqua store"};

char *water_level_arr[]={"Auto","Low","Medium","High","Max"};


unsigned char sec, min;
static void init_config(void){
   //CLCD Module
       init_clcd();
       
       //DKP
       init_digital_keypad();
       
       //TIMER2
       init_timer2();
       
       //FAN MODULE
       FAN_DDR = 0; //OUTPUT
       
       BUZZER_DDR = 0;
       
       PEIE = 1;
       GIE = 1;
}

void main(void) {
    unsigned char key =0x3F, key_copy;
    operation_mode = WASHING_PROGRAM_DISPLAY;
    reset_mode = WASHING_PROGRAM_DISPLAY_RESET;
    init_config();
    
    clcd_print(" Press Key5 To  ", LINE1(0));
    clcd_print("    Power ON    ", LINE2(0));
    clcd_print(" Washing Machine", LINE3(0));
    
   while( read_digital_keypad(STATE) != SW5)  //SW5 ->0x2F
   {
       for(int j=3000; j--; );
       
   }
    //clear screen
    clear_screen();
    
   // call power on screen 
   power_on_screen();
   
    
    while(1)    {
        key = read_digital_keypad(STATE); //SW4
         for(int j=3000; j--; );
        
         if(key == LSW4 && operation_mode == WASHING_PROGRAM_DISPLAY)
        {
             //Display water level screen
            operation_mode = WATER_LEVEL;
            reset_mode = WATER_LEVEL_RESET;
        }
        else if(key == LSW4 && operation_mode == WATER_LEVEL)
        {
            //set timing for washing programs
            set_time_for_program();
            
            clear_screen();
            //display required message
            clcd_print("Press Switch", LINE1(0));
            clcd_print(" SW5: START ", LINE2(0));
            clcd_print(" SW6: STOP ", LINE3(0));
            operation_mode = START_STOP_SCREEN;
        }
        else if(key == SW5 && operation_mode == PAUSE)
        {
            TMR2ON = ON; //START TIMER
            FAN = ON;
            operation_mode = START_PROGRAM;
        }
        
        switch(operation_mode)
        {
            
            case WASHING_PROGRAM_DISPLAY:
                 washing_program_display(key);
                 break;
            case WATER_LEVEL:
                water_level_display(key);
                break;
            case  START_STOP_SCREEN:
                if(key == SW5)
                {
                    operation_mode = START_PROGRAM;
                    reset_mode = START_PROGRAM_RESET;
                    continue;
                }
                else if(key == SW6)
                {
                    operation_mode = WASHING_PROGRAM_DISPLAY;
                    reset_mode = WASHING_PROGRAM_DISPLAY_RESET;
                    continue;
              
                }
                break;
            case START_PROGRAM:
                run_program(key);
                break;
                
        }
        reset_mode = RESET_NOTHING;
    }

}

void clear_screen(void)
{
     clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
     __delay_us(100);
}

void power_on_screen(void)
{
         //presenting blocks on LINE1
       for(unsigned char i=0; i<16; i++)
       {
            clcd_putch(BLOCK, LINE1(i));
            __delay_ms(100);
       }
       
       clcd_print("  POWERING ON   ",LINE2(0));
       clcd_print(" WASHING MACHINE",LINE3(0));
       
       //presenting blocks on LINE4
       for(unsigned char i=16; i>0; i--)
       {
         clcd_putch(BLOCK, LINE4(i));
         __delay_ms(100);
       }
       clcd_putch(BLOCK, LINE4(0));
       __delay_ms(500);  //3s delay

}
   
void washing_program_display(unsigned char key)
{
    if( reset_mode == WASHING_PROGRAM_DISPLAY_RESET) 
    {
        clear_screen();
        program_no = 0;
    } 
    if (key == SW4)
    {
       program_no++; 
       if(  program_no == 12) 
              program_no = 0;
             clear_screen();   
    }
    
    clcd_print("Washing Program", LINE1(0));
    clcd_putch('>', LINE2(0));
    //display washing program
    
    if(program_no <= 9)
    {
          clcd_print(washing_prog[program_no],LINE2(2));
          clcd_print(washing_prog[program_no + 1],LINE3(2));
          clcd_print(washing_prog[program_no + 2],LINE4(2));
    }
    else if(program_no == 10)
    {
          clcd_print(washing_prog[program_no],LINE2(2));
          clcd_print(washing_prog[program_no + 1],LINE3(2));
          clcd_print(washing_prog[0],LINE4(2));
    
    }
    else if(program_no == 11)
    {
          clcd_print(washing_prog[program_no], LINE2(2));
          clcd_print(washing_prog[0], LINE3(2));
          clcd_print(washing_prog[1], LINE4(2));
    }
}

void water_level_display(unsigned char key)
{
    if( reset_mode == WATER_LEVEL_RESET)
    {
        water_level_index = 0;
        clear_screen();
    }
    if(key == SW4)
    {
        water_level_index++;
        if(water_level_index == 5)
        {
            water_level_index = 0;
        }
        clear_screen();
        
    }
    
    clcd_print("Water Level", LINE1(0));
    clcd_putch('>', LINE2(0));
    if(water_level_index <= 2)
    {
        clcd_print(water_level_arr[water_level_index], LINE2(2));
        clcd_print(water_level_arr[water_level_index + 1], LINE3(2));
        clcd_print(water_level_arr[water_level_index + 2], LINE4(2));
    }
    else if(water_level_index == 3)
    {
        clcd_print(water_level_arr[water_level_index], LINE2(2));
        clcd_print(water_level_arr[water_level_index + 1], LINE3(2));
        clcd_print(water_level_arr[0],LINE4(2));
    }
     else if(water_level_index == 4)
    {
        clcd_print(water_level_arr[water_level_index], LINE2(2));
        clcd_print(water_level_arr[0], LINE3(2));
        clcd_print(water_level_arr[1], LINE4(2));
    }
}

void   set_time_for_program(void)
{
    switch(program_no)
    {
    case 0: //daily program
        switch(water_level_index)
        {

            case 1://low
                sec = 33;
                min = 0;
            break;
            case 0:
            case 2:
                sec = 41;
                min = 0;
                break;
             case 3:
             case 4:
                sec = 45;
                min = 0;
        }
        break;
     case 1://heavy
         switch(water_level_index)
         {
            
     case 1://LOW
         sec = 43;
         min = 0;
     case 0:
     case 2:
        sec = 50;
        min = 0;
        break;
     case 3:
     case 4:
        sec = 57;
        min = 0;
         }
         break;   
        case 2: //delicates
            switch(water_level_index)
            {
                
                case 1://Low
                case 0:
                case 2:
                    sec = 26;
                    min = 0;
                    break;
                case 3:
                case 4:
                sec = 31;
                min = 0;
            }
            break;
        case 3: //whites
            sec = 16;
            min =1;
            break;
        case 4: //stain wash
//96sec
            sec = 36;
            min = 1;
        case 5: //EcoCottons
            sec = 28;
            min = 0;
            break;
        case 6: //woolLens
            sec = 29;
            break;
        case 7: //BedSheets
            switch(water_level_index)
            {
                
                case 1: // low
                sec = 46;
                min = 0;
                case 0:
                case 2:
                    sec = 53;
                    min = 0;
                    break;
                case 3:
                case 4:
                    sec = 60;
                    min = 0;
            }
            break;
        case 8: //Rinse+Dry
            switch(water_level_index)
            {
                
                case 1: //low
                    sec = 18;
                    min = 0;
                case 0:
                case 2:
                case 3:
                case 4:
                    sec = 20;
                    min = 0;
            }
            break;
        case 9: //Dry only
            sec = 6;
            break;
        case 10: //wash only
        case 11: //Aqua store
            switch(water_level_index)
            {
                
                case 1: //low
                    sec = 16;
                    min = 0;
                case 0:
                case 2:
                    sec = 21;
                    min = 0;
                    break;
                case 3:
                case 4:
                    sec = 26;
                    min = 0;
            }
            break;
      }

}

void door_status_check(void)
{
    if(RB0 == 0) //DOOR OPEN CONDITION
    {
        //TURN ON BUZZER
        BUZZER = ON;
        FAN = OFF;
        TMR2ON = OFF;
        
        clear_screen();
        clcd_print("DOOR : OPEN", LINE1(0));
        clcd_print("Please Close", LINE3(0));
        
        // wait still door is closed
        while( RB0 == 0 )
        {
            ;
        }
        clear_screen();
        //CONTINUE THE WORK
        FAN = ON;
        TMR2ON = ON;
        
        BUZZER = OFF;
        clcd_print("Function -", LINE1(0));
        clcd_print("TIME", LINE2(0));
        clcd_print("5-START  6-PAUSE", LINE4(0));
    
    }
}

void run_program(unsigned char key)
{
    door_status_check();
    static int total_time, time;
        if(reset_mode == START_PROGRAM_RESET)
        {
            clear_screen();
            clcd_print("Prog: ",LINE1(0));
            clcd_print(washing_prog[program_no],LINE1(6));
            clcd_print("Time:",LINE2(0));
            //mm:ss
            //min--> 12
            clcd_putch((min/10)+'0',LINE2(6));
            clcd_putch((min%10)+'0',LINE2(7));
            clcd_putch(':',LINE2(8));
            clcd_putch((sec/10)+'0',LINE2(9));
            clcd_putch((sec%10)+'0',LINE2(10));
            clcd_print("(MM:SS)",LINE3(5));
            __delay_ms(2000);//2sec
            
            //display screen
            clear_screen();
            clcd_print("Function -",LINE1(0));
            clcd_print("TIME",LINE2(0));
            clcd_print("5-START  6-PAUSE", LINE4(0));
            
            //sec= 60 , min =0
            time = total_time = (min*60+sec);
            wash_time = (int)(total_time *0.4);
            rinse_time =(int)(total_time *0.12);
            spin_time = total_time - wash_time - rinse_time;

            //start the timer
            TMR2ON = 1;
            //start the fan
            FAN = 1;
          }
        
    if(key == SW6)//PAUSE THE MACHINE
    {
        TMR2ON = OFF; //TURN OFF TIMER
        FAN = OFF;
        operation_mode = PAUSE;
    }
    
    total_time = (min*60+sec);
    if(program_no <= 7)
    {
    if(total_time >= (time - wash_time)) 
    {
        clcd_print("Wash",LINE1(11));
    }
    else if(total_time >= (time - wash_time - rinse_time))
       {
           clcd_print("Rinse",LINE1(11));
       }
       else
           clcd_print("Spin ",LINE1(11));
    }
    else if(program_no == 8)  //rinse+dry
    {
        if(total_time >= (time - (0.40 * time)) )
        {
          clcd_print("Rinse",LINE1(11));  
        }
        else
           clcd_print("Spin ",LINE1(11));
    } 
    else if(program_no == 9) //dry only
    {
        clcd_print("Rinse",LINE1(11)); //aqua store
    }
    else
    {
        clcd_print("Wash",LINE1(11));
    }
    
    clcd_putch((min/10)+'0',LINE2(6));
    clcd_putch((min%10)+'0',LINE2(7));
    clcd_putch(':',LINE2(8));
    clcd_putch((sec/10)+'0',LINE2(9));
    clcd_putch((sec%10)+'0',LINE2(10));
            
    if(sec == 0 && min == 0)
    {
          //STOP THE TIMER
          TMR2ON = OFF;
          //TURN OFF FAN
          FAN =OFF;
          //TURN ON BUZZER
          BUZZER = ON;
                
          clear_screen();
          clcd_print("Prog. Completed", LINE1(0));
          clcd_print("Remove Cloths",LINE2(0));
          __delay_ms(2000);
          BUZZER = OFF;
          operation_mode = WASHING_PROGRAM_DISPLAY;
          reset_mode = WASHING_PROGRAM_DISPLAY_RESET;
          clear_screen();
            
    }
    
}