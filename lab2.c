// ******************************************************************************************* //
// Include file for PIC24FJ64GA002 microcontroller. This include file defines
// MACROS for special function registers (SFR) and control bits within those
// registers.

#include "p24fj64ga002.h"
#include <stdio.h>
#include "lcd.h"
//#include "keypad.h"

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
volatile unsigned char cnt;
// ******************************************************************************************* //
int main(void)
{
	char key;
        int count=0;//keeps track of how many times each key was pressed.
        int state;//keeps track of current state
        int doublecheck=0;
        int i=0;
        
       char data[4][4]= {'1','2','3','4','0','0','0','0','0','0','0','0','0','0','0','0'};
      char temporary[4];

        PR1 = 57599;//1 second delay
	TMR1 = 0;//resets timer 1
	IFS0bits.T1IF = 0;
	IEC0bits.T1IE = 1;
	T1CONbits.TCKPS = 3;
	T1CONbits.TON = 1;


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
                               temporary[count]='key';
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
                            else if(count==4)//after password was made, determine if its good
                            {
                                for(i=0;i<=3;i++){
                                    if(data[0][i]!=temporary[i]){
                                    LCDClear();
                                    LCDMoveCursor(0,0);
                                    LCDPrintString("Bad");
                                    cnt=0;
                                    state=0;
                                    while(cnt<3);//2 second delay;
                                    LCDClear();
                                    break;
                                    }

                                    if(data[3]==temporary[3]){
                                        LCDClear();
                                    LCDMoveCursor(0,0);
                                    LCDPrintString("Good");
                                    cnt=0;
                                    while(cnt<3);//2 second delay;
                                    LCDClear();
                                }

                                }
                                
                            }

                        }
                        else if( key != -1 && (key=='*'|| key =='#'))
                        {
                            
                                if(count==0 && key=='*')
                                {
                                     LCDMoveCursor(1,count);
                                     LCDPrintChar(key);
                                     count++;
                                     doublecheck==1;
                                }
                                else if(count==1 && key== '*')
                                {
                                     LCDMoveCursor(1,count);
                                     LCDPrintChar(key);
                                     count=0;
                                     doublecheck=0;
                                     state=2;
                                     break;
                                }
                                else if(count<2&& key=='#')
                                {
                                    LCDClear();
                                    LCDMoveCursor(0,0);
                                    LCDPrintString("Bad");
                                    cnt=0;
                                    while(cnt<3);//2 second delay;
                                    LCDClear();
                                    state=0;
                                    break;
                                }
                                else{
                                    LCDClear();
                                    LCDMoveCursor(0,0);
                                    LCDPrintString("Bad");
                                    cnt=0;
                                    while(cnt<3);//2 second delay;
                                    LCDClear();
                                    state=0;
                                    break;
                                }
                        }
			scanKeypad = 0;
                    break;

                    case 2://Program mode
                        LCDClear();
                        LCDMoveCursor(0,0);
                        LCDPrintString("Set Mode");
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
void __attribute__((interrupt)) _CNInterrupt(void)
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
void __attribute__((interrupt,auto_psv)) _T1Interrupt(void)
//void _ISR _T1Interrupt(void)

{
	// Clear Timer 1 interrupt flag to allow another Timer 1 interrupt to occur.
	IFS0bits.T1IF = 0;
        
        cnt = (cnt<9)?(cnt+1):0;//counts the seconds

}


// ******************************************************************************************* //