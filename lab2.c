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
         discard[i]='p'; //set values of array to 'p'
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
        int starpound=0;//keeps track of if star/pound is pressed
        int passfilled = 1;//keeps track of number of passwords in array
        int Good=0;//indicates validity of user-entered password
        
       char data[4][4]={{'1','2','3','4'},{'p','p','p','p'},{'p','p','p','p'},{'p','p','p','p'}};//initialize array with '1234' password and 3 other passwords with 'pppp'


      char temporary[4];
       PR2 =57599 ;//10 ms
	   TMR2 = 0;//resets timer 1

	T2CONbits.TCKPS = 3;//set prescale to 256
	T2CONbits.TON = 1; //turn on timer 2


      TMR4 = 0;//reset timer 4
      TMR5 = 0;//reset timer 5
	  //both the following lines create a 32-bit 2s delay
      PR4 = 0b1100000111111111 ;
      PR5 = 0b1;
      IFS1bits.T5IF = 0;//flag down
      IEC1bits.T5IE = 1;//enable on
      T4CONbits.T32 = 1;//set to 32-bit timer
	//set prescale to 256
      T4CONbits.TCKPS0 = 1;
      T4CONbits.TCKPS1 = 1;
       T4CONbits.TON = 1;//turn on timer

	// TODO: Initialize and configure IOs, LCD (using your code from Lab 1), 
	// UART (if desired for debugging), and any other configurations that are needed.
	
	LCDInitialize();
	KeypadInitialize();
	
	// TODO: Initialize scanKeypad variable.
     
       state=0;//initialize state
	while(1)
	{
		// TODO: Once you create the correct keypad driver (Part 1 of the lab assignment), write
		// the C program that use both the keypad and LCD drivers to implement the 4-digit password system.

            if(state==0){//enter mode
                LCDMoveCursor(0,0);
                LCDPrintString("Enter");//display enter
                count=0;//key position
                doublecheck=0;//number of * entered in a row
                state=1;//update state
            }
            if(scanKeypad==1)
            {
                key = KeypadScan();//receive input key value
                
                switch(state){

                  case 1://User Mode
			if( key != -1 && key!='*'&& key !='#' && doublecheck==0) {
			//if key valid and a number
                            if(count==0){//if key is in 1st position
								//print character in appropriate position
								LCDMoveCursor(1,count);
				LCDPrintChar(key);
                               temporary[count]=key;//update temporary array value
                                count++;//increment key position 
                            }
                            else if (count==1){ //if key is in 2nd position
								//print character in appropriate position
                                LCDMoveCursor(1,count);
				LCDPrintChar(key);
                                temporary[count]=key;//update temporary array value
                                count++;//inrement key position
                            }
                            else if (count==2){//if key is in 3rd position
								//print character in appropriate position
                                LCDMoveCursor(1,count);
				LCDPrintChar(key);
                                temporary[count]=key;//update temporary array value
                                count++;//inrement key position
                            }
                            else if (count==3){//if key is in 3rd position
								//print character in appropriate position
                                LCDMoveCursor(1,count);
				LCDPrintChar(key);
                               temporary[count]=key;//update temporary array value
                               count++;//inrement key position
                               
                               cnt=0;
							//reset 32-bit timer
                               TMR3=0;
                               TMR4=0;
                               while(cnt<1);//2 second delay
                               Good=0;
                               for(j=0;j<passfilled;j++)//check filled passwords
                               {
								//if user input matched stored password
                                   if(data[j][0]==temporary[0])
                                   { 
                                       if(data[j][1]==temporary[1])
                                           if (data[j][2]==temporary[2])
                                                  if (data[j][3]==temporary[3])
                                                  {
                                                      Good=1;//passwords match
                                                  }
                                   } 
                               }
                               if(Good==1)//if passwords match
                                {
									//display 'Good'
                                    LCDClear();
                                    LCDMoveCursor(0,0);
                                    
                                    LCDPrintString("Good");
									//clear user-entered array
                                    discardarray(temporary);
                                    cnt=0;
									//reset timer
                                    TMR4 = 0;
                                    TMR5 = 0;
                                    
                                    while(cnt<1);//2 second delay;
                                    
                                    LCDClear();//clear display
                                    state=0;//await user input
                                    break;
                                }
                               else if(Good==0)//if passwords don't match
                                {
									//display 'Bad'
                                    LCDClear();
                                    LCDMoveCursor(0,0);
                                    LCDPrintString("Bad");
                                    discardarray(temporary);//clear user-input array
                                    cnt=0;//reset cnt
									//reset timer
                                    TMR4 = 0;
                                    TMR5= 0;
                                    while(cnt<1);//2 second delay;
                                    LCDClear();//clear user display
                                    state=0;//await user input

                                    break;
                                }


                               
                         }   
                                
                            }

                        
                        else if( key != -1 && (key=='*'|| key =='#'))//if user entered key is a * or #
                        {
                            
                                if(count==0 && key=='*')//if 1st position is *
                                {
									//display character
                                     LCDMoveCursor(1,count);
                                     LCDPrintChar(key);
                                     doublecheck=1;//increment * count
                                     count++;//increment key position
                                     
                                }
                                else if(count==1 && doublecheck==1 && key== '*')//if 2 * entered in a row
                                {
                                    LCDClear();//clear display
                                     count=0;//reset key position
                                     doublecheck=0;//reset * count
                                     state=2;//exit to state 2
                                     break;
                                }
                                else if(count<2&& key=='#')//if # entered as character 1 or 2
                                {
									//display character
                                    LCDMoveCursor(1,count);
                                    LCDPrintChar(key);
									//reset timer
                                    TMR2=0;
                              		TMR3=0;
                               		cnt=0;//reset cnt
                               while(cnt<1);//2s delay
							   	//display 'Bad'
                                    LCDClear();
                                    LCDMoveCursor(0,0);
                                    LCDPrintString("Bad");
                                    discardarray(temporary);//clear user entered array
                                    cnt=0;//reset cnt
									//reset timer
                                    TMR4 = 0;
                                    TMR5= 0;
                                    while(cnt<1);//2 second delay;
                                    LCDClear();//clear display
                                    state=0;//await user input
                                    break;
                                }
                                else{
									//display key
                                    LCDMoveCursor(1,count);
                                    LCDPrintChar(key);
										//reset timer
                                        TMR2=0;
                              		    TMR3=0;
                               			cnt=0;//reset cnt
                              	 while(cnt<1);//2s delay
									//display 'Bad'
                                    LCDClear();
                                    LCDMoveCursor(0,0);
                                    LCDPrintString("Bad");
                                    cnt=0;//reset cnt
									//reset timer
                                    TMR4 = 0;
                                    TMR5= 0;
                                    while(cnt<1);//2 second delay;
                                    LCDClear();//clear display
                                    state=0;//await user input
                                    break;
                                }
                        }
			scanKeypad = 0;
                    break;

                    case 2://Program mode
                        //display 'Set Mode'
                        LCDMoveCursor(0,0);
                        LCDPrintString("Set Mode");
                        state=3;
                        scanKeypad = 0;
                        break;
                        
                    case 3:

                        if( key != -1 && key!='*'&& key !='#') { //if key entered in a number
                            if(count==0){//key is 1st position
								//display key
								LCDMoveCursor(1,count);
				LCDPrintChar(key);
                               temporary[count]=key;//alter array to include key
                                count++;//increment key position
                            }
                            else if (count==1){//key is 2nd position
								//display key
                                LCDMoveCursor(1,count);
				LCDPrintChar(key);
                                temporary[count]=key;//alter array to include key
                                count++;//increment key position
                            }
                            else if (count==2){//key is 3rd position
								//display key
                                LCDMoveCursor(1,count);
				LCDPrintChar(key);
                                temporary[count]=key;//alter array to include key
                                count++;//increment key position
                            }
                            else if (count==3){//key is 4th position
								//display key
                                LCDMoveCursor(1,count);
				LCDPrintChar(key);
                               temporary[count]=key;//alter array to include key
                               count++;//increment key position
                         }
                             else if (count==4){
                                //invalid
                                    LCDMoveCursor(1,count);
                                    LCDPrintChar(key);
                                    LCDClear();
                                    LCDMoveCursor(0,0);
                                    discardarray(temporary);
                                    LCDPrintString("Invalid");
                                    cnt=0;//reset cnt
									//reset timer
                                    TMR4 = 0;
                                    TMR5= 0;
                                    while(cnt<1);//2 second delay;
                                    LCDClear();//clear display
                                    count=0;//reset key position
                                    starpound=0;
                                    state=0;
                                    break;

                             }

                        }


                        else if( key != -1 && (key=='*'|| key =='#'))//key is * or #
                        {

                                if(count<=3)//key position is 1-4
                                {
									//display key
                                     LCDMoveCursor(1,count);
                                     LCDPrintChar(key);
                                     count++;//increment key position
                                     starpound=1;//update starpound
                                }

                                else if(count==4)//key position is 5
                                {
                                    if(key=='#')//key is #
                                    {
                                        if(starpound==0){//valid

                                            if(passfilled<4)//data array has open position
                                            { LCDClear();
                                             LCDMoveCursor(0,0);
                                             LCDPrintString("Valid");
											//reset timer
                                             TMR4 = 0;
                                             TMR5= 0;
                                             cnt=0;//reset cnt
                                             while(cnt<1);//2 second delay;
                                            LCDClear();//clear display
                                            state=0;//await user input
                                            count=0;//key position to 0
                                            starpound=0;//reset starpound

                                            }
                                              if(data[passfilled][0] == 'p'){//if array slot open
                                                 for(i=0; i<=3; i++){
                                                     data[passfilled][i] = temporary[i];//update with user entered key
                                                 }

                                             }

                                             else if(passfilled == 4) {//data array full
                                                 LCDClear();
                                                 LCDMoveCursor(0,0);
                                                 LCDPrintString("DATA");
                                                 LCDMoveCursor(1,0);
                                                 LCDPrintString("FULL");
												//reset timer
                                                 TMR4 = 0;
                                                 TMR5= 0;
                                                 cnt=0;//reset cnt
                                                while(cnt<1);//2 second delay;
                                                LCDClear();//clear display
                                                state=0;//await user input
                                                count=0;//key position to 0
                                                starpound=0;//reset starpound
                                                break;

                                             }

                                             passfilled++;//increment passfilled to next line in data array

                                            
                                            break;
                                        }
                                        else if(starpound==1){//invalid
                                             LCDClear();
                                             LCDMoveCursor(0,0);
                                             LCDPrintString("Invalid");
											//reset timer
                                             TMR4 = 0;
                                             TMR5= 0;
                                             cnt=0;//reset cnt
                                            while(cnt<1);//2 second delay;
											//clear display
                                            LCDClear();

                                            state=0;//await key press
                                            count=0;//key position to 0
                                            starpound=0;
                                            break;
                                        }
                                    }
                                    else if(key=='*'){//Invalid
                                            LCDClear();
                                             LCDMoveCursor(0,0);
                                             LCDPrintString("Invalid");
                                             cnt=0;//reset cnt
											//reset timer
                                             TMR4 = 0;
                                             TMR5= 0;
                                            while(cnt<1);//2 second delay;
                                            LCDClear();//clear display

                                            state=0;//await key press
                                            count=0;//key position to 0
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