#include <LiquidCrystal.h>

//Pins:

const int trigPin = 13;
const int echoPin = 12;
const int buzzerpin = 3;
const int ledpin = 2;
LiquidCrystal lcd(11, 10, 7, 6, 5, 4); // RS, EN, D4, D5, D6, D7

//These variables are for activation duration for buzzer and LED

float Buzzer_LED_Freq_Max = 1000;
float Buzzer_LED_Freq_Min = 10;

//These variables control the distance space we use on our equation

float Dist_Max = 50;
float Dist_Min = 10;
bool flag = false;
//current distance returned by sensor 

float Current_Dist = 0;
float Current_Freq = 0;

/*These variable are used to how frequently we should check the current distance and 
update “Current_Dist” variable and LCD display for values. */ 

int Update_Freq = 500;
int Update_LCD = 200;

//Frequency related values
int time1;
int time2;

char buffer[16];
void setup() {
  lcd.begin(16, 2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
}

void led_buzz(){
  
  if((Current_Freq <= Buzzer_LED_Freq_Max) && (Current_Freq >= Buzzer_LED_Freq_Min)){
     
     tone(buzzerpin,400, Current_Freq);
     digitalWrite(ledpin, HIGH);
     delay(Current_Freq);
     digitalWrite(ledpin, LOW);
  }
  else if(Current_Freq<Buzzer_LED_Freq_Max){
     tone(buzzerpin,400, Buzzer_LED_Freq_Max);
     digitalWrite(ledpin, HIGH);
     delay(20);
     digitalWrite(ledpin, LOW);

  }
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  int duration = pulseIn(echoPin, HIGH);
  Current_Dist = duration * 0.034 / 2;
//  Serial.print(Current_Dist);
//  delay(500);
  //Current_Freq formula
  float distance_normalized = (Current_Dist - Dist_Min) / (Dist_Max-Dist_Min);
  Current_Freq = distance_normalized * (Buzzer_LED_Freq_Max - Buzzer_LED_Freq_Min);
  Current_Freq +=  Buzzer_LED_Freq_Min;

  time2 = millis();
  int freq = time2-time1;
  if( freq >=Update_LCD){
    lcd.clear();
    lcd.setCursor(0,0);
    int roundabout = round(Current_Dist);
    sprintf(buffer,"Distance : %5d",roundabout);
    lcd.print(buffer); // Prints string "Distance" on the LCD
    if(freq>=Update_Freq){
      //Current_Freq formula
      Current_Freq =(((Current_Dist - Dist_Min) / (Dist_Max-Dist_Min))*(Buzzer_LED_Freq_Max - Buzzer_LED_Freq_Min))+Buzzer_LED_Freq_Min;   
    }
    lcd.setCursor(0,1);
    lcd.print("Frequency :  "); // Prints string "Frequency" on the LCD
    int roundabout2 = round(Current_Freq);
    lcd.print(roundabout2); // Prints the distance value from the sensor
    led_buzz();
    time1=time2;
  }
    digitalWrite(ledpin, LOW);
}
