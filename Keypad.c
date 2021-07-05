// Based on a 4x4 matrix keypad
// The keypad uses Pull Down Resistors
// KeyPad accesses two GPIO sections; PORTD & GPIO_PORTE
// The Keypad layout is shown below
//       col1  col2  col3  col4
// row1  (1)   (2)   (3)   (A)
// row2  (4)   (5)   (6)   (B)
// row3  (7)   (8)   (9)   (C)
// row4  (*)   (0)   (#)   (D)

// Written By: James Morley
// 06/01/2021

// *** Pre-processor Directives ***

//PortE Registers
//The rows on the keypad: Input
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
//#define GPIO_PORTE_PUR_R        (*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_LOCK_R       (*((volatile unsigned long *)0x40024520))
#define GPIO_PORTE_CR_R         (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
//PortE Individual Port specifics
//Base address = 0x40024000
//Offset = 4*(2)^n, where n is the bit position
//Individual port address= Base address + Offset
#define ROW_1                 (*((volatile unsigned long *)0x40024004)) // PE0
#define ROW_2                 (*((volatile unsigned long *)0x40024008)) // PE1
#define ROW_3                 (*((volatile unsigned long *)0x40024010)) // PE2
#define ROW_4                 (*((volatile unsigned long *)0x40024020)) // PE3

//PortD Registers
//The coloumns on the keypad: Output
#define GPIO_PORTD_DATA_R       (*((volatile unsigned long *)0x400073FC))
#define GPIO_PORTD_DIR_R        (*((volatile unsigned long *)0x40007400))
#define GPIO_PORTD_AFSEL_R      (*((volatile unsigned long *)0x40007420))
//#define GPIO_PORTD_PUR_R        (*((volatile unsigned long *)0x40007510))
#define GPIO_PORTD_DEN_R        (*((volatile unsigned long *)0x4000751C))
#define GPIO_PORTD_LOCK_R       (*((volatile unsigned long *)0x40007520))
#define GPIO_PORTD_CR_R         (*((volatile unsigned long *)0x40007524))
#define GPIO_PORTD_AMSEL_R      (*((volatile unsigned long *)0x40007528))
#define GPIO_PORTD_PCTL_R       (*((volatile unsigned long *)0x4000752C))
//PortD Individual Port specifics
//PortD Individual Port specifics
//Base address = 0x40007000
//Offset = 4*(2)^n, where n is the bit position
//Individual port address'= Base address + Offset
#define COL_1                 (*((volatile unsigned long *)0x40007004)) // PD0
#define COL_2                 (*((volatile unsigned long *)0x40007008)) // PD1
#define COL_3                 (*((volatile unsigned long *)0x40007010)) // PD2
#define COL_4                 (*((volatile unsigned long *)0x40007020)) // PD3

// GPIO Run Mode Clock Gating Control Register
#define SYSCTL_RCGCGPIO_R       (*((volatile unsigned long *)0x400FE608)) //Page 338 data sheet
// GPIO Peripheral Ready Register
#define SYSCTL_PRGPIO_R         (*((volatile unsigned long *)0x400FEA08)) //Page 404 data sheet
// GPIO Run Mode Clock Gating Control Register Port
#define GPIOE   0x10 //Enables and provides a clock for PORTE in run time.(0001 0000)
#define GPIOD   0x08 //Enables and provides a clock for PORTD in run time.(0000 1000)

//Prototypes
void Lcd_Clear(void);

// Initializes Port D and Port E pins for either output or Input
void PortED_Init(void) {
    //PortE
    // All pins are set to Input
    SYSCTL_RCGCGPIO_R |= GPIOE;     // 1) E clock
    while((SYSCTL_PRGPIO_R&0x10)==0){};             // Allows time for clock to stabilize
    GPIO_PORTE_LOCK_R = 0x4C4F434B;                 // 2) unlock PortE PE0
    GPIO_PORTE_CR_R = 0x0F;                         // allow changes to PE0-3
    GPIO_PORTE_AMSEL_R = 0x00;                      // 3) disable analog function
    GPIO_PORTE_PCTL_R = 0x00000000;                 // 4) GPIO clear bit PCTL
    GPIO_PORTE_DIR_R = 0x00;                        // 5) PE0,PE2,PE3,PE3 input
    GPIO_PORTE_AFSEL_R = 0x00;                      // 6) No alternate function
    GPIO_PORTE_DEN_R = 0x0F;                        // 7) Enable digital pins PE0-3

    //PortD
    // All pins are set to Output
    SYSCTL_RCGCGPIO_R |= GPIOD;     // 1) D clock
    while((SYSCTL_PRGPIO_R&0x08)==0){};             // Allows time for clock to stabilize
    GPIO_PORTD_LOCK_R = 0x4C4F434B;                 // 2) unlock PortD PD0
    GPIO_PORTD_CR_R = 0x0F;                         // allow changes to PD0-3
    GPIO_PORTD_AMSEL_R = 0x00;                      // 3) disable analog function
    GPIO_PORTD_PCTL_R = 0x00000000;                 // 4) GPIO clear bit PCTL
    GPIO_PORTD_DIR_R = 0x0F;                        // 5) PD0,PD1,PD2,PD3 Output
    GPIO_PORTD_AFSEL_R = 0x00;                      // 6) No alternate function
    GPIO_PORTD_DEN_R = 0x0F;                        // 7) Enable digital pins PD0-3
}


//Sets PD0 high, the rest low
void column1() {
  COL_1=0x01;
  COL_2=0x00;
  COL_3=0x00;
  COL_4=0x00;
}
//Sets PD1 high, the rest low
void column2() {
  COL_1=0x00;
  COL_2=0x02;
  COL_3=0x00;
  COL_4=0x00;
}
//Sets PD2 high, the rest low
void column3() {
  COL_1=0x00;
  COL_2=0x00;
  COL_3=0x04;
  COL_4=0x00;
}
//Sets PD3 high, the rest low
void column4() {
COL_1=0x00;
COL_2=0x00;
COL_3 =0x00;
COL_4=0x08;
}

// Fuction to read the KeyPad and return the input
// Due to the 4x4 keypad not having enough buttons, the following will be changed.
//       col1  col2  col3  col4 ->       col1  col2  col3  col4
// row1  (1)   (2)   (3)   (A)  -> row1  (1)   (2)   (3)   (+ or *)
// row2  (4)   (5)   (6)   (B)  -> row2  (4)   (5)   (6)   (- or /)
// row3  (7)   (8)   (9)   (C)  -> row3  (7)   (8)   (9)   (=)
// row4  (*)   (0)   (#)   (D)  -> row4  (alt)   (0)   (C)   (S)
// Where C=Clear & S=Shift
char KeyPad_Entry() {
  //Checking col1, against all rows
  column1();
  if (ROW_1 == 0x01) {return '1';}
  else if (ROW_2 == 0x02) {return '4';}
  else if (ROW_3 == 0x04) {return '7';}
  else if (ROW_4 == 0x08) {return 'x';} //No function currently associated with this

  //Checking col2, against all rows
  column2();
  if (ROW_1 == 0x01) {return '2';}
  else if (ROW_2 == 0x02) {return '5';}
  else if (ROW_3 == 0x04) {return '8';}
  else if (ROW_4 == 0x08) {return '0';}

  //Checking col3, agasint all rows
  column3();
  if (ROW_1 == 0x01) {return '3';}
  else if (ROW_2 == 0x02) {return '6';}
  else if (ROW_3 == 0x04) {return '9';}
  else if (ROW_4 == 0x08) {return 'C';}

  //Checking col4, against all rows
  column4();
  if (ROW_1 == 0x01 && ROW_4 == 0x08) {return '*';} // If shift is held, multiply
  else if (ROW_2 == 0x02 &&  ROW_4 == 0x08) {return '/';} // If shift is held, divide
  else if (ROW_1 == 0x01 && ROW_4 != 0x08) {return '+';} //If shift is not held, addition
  else if (ROW_2 == 0x02 && ROW_4 != 0x08) {return '-';} //If shit is not held, minus
  else if (ROW_3 == 0x04 && ROW_4 != 0x08) {return '=';} //If shift is not held, equals
  else {return 'e';} //If no button is pressed
}
