// ELEC3662 Mini Project
// Runs TM4C123GH6PM
// A calculator that can do basic operations

// Written by: James Morley
// 05/01/2021

// Pre-processor Directives
#include <stdio.h>
#include <stdlib.h>

//Function Prototypes

//lcd Prototypes
void Lcd_Init(); //Initiates and sets the LCD to four bit mode
void Lcd_Data(unsigned char n3); //Sends a character to the LCD
void PortAB_Init(void); // Initiates the ports to work with the LCD.
void Lcd_Clear(void); // Clears the LCD
void LCD_Str(char *str); //Allows the LCD to print strings

void PortED_Init(void); // Initiates the ports to work with the Keypad
char KeyPad_Entry(); // Runs throug cols and rows to check which buttons are pressed

//Prototypes for  time
void SysTick_Wait(unsigned long delay);
void SysTick_Init(void);
void PLL_Init(void);

//Prototypes for calculations
int KeyOne();// Inputs and displays 1st key
int KeyTwo();// Inputs and displays second pressed key
int CharConv(char ch);// Converts characters to integers
char Function();// Inputs and displays the operator
char Equals();// Decides to display simple calcaultion or display inline
void Animation();

//Declartion of variables
unsigned char KeyPressed;
int KeyReturn1;
int KeyReturn2;
int KeyReturn3;
char Operator;
int ans;
int final;
unsigned char operator;
unsigned char operator1;
int c;
int d;

int main(void)
{
    PLL_Init();
    SysTick_Init();
    PortAB_Init();
    Lcd_Init();
    PortED_Init();

   Animation();
    while(1){

        KeyOne();
        Function();
        KeyTwo();
        switch (operator){
          case '+': c=KeyReturn1+KeyReturn2;break;
          case '-': c=KeyReturn1-KeyReturn2;break;
          case '*': c=KeyReturn1*KeyReturn2;break;
          case '/': c=KeyReturn1/KeyReturn2;break;
        }
        Equals();
        Lcd_Clear();
        SysTick_Wait(50000);
    }
}

int KeyOne(){
    KeyPressed = KeyPad_Entry();
    if (KeyPressed =='e'|| KeyPressed == 'C'){ //If no key or clear is pressed, clear display
      Lcd_Clear();
    }
    else {
      Lcd_Data(KeyPressed);// Displays the key pressed
      KeyReturn1 = CharConv(KeyPressed); //Converts keypressed Char to an int
      SysTick_Wait(50000); //Wait
      return KeyReturn1;
    }
}

char Function() {
  operator = KeyPad_Entry();
  if (KeyPressed =='e'|| KeyPressed == 'C'){//If no key or clear is pressed, clear display
    Lcd_Clear();
  }
  else {
  Lcd_Data(operator); //Display the operator
  SysTick_Wait(50000); //Wait
  return operator;
  }
}

char Function1() {
  operator1 = KeyPad_Entry();
  if (KeyPressed =='e'|| KeyPressed == 'C'){
    Lcd_Clear();
  }
  else {
  Lcd_Data(operator1);
  SysTick_Wait(50000);
  return operator1;
  }
}

int KeyTwo() {
    KeyPressed = KeyPad_Entry();
    if (KeyPressed =='e'|| KeyPressed == 'C'){
      Lcd_Clear();
    }
    else {
       Lcd_Data(KeyPressed);
       KeyReturn2=CharConv(KeyPressed);
       SysTick_Wait(50000);
       return KeyReturn2;
    }
}

int KeyThree() {
  KeyPressed = KeyPad_Entry();
  if (KeyPressed =='e'|| KeyPressed == 'C'){
    Lcd_Clear();
  }
  else {
    Lcd_Data(KeyPressed);
    KeyReturn3=CharConv(KeyPressed);
    SysTick_Wait(50000);
    return KeyReturn3;
  }
}

char PrintFinal() {
  Lcd_Data('=');
  //SysTick_Wait(10000);
  char strr[12]; // Big buffer to prevent any overflow
  sprintf(strr, "%d",d);
  LCD_Str(strr);
  SysTick_Wait(50000);
}

char Equals() {
  KeyPressed = KeyPad_Entry();
  if (KeyPressed != '='){
   Function1();
   KeyThree();
   if (operator1 == '+') {d=c+KeyReturn3;}
   if (operator1 == '-') {d=c-KeyReturn3;}
   if (operator1 == '*' && operator == '+') {d=KeyReturn2*KeyReturn3+KeyReturn1;}
   if (operator1 == '*' && operator == '-') {d=KeyReturn1-(KeyReturn2*KeyReturn3);}
   if (operator1 == '*' && operator == '/') {d=KeyReturn1/KeyReturn2*KeyReturn3;}
   if (operator1 == '/' && operator == '+') {d=(KeyReturn2/KeyReturn3)+KeyReturn1;}
   if (operator1 == '/' && operator == '-') {d=KeyReturn1-(KeyReturn2/KeyReturn3);}
    if (operator1 == '/' && operator == '*') {d=KeyReturn1*KeyReturn2/KeyReturn3;}
   PrintFinal();
  }
    else {
      Lcd_Data(KeyPressed);
      char strr[12]; // Big buffer to prevent any overflow
      sprintf(strr, "%d",c);
      LCD_Str(strr);
     SysTick_Wait(50000);
    }
}

int CharConv(char ch){
  if ( ch == '1'){return 1;}
  else if( ch == '2'){return 2;}
  else if( ch == '3'){return 3;}
  else if( ch == '4'){return 4;}
  else if( ch == '5'){return 5;}
  else if( ch == '6'){return 6;}
  else if( ch == '7'){return 7;}
  else if( ch == '8'){return 8;}
  else if( ch == '9'){return 9;}

  else {
    return 0;
  }
}

void Animation() {
  Lcd_Data('E');
  SysTick_Wait(5000);
  Lcd_Data('L');
  SysTick_Wait(5000);
  Lcd_Data('E');
  SysTick_Wait(5000);
  Lcd_Data('C');
  SysTick_Wait(5000);
  Lcd_Data('3');
  SysTick_Wait(5000);
  Lcd_Data('6');
  SysTick_Wait(5000);
  Lcd_Data('6');
  SysTick_Wait(5000);
  Lcd_Data('2');
  SysTick_Wait(10000);
  Lcd_Clear();
  LCD_Str("WELCOME");
  SysTick_Wait(50000);
  Lcd_Clear();
}
