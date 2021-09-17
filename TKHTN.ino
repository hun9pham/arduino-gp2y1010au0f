#include "GP2Y1010AU0F.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define DUST_DENSITY_MAX 500
#define DUST_DENSITY_MIN 0

typedef unsigned char uchar;
typedef unsigned long ulong;
typedef unsigned int uint;

/* * * * * * DECLARE GLOBAL VARIABLES * * * * * * * * */
ulong now =0;//Time Counter
String key_input;//Store the char input from keypad
uint THRESHOLD =DUST_DENSITY_MAX;//Value default 
//uint dust_sensor_read;//Read dust density from sensor GP2Y101AU0F
bool state = true;
/* * * * * * END DECLARE GLOBAL VARIABLES * * * * * * * * */

/* * * * * * DECLARE CONNECTION AND INIT SENSOR * * * * * * * * */
const int _GPY2Y1010AU0F_LED = 11; //V_Led
const int _GPY2Y1010AU0F_OUTPUT = A0; //Vo
GPY2Y1010AU0F Dust_Sensor(_GPY2Y1010AU0F_LED,_GPY2Y1010AU0F_OUTPUT);
void READ_GPY2Y1010AU0F(); //Get data from GPY2Y1010AU0F function
/* * * * * * END CONNECTION AND INIT SENSOR * * * * * * * * * */

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

/* * * * * * DECLARE CONNECTION AND INIT KEYPAD * * * * * * * * * */
char KEYPAD_ASCii[4][4]={
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'},
  };
uchar ROWS_PIN[4] ={10,9,8,7};//ROWS<0:3>
uchar COLS_PIN[4] ={6,5,4,3};//COLS<0:3>
void KEYPAD_INIT();
char KEYPAD_READ();
/* * * * * * END DECLARE CONNECTION AND INIT KEYPAD * * * * * * * * * */

/* * * * * * DECLARE PROTOTYPE * * * * * * * * * */
void ADJUST_THRESHOLD();//Modify Threshold from the Keypad
/* * * * * * END DECLARE PROTOTYPE * * * * * * * * * */


void setup() {
  // put your setup code here, to run once:
  lcd.init();//Init LCD 16x2
  lcd.backlight();// Turn on the blacklight and print a message.
  pinMode(12,OUTPUT);//Buzzer
  digitalWrite(12,LOW);//BUZZER OFF
  KEYPAD_INIT();//Set pin <6:9> as output, pin <5:2> as input
  key_input.reserve(3);

}

//int n=0;
void loop() {
  // put your main code here, to run repeatedly: 
  while(millis() - now >1000){
    float dust_sensor_read = Dust_Sensor.GET_DUST_DENSITY();//Read Sensor values Unit:mg/l
    char key = KEYPAD_SCAN();
    if(key){
      if(key=='A') ADJUST_THRESHOLD();
      else if(key=='D'){
        digitalWrite(12,LOW);
        state = false;
      }
      else if(key=='#'){
        digitalWrite(12,HIGH);//Check Buzzer Working
      }
    }
    lcd.setCursor(0,0);//Col 0 Row 0
    lcd.print("Thresh: ");
    lcd.print(THRESHOLD);
    lcd.print("mg/m3");
    lcd.setCursor(0,1);//Col 0 Row 1
    lcd.print("Dust:");
    lcd.print(dust_sensor_read);
    lcd.print(" mg/m3:");
    if(dust_sensor_read >= THRESHOLD && state ==true){
      _ISR();
    } 
    //n++;
    now = millis();
  }  
}

//External Interrupt 
void _ISR(){
  while(1){
      digitalWrite(12,HIGH);
      for(uint i =0;i<100;i++){
        char key = KEYPAD_SCAN();
        if(key=='D'){
          break;
        }
        delay(50);
        }
      break;
  }
  digitalWrite(12,LOW);        
  lcd.clear();
  digitalWrite(12,LOW);
  state = false;  
}

/* * * * * * KEYPAD FUNCTION * * * * * * * * */
void KEYPAD_INIT(){
  for(uchar i =0;i<4;i++){
    pinMode(ROWS_PIN[i],OUTPUT);
    digitalWrite(ROWS_PIN[i],HIGH);
  }
  for(uchar i =0;i<4;i++){
    pinMode(COLS_PIN[i],INPUT_PULLUP);
  }
}
char KEYPAD_SCAN(){
  ulong _time = 0;
  char key =0;
  for(uchar r=0;r<4;r++){
    digitalWrite(ROWS_PIN[r],LOW);
    for(uchar c=0;c<4;c++){
      if(digitalRead(COLS_PIN[c])==LOW){
        delay(10);
        while(digitalRead(COLS_PIN[c])==LOW);
        key = KEYPAD_ASCii[r][c];
        break;
      }
    }   
    digitalWrite(ROWS_PIN[r],HIGH);
  }
  return key;
}
/* * * * * * END KEYPAD FUNCTION * * * * * * * * */

/* * * * * * MODIFY THRESHOLD FROM THE KEYPAD FUNCTION * * * * * * * * */
void ADJUST_THRESHOLD(){
  unsigned char i=12;
  lcd.clear();
  NOTE:
    lcd.setCursor(0,0);//Col 0 Row 0
    lcd.print("Adjust Threshold");
    lcd.setCursor(0,1);//Col 0 Row 1
    lcd.print("InputNumber:");
  while(1){
    char key = KEYPAD_SCAN();
    if(key!=0){
      switch(key){
        case '1':        case '2':        case '3':        case '4':        case '5':        
        case '6':        case '7':        case '8':        case '9':        case '0':
          lcd.setCursor(0,1);//Col 0 Row 1
          lcd.print("InputNumber:");
          lcd.setCursor(i,1);//Col i Row 1
          lcd.print(key);
          key_input+=key;
          i++;
          break;
        case 'C'://Confirm Threshold
          if(key_input.length() > 0 && key_input.length() <4){
              THRESHOLD = key_input.toInt();//Convert String to Int, New THRESHOLD   
              if( THRESHOLD <=500){
                key_input ="";//Clear key_input  
                state = true;
                lcd.clear();
                lcd.setCursor(0,0);//Col 0 Row 0
                lcd.print("Valid Number");
                lcd.setCursor(0,1);//Col 0 Row 0
                lcd.print("Press B to back"); 
              } 
              else{
                lcd.clear();
                lcd.setCursor(0,0);//Col 0 Row 0
                lcd.print("Invalid Number"); 
                key_input ="";//Clear key_input
                lcd.setCursor(0,1);//Col 0 Row 1
                lcd.print("Input again");
                i=12;
                delay(1000);
                goto NOTE;  
              }
          }
          else{
              lcd.clear();
              lcd.setCursor(0,0);//Col 0 Row 0
              lcd.print("Invalid Number"); 
              key_input ="";//Clear key_input
              lcd.setCursor(0,1);//Col 0 Row 1
              lcd.print("Input again");
              delay(1000);
              i=12;
              goto NOTE;
          }
          break;
        case '*'://Clear Input
          key_input ="";
          lcd.clear();
          i=12;
          goto NOTE;
          break;
        case 'B'://Black to LCD Display value Sensor
          lcd.clear();
          goto EXIT_ADJUST;
          break;
      }
    } 
  }
  EXIT_ADJUST:;
}
/* * * * * * END MODIFY THRESHOLD FROM THE KEYPAD * * * * * * * * */
