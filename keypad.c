// ******************************************************************************************* //

#include "p24fj64ga002.h"
#include "keypad.h"

// ******************************************************************************************* //

void KeypadInitialize() {

    // TODO: Configure IOs and Change Notificaiton interrupt for keypad scanning. This
    // configuration should ensure that if any key is pressed, a change notification interrupt
    // will be generated.

    AD1PCFGbits.PCFG0 = 1; // digital configuration for the inputs, columns
    AD1PCFGbits.PCFG1 = 1;
    AD1PCFGbits.PCFG4 = 1;

    TRISBbits.TRISB11 = 0; //  row 1, output
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
    //    CNPU2bits.CN22PUE=1;//pull up resistor for rows
    //    CNPU2bits.CN21PUE=1;
    //    CNPU2bits.CN16PUE=1;
    //    CNPU1bits.CN15PUE=1;


    IFS1bits.CNIF = 0; // enable change notification interrupt
    IEC1bits.CNIE = 1;

    CNEN1bits.CN2IE = 1; // change notificatio enable for the columns, inputs
    CNEN1bits.CN3IE = 1;
    CNEN1bits.CN6IE = 1;



}

// ******************************************************************************************* //

char KeypadScan() {//function that returns a character and makes a scan
    char key = 0;//variable used to return character
    int i = 0;
    int pressed = 0;//variable that indicates how many times a button was pressed
    int hide = 0x0800;//bitmasking variables
    int mask = 0x0000;

    PR1 = 1151; //10 ms
    TMR1 = 0; //resets timer 1

    T1CONbits.TCKPS = 3;
    T1CONbits.TON = 1;//turns timer 1 on

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

    //        check if a key was pressed, set all rows to 0 first
    LATBbits.LATB11 = 0; //row1
    LATBbits.LATB10 = 0; //row 2
    LATBbits.LATB9 = 0; //row 3
    LATBbits.LATB8 = 0; //row 4


    if (PORTAbits.RA0 == 0)//if a key in first column was pressed pressed
    {
        if (PORTAbits.RA1 == 0 || PORTBbits.RB2 == 0)//if another key in another column was pressed
            key = -1;
        else
            key = 1;
    }
    if (PORTAbits.RA1 == 0)//if a key in first column was pressed pressed
    {
        if (PORTAbits.RA0 == 0 || PORTBbits.RB2 == 0)//if another key in another column was pressed
            key = -1;
        else
            key = 1;
    }
    if (PORTBbits.RB2 == 0)//if a key in first column was pressed pressed
    {
        if (PORTAbits.RA1 == 0 || PORTAbits.RA0 == 0)//if another key in another column was pressed
            key = -1;
        else
            key = 1;
    }

    if (PORTAbits.RA0 == 1 && PORTAbits.RA1 == 1 && PORTBbits.RB2 == 1)
        key = -1; // no key was pressed

    if (key == 1)//check which key was pressed
    {
        LATBbits.LATB11 = 0; //row1
        LATBbits.LATB10 = 1; //row 2
        LATBbits.LATB9 = 1; //row 3
        LATBbits.LATB8 = 1; //row 4

        TMR1 = 0;//resets timer1
        while (TMR1 < PR1);//causes a 10ms delay , to fix debouncing
        for (i = 1; i < 5; i++) {//loop used to check each row at a time


            if (i == 1 && PORTBbits.RB11 == 0 && PORTAbits.RA0 == 0) {//if the key in ROW 1 & column 1 was pressed

                pressed = pressed + 1;//mark the key as pressed
                key = '1';
            }
            if (i == 1 && PORTBbits.RB11 == 0 && PORTAbits.RA1 == 0) {//if the key in ROW 1 & column 2 was pressed
                pressed = pressed + 1;
                key = '2';
            }
            if (i == 1 && PORTBbits.RB11 == 0 && PORTBbits.RB2 == 0) {//if the key in ROW 1 & column 3 was pressed
                pressed = pressed + 1;
                key = '3';
            }

            if (i == 2 && PORTBbits.RB10 == 0 && PORTAbits.RA0 == 0) {//if the key in ROW 2 & column 1 was pressed
                pressed = pressed + 1;
                key = '4';
            }
            if (i == 2 && PORTBbits.RB10 == 0 && PORTAbits.RA1 == 0) {//if the key in ROW 2 & column 2 was pressed
                pressed = pressed + 1;
                key = '5';
            }
            if (i == 2 && PORTBbits.RB10 == 0 && PORTBbits.RB2 == 0) {//if the key in ROW 2 & column 3 was pressed
                pressed = pressed + 1;
                key = '6';
            }
            if (i == 3 && PORTBbits.RB9 == 0 && PORTAbits.RA0 == 0) {//if the key in ROW 3 & column 1 was pressed
                pressed = pressed + 1;
                key = '7';
            }
            if (i == 3 && PORTBbits.RB9 == 0 && PORTAbits.RA1 == 0) {//if the key in ROW 3 & column 2 was pressed
                pressed = pressed + 1;
                key = '8';
            }
            if (i == 3 && PORTBbits.RB9 == 0 && PORTBbits.RB2 == 0) {//if the key in ROW 3 & column 3 was pressed
                pressed = pressed + 1;
                key = '9';
            }

            if (i == 4 && PORTBbits.RB8 == 0 && PORTAbits.RA0 == 0) {//if the key in ROW 4 & column 1 was pressed
                pressed = pressed + 1;
                key = '*';
            }
            if (i == 4 && PORTBbits.RB8 == 0 && PORTAbits.RA1 == 0) {//if the key in ROW 4 & column 2 was pressed
                pressed = pressed + 1;
                key = '0';
            }

            if (i == 4 && PORTBbits.RB8 == 0 && PORTBbits.RB2 == 0) {//if the key in ROW 4 & column 3 was pressed
                pressed = pressed + 1;
                key = '#';
            }

            if (pressed > 1)//if more than 1 key was pressed
                key = -1;

                //bitmasking used to check each row
            mask = LATB;
            mask = mask & 0xF0FF;
            hide = hide >> 1;
            LATB = mask | ((~hide)&0x0F00);
        }
        LATBbits.LATB11 = 1; //row1
        LATBbits.LATB10 = 1; //row 2
        LATBbits.LATB9 = 1; //row 3
        LATBbits.LATB8 = 1; //row 4

    }

    return (key);//return key which can be a character or -1
}

// ******************************************************************************************* //




