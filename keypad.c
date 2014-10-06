// ******************************************************************************************* //

#include "p24fj64ga002.h"
#include "keypad.h"

// ******************************************************************************************* //

void KeypadInitialize() {
	
	// TODO: Configure IOs and Change Notificaiton interrupt for keypad scanning. This 
	// configuration should ensure that if any key is pressed, a change notification interrupt 
	// will be generated.

    TRISBbits.TRISB11 = 0 //  row 1, output
    TRISBbits.TRISB10 = 0; // row 2 , output
    TRISBbits.TRISB9 = 0; // row 3 , output
    TRISBbits.TRISB8 = 0; // row 4 output

    ODCBbits.ODB11 = 1; // open drain configuration for rows
    ODCBbits.ODB10 = 1;
    ODCBbits.ODB9 = 1;
    ODCBbits.ODB8 = 1;

    TRISAbits.TRISA0 = 1; // column 1, input
    TRISAbits.TRISA1 = 1; // column 2 , input
    TRISBbits.TRISB2 = 1; // column 3 , input

    CNPU1bits.CN2PUE = 1; // pull up resistor for columns
    CNPU1bits.CN3PUE = 1;
    CNPU1bits.CN6PUE = 1;

    AD1PCFGbits.PCFG0 = 1; // digital configuration for the inputs, columns
    AD1PCFGbits.PCFG1 = 1;
    AD1PCFGbits.PCFG4 = 1;

    IFS1bits.CNIF = 0; // enable change notification interrupt
    IEC1bits.CNIE = 1;

    CNEN1bits.CN2IE = 1; // change notificatio enable for the columns, inputs
    CNEN1bits.CN3IE = 1;
    CNEN1bits.CN6IE = 1;



}

// ******************************************************************************************* //

char KeypadScan() {
	char key = -1;
        int scan = 0;
	
	
	// TODO: Implement the keypad scanning procedure to detect if exactly one button of the 
	// keypad is pressed. The function should return:
	//
	//      -1         : Return -1 if no keys are pressed.
	//      '0' - '9'  : Return the ASCII character '0' to '9' if one of the 
	//                   numeric (0 - 9) keys are pressed.
	//      '#'        : Return the ASCII character '#' if the # key is pressed. 
	//      '*'        : Return the ASCII character '*' if the * key is pressed. 
	//       -1        : Return -1 if more than one key is pressed simultaneously.
	// Notes: 
	//        1. Only valid inputs should be allowed by the user such that all invalid inputs 
	//           are ignored until a valid input is detected.
	//        2. The user must release all keys of the keypad before the following key press
	//           is processed. This is to prevent invalid keypress from being processed if the 
	//           users presses multiple keys simultaneously.
	//


	return key;
}
