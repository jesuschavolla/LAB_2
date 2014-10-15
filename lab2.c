// ******************************************************************************************* //
// Include file for PIC24FJ64GA002 microcontroller. This include file defines
// MACROS for special function registers (SFR) and control bits within those
// registers.

#include "p24fj64ga002.h"
#include <stdio.h>
#include "lcd.h"
#include "keypad.h"

// ******************************************************************************************* //
// Configuration bits for CONFIG1 settings. 
//
// Make sure "Configuration Bits set in code." option is checked in MPLAB.
//
// These settings are appropriate for debugging the PIC microcontroller. If you need to 
// program the PIC for standalone operation, change the COE_ON option to COE_OFF.

_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & 
		 BKBUG_ON & COE_ON & ICS_PGx1 & 
		 FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

// ******************************************************************************************* //
// Configuration bits for CONFIG2 settings.
// Make sure "Configuration Bits set in code." option is checked in MPLAB.

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
		 IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

// ******************************************************************************************* //

// Varible used to indicate that the current configuration of the keypad has been changed,
// and the KeypadScan() function needs to be called.

volatile char scanKeypad;
volatile unsigned int cnt;
// ******************************************************************************************* //
void discardarray(char discard[4]){
    int i=0;
     for(i=1; i <=3; i++){
         discard[i]='p';
     }
}
int main(void)
{
	char key;
        int count=0;//keeps track of how many times each key was pressed.
        int state;//keeps track of current state
        int doublecheck=0;
        int i=0;
        int j=0;
        int starpound=0;
        int passfilled = 1;
        int Good=0;
        
       char data[4][4]={{'1','2','3','4'},{'p','p','p','p'},{'p','p','p','p'},{'p','p','p','p'}};


      char temporary[4];
       PR2 =57599 ;//10 ms
	TMR2 = 0;//resets timer 1

	T2CONbits.TCKPS = 3;
	T2CONbits.TON = 1;


      TMR4 = 0;
      TMR5 = 0;
      PR4 = 0b1100000111111111 ;
      PR5 = 0b1;
      IFS1bits.T5IF = 0;
      IEC1bits.T5IE = 1;
      T4CONbits.T32 = 1;
      T4CONbits.TCKPS0 = 1;
      T4CONbits.TCKPS1 = 1;
       T4CONbits.TON = 1;

	// TODO: Initialize and configure IOs, LCD (using your code from Lab 1), 
	// UART (if desired for debugging), and any other configurations that are needed.
	
	LCDInitialize();
	KeypadInitialize();
	
	// TODO: Initialize scanKeypad variable.
     
       state=0;
	while(1)
	{
		// TODO: Once you create the correct keypad driver (Part 1 of the lab assignment), write
		// the C program that use both the keypad and LCD drivers to implement the 4-digit password system.

            if(state==0){//enter mode
                LCDMoveCursor(0,0);
                LCDPrintString("Enter");
                count=0;
                doublecheck=0;
                state=1;
            }
            if(scanKeypad==1)
            {
                key = KeypadScan();
                
                switch(state){

                  case 1://User Mode
			if( key != -1 && key!='*'&& key !='#' && doublecheck==0) {
                            if(count==0){
				LCDMoveCursor(1,count);
				LCDPrintChar(key);
                               temporary[count]=key;
                                count++;
                            }
                            else if (count==1){
                                LCDMoveCursor(1,count);
				LCDPrintChar(key);
                                temporary[count]=key;
                                count++;
                            }
                            else if (count==2){
                                LCDMoveCursor(1,count);
				LCDPrintChar(key);
                                temporary[count]=key;
                                count++;
                            }
                            else if (count==3){
                                LCDMoveCursor(1,count);
				LCDPrintChar(key);
                               temporary[count]=key;
                               count++;
                               
                               cnt=0;
                               TMR3=0;
                               TMR4=0;
                               while(cnt<1);
                               Good=0;
                               for(j=0;j<passfilled;j++)
                               {
                                   if(data[j][0]==temporary[0])
                                   { 
                                       if(data[j][1]==temporary[1])
                                           if (data[j][2]==temporary[2])
                                                  if (data[j][3]==temporary[3])
                                                  {
                                                      Good=1;
                                                  }
                                   } 
                               }
                               if(Good==1)
                                {
                                    LCDClear();
                                    LCDMoveCursor(0,0);
                                    
                                    LCDPrintString("Good");
                                    discardarray(temporary);
                                    cnt=0;
                                    TMR4 = 0;
                                    TMR5 = 0;
                                    
                                    while(cnt<1);//2 second delay;
                                    
                                    LCDClear();
                                    state=0;
                                    break;
                                }
                               else if(Good==0)
                                {
                                    LCDClear();
                                    LCDMoveCursor(0,0);
                                    LCDPrintString("Bad");
                                    discardarray(temporary);
                                    cnt=0;
                                    TMR4 = 0;
                                    TMR5= 0;
                                    while(cnt<1);//2 second delay;
                                    LCDClear();
                                    state=0;

                                    break;
                                }


                               
                         }   
                                
                            }

                        
                        else if( key != -1 && (key=='*'|| key =='#'))
                        {
                            
                                if(count==0 && key=='*')
                                {
                                     LCDMoveCursor(1,count);
                                     LCDPrintChar(key);
                                     doublecheck=1;
                                     count++;
                                     
                                }
                                else if(count==1 && doublecheck==1 && key== '*')
                                {
                                    LCDClear();
                                     count=0;
                                     doublecheck=0;
                                     state=2;
                                     break;
                                }
                                else if(count<2&& key=='#')
                                {
                                    LCDMoveCursor(1,count);
                                    LCDPrintChar(key);
                                        TMR2=0;
                               TMR3=0;
                               cnt=0;
                               while(cnt<1);
                                    LCDClear();
                                    LCDMoveCursor(0,0);
                                    LCDPrintString("Bad");
                                    discardarray(temporary);
                                    cnt=0;
                                    TMR4 = 0;
                                    TMR5= 0;
                                    while(cnt<1);//2 second delay;
                                    LCDClear();
                                    state=0;
                                    break;
                                }
                                else{
                                    LCDMoveCursor(1,count);
                                    LCDPrintChar(key);
                                        TMR2=0;
                               TMR3=0;
                               cnt=0;
                               while(cnt<1);
                                    LCDClear();
                                    LCDMoveCursor(0,0);
                                    LCDPrintString("Bad");
                                    cnt=0;
                                    TMR4 = 0;
                                    TMR5= 0;
                                    while(cnt<1);//2 second delay;
                                    LCDClear();
                                    state=0;
                                    break;
                                }
                        }
			scanKeypad = 0;
                    break;

                    case 2://Program mode
                        
                        LCDMoveCursor(0,0);
                        LCDPrintString("Set Mode");
                        state=3;
                        scanKeypad = 0;
                        break;
                        
                    case 3:

                        if( key != -1 && key!='*'&& key !='#') {
                            if(count==0){
				LCDMoveCursor(1,count);
				LCDPrintChar(key);
                               temporary[count]=key;
                                count++;
                            }
                            else if (count==1){
                                LCDMoveCursor(1,count);
				LCDPrintChar(key);
                                temporary[count]=key;
                                count++;
                            }
                            else if (count==2){
                                LCDMoveCursor(1,count);
				LCDPrintChar(key);
                                temporary[count]=key;
                                count++;
                            }
                            else if (count==3){
                                LCDMoveCursor(1,count);
				LCDPrintChar(key);
                               temporary[count]=key;
                               count++;
                         }
                             else if (count==4){
                                //invalid
                                    LCDMoveCursor(1,count);
                                    LCDPrintChar(key);
                                    LCDClear();
                                    LCDMoveCursor(0,0);
                                    discardarray(temporary);
                                    LCDPrintString("Invalid");
                                    cnt=0;
                                    TMR4 = 0;
                                    TMR5= 0;
                                    while(cnt<1);//2 second delay;
                                    LCDClear();
                                    count=0;
                                    starpound=0;
                                    state=0;
                                    break;

                             }

                        }


                        else if( key != -1 && (key=='*'|| key =='#'))
                        {

                                if(count<=3)
                                {
                                     LCDMoveCursor(1,count);
                                     LCDPrintChar(key);
                                     count++;
                                     starpound=1;
                                }

                                else if(count==4)
                                {
                                    if(key=='#')
                                    {
                                        if(starpound==0){//valid

                                            if(passfilled<4)
                                            { LCDClear();
                                             LCDMoveCursor(0,0);
                                             LCDPrintString("Valid");
                                             TMR4 = 0;
                                             TMR5= 0;
                                             cnt=0;
                                             while(cnt<1);//2 second delay;
                                            LCDClear();
                                            state=0;
                                            count=0;
                                            starpound=0;

                                            }
                                              if(data[passfilled][0] == 'p'){
                                                 for(i=0; i<=3; i++){
                                                     data[passfilled][i] = temporary[i];
                                                 }

                                             }

                                             else if(passfilled == 4) {
                                                 LCDClear();
                                                 LCDMoveCursor(0,0);
                                                 LCDPrintString("DATA");
                                                 LCDMoveCursor(1,0);
                                                 LCDPrintString("FULL");
                                                 TMR4 = 0;
                                                 TMR5= 0;
                                                 cnt=0;
                                                while(cnt<1);//2 second delay;
                                                LCDClear();
                                                state=0;
                                                count=0;
                                                starpound=0;
                                                break;

                                             }

                                             passfilled++;

                                            
                                            break;
                                        }
                                        else if(starpound==1){//invalid
                                             LCDClear();
                                             LCDMoveCursor(0,0);
                                             LCDPrintString("Invalid");
                                             TMR4 = 0;
                                             TMR5= 0;
                                             cnt=0;
                                            while(cnt<1);//2 second delay;
                                            LCDClear();

                                            state=0;
                                            count=0;
                                            starpound=0;
                                            break;
                                        }
                                    }
                                    else if(key=='*'){//Invalid
                                            LCDClear();
                                             LCDMoveCursor(0,0);
                                             LCDPrintString("Invalid");
                                             cnt=0;
                                             TMR4 = 0;
                                             TMR5= 0;
                                            while(cnt<1);//2 second delay;
                                            LCDClear();

                                            state=0;
                                            count=0;
                                            starpound=0;
                                            break;
                                    }
                                }
                        }
			scanKeypad = 0;
                        break;
                    
                       
                        
                }
            }
            
               
      
	}
	return 0;
}

// ******************************************************************************************* //
// Defines an interrupt service routine that will execute whenever any enable
// input change notifcation is detected.
// 
//     In place of _ISR and _ISRFAST, we can directy use __attribute__((interrupt))
//     to inform the compiler that this function is an interrupt. 
//
//     _CNInterrupt is a macro for specifying the interrupt for input change 
//     notification.
//
// The functionality defined in an interrupt should be a minimal as possible
// to ensure additional interrupts can be processed. 
void __attribute__((interrupt,auto_psv)) _CNInterrupt(void)
{	
	// TODO: Clear interrupt flag
	IFS1bits.CNIF = 0;
	
	// TODO: Detect if *any* key of the keypad is *pressed*, and update scanKeypad
	// variable to indicate keypad scanning process must be executed.
//        LATBbits.LATB11=0;//row1
//        LATBbits.LATB10=0;//row 2
//        LATBbits.LATB9=0;//row 3
//        LATBbits.LATB8=0;//row 4

        if (PORTAbits.RA0==0 || PORTAbits.RA1==0 || PORTBbits.RB2==0)
             scanKeypad=1;
        else
            scanKeypad=0;
}
void __attribute__((interrupt,auto_psv)) _T5Interrupt(void)
//void _ISR _T1Interrupt(void)
{      
	// Clear Timer 1 interrupt flag to allow another Timer 1 interrupt to occur.
	IFS1bits.T5IF = 0;
        cnt++;
        //cnt = (cnt<9)?(cnt+1):0;//counts the seconds

}


// ******************************************************************************************* //