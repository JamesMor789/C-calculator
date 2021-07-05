// Based of the HD44780U LCD display
// LCD is used in 4-bit mode //
// LCD accesses two GPIO sections; PORTA & PORTB
//PORTA is interfaced with E(Starts data read/write) & RS (Selects register)
//PORTB(PB4,PB5,PB6,PB7) is interfaced with DB4-DB7 (High order bidirectional pins for data transfer)

// Written By: James Morley
// 06/01/2021


// *** Pre-processor Directives ***
//PortA Registers
#define GPIO_PORTA_DATA_R       (*((volatile unsigned long *)0x400043FC))
#define GPIO_PORTA_DIR_R        (*((volatile unsigned long *)0x40004400))
#define GPIO_PORTA_AFSEL_R      (*((volatile unsigned long *)0x40004420))
//#define GPIO_PORTA_PUR_R       (*((volatile unsigned long *)0x40004510)) // Tina does not support internal resistors
#define GPIO_PORTA_DEN_R        (*((volatile unsigned long *)0x4000451C))
#define GPIO_PORTA_LOCK_R       (*((volatile unsigned long *)0x40004520))
#define GPIO_PORTA_CR_R         (*((volatile unsigned long *)0x40004524))
#define GPIO_PORTA_AMSEL_R      (*((volatile unsigned long *)0x40004528))
#define GPIO_PORTA_PCTL_R       (*((volatile unsigned long *)0x4000452C))
//PortA Individual Port specifics
//Base address = 0x40004000
//Offset = 4*(2)^n, where n is the bit position
//Individual port address= Base address + Offset
#define EN                 (*((volatile unsigned long *)0x40004010)) // Interfaces with PORT_A2
#define RS                 (*((volatile unsigned long *)0x40004020)) // Interfaces with PORT_A3

//PortB Registers
#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
//#define GPIO_PORTB_PUR_R        (*((volatile unsigned long *)0x40005510)) // Tina does not support internal resistors
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_LOCK_R       (*((volatile unsigned long *)0x40005520))
#define GPIO_PORTB_CR_R         (*((volatile unsigned long *)0x40005524))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))
//PortB Individual Port specifics
//Base address = 0x40005000
//Offset = 4*(2)^n, where n is the bit position
//Individual port address= Base address + Offset
#define PORT_DB                 (*((volatile unsigned long *)0x400053C0)) // Interfaces PB4-7  (1111 0000)
#define PORT_DB4                (*((volatile unsigned long *)0x40005040)) // Interfaces PB4 (0001 0000)
#define PORT_DB5                (*((volatile unsigned long *)0x40005080)) // Interfaces PB5 (0010 0000)
#define PORT_DB6                (*((volatile unsigned long *)0x40005100)) // Interfaces PB6 (0100 0000)
#define PORT_DB7                (*((volatile unsigned long *)0x40005200)) // Interfaces PB7 (1000 0000)

// GPIO Run Mode Clock Gating Control Register
#define SYSCTL_RCGCGPIO_R       (*((volatile unsigned long *)0x400FE608)) //Page 338 data sheet
// GPIO Peripheral Ready Register
#define SYSCTL_PRGPIO_R         (*((volatile unsigned long *)0x400FEA08)) //Page 404 data sheet
// GPIO Run Mode Clock Gating Control Register Port

#define GPIOA 0x01
#define GPIOB 0x02

// Delays
// Time/1.25x10^-8
#define DELAY450_NS 37

//Prototypes for time
void SysTick_Wait(unsigned long delay);
void SysTick_Init(void);
void PLL_Init(void);

// Initializes Port A and Port B
void PortAB_Init(void) {
  //Port A
  SYSCTL_RCGCGPIO_R |= GPIOA;     // 1) A clock
  while((SYSCTL_PRGPIO_R&0x01)==0){};             // Allows time for clock to stabilize
  GPIO_PORTA_LOCK_R = 0x4C4F434B;                 // 2) unlock PortA PA0
  GPIO_PORTA_CR_R = 0x0C;                         // allow changes to PA2 & PA3.(1100)
  GPIO_PORTA_AMSEL_R = 0x00;                      // 3) disable analog function
  GPIO_PORTA_PCTL_R = 0x00000000;                 // 4) GPIO clear bit PCTL
  GPIO_PORTA_DIR_R = 0x0C;                        // 5) PA2 & PA3 set to outputs
  GPIO_PORTA_AFSEL_R = 0x00;                      // 6) No alternate function
  GPIO_PORTA_DEN_R = 0x0C;                        // 7) Enable digital pins PA2-3

  //PORTB
  SYSCTL_RCGCGPIO_R |= GPIOB;     // 1) B clock
  while((SYSCTL_PRGPIO_R&0x02)==0){};             // Allows time for clock to stabilize
  GPIO_PORTB_LOCK_R = 0x4C4F434B;                 // 2) unlock PortE PA0
  GPIO_PORTB_CR_R = 0xF0;                         // allow changes to PB4,PB5,PB6,PB7 .(1111 0000)
  GPIO_PORTB_AMSEL_R = 0x00;                      // 3) disable analog function
  GPIO_PORTB_PCTL_R = 0x00000000;                 // 4) GPIO clear bit PCTL
  GPIO_PORTB_DIR_R = 0xF0;                        // 5) PB4,PB5,PB6,PB7 set to outputs
  GPIO_PORTB_AFSEL_R = 0x00;                      // 6) No alternate function
  GPIO_PORTB_DEN_R = 0xF0;                        // 7) Enable digital pins PB4,PB5,PB6,PB7
}

//Pulses the E pin
void Pulse_EN(void) {
  GPIO_PORTA_DATA_R |= 0x04; // Sets EN to high
  SysTick_Wait(DELAY450_NS); //450NS defined in HD55780 pg49 datasheet
  GPIO_PORTA_DATA_R &= ~0x04; //Sets EN to low
}

void Lcd_Half_Byte(unsigned char n1) {
    GPIO_PORTA_DATA_R &= ~0x08; //Command Register
    GPIO_PORTB_DATA_R = (n1 & 0xF0); //Masks the higher 4 bits, and sends the bits to the lcd.
    Pulse_EN();
}

void Lcd_Command(unsigned char n2) {
    GPIO_PORTA_DATA_R &= ~0x08; //Comand Register
    GPIO_PORTB_DATA_R = (n2 & 0xF0); //Masks the higher 4 bits, and sends the bits to the lcd.
    Pulse_EN();
    GPIO_PORTB_DATA_R = ((n2 & 0x0F)<<4); //Masks the lower 4 bits, and sends the bits to the lcd.
    Pulse_EN();
}

void Lcd_Data(unsigned char n3){
    GPIO_PORTA_DATA_R |= 0x08;  //Data register, set to high
    GPIO_PORTB_DATA_R = (n3 & 0xF0); //Masks the higher 4 bits, and sends the bits to the lcd.
  Pulse_EN();

  GPIO_PORTB_DATA_R = ((n3 <<4)& 0xF0); //Masks the lower 4 bits, and sends the bits to the lcd.
  Pulse_EN();
}

void Lcd_Init(){
  Lcd_Half_Byte(0x30); // LCD initilaization start (0011 0000) (48)
  Lcd_Half_Byte(0X30); // LCD initilaization start (0011 0000) (48)
  Lcd_Half_Byte(0X30); // LCD initilaization start (0011 0000) (48)
  Lcd_Half_Byte(0x20); // Function set, set interface to 4 bits long (0010 0000) (32)
  Lcd_Command(0x28); //  LCD 4 bit mode. 2 lines (N=1). 5X7 font (F=0) (1st. 0010 0000 2nd. 1000 0000)
  Lcd_Command(0x08); // Display Off (1st. 0000 0000 2nd. 1000 0000)
  Lcd_Command(0x01); // ClearDsiplay (1st. 0000 0000 2nd. 0001 0000)
  Lcd_Command(0x06); // Cursors increments to the right, screen does not shift (1st. 0000 0000 2nd. 0110 0000)
  Lcd_Command(0x0C); // Display on, no blinker and no cursosr (1st. 0000 0000 2nd. 1100 0000)
  Lcd_Command(0x80); // DRAM to 0x00
}

void Lcd_Clear(void) { //Clears the LCD
  Lcd_Command(0x01);
  Lcd_Command(0x02);
}

void LCD_Str(char *str) { // Allows the LCD to print strings.
  int i;
  for (i=0;str[i]!=0;i++){
    Lcd_Data(str[i]);
  }
}
