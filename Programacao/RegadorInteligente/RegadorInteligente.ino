// FUnnciona git hub por favorzinho
#include <LiquidCrystal_I2C.h>

#include <Wire.h>

#include <SoftwareSerial.h>

/* This sample code demonstrates the normal use of a TinyGPS object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/

//SoftwareSerial ss(4, 3);

// initialize the LCD library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x27,16,2);

float Tagora = 0, Tsoma = 0 , T = 0, k = 0.92;
int TPin = 1; // temp sensor pin
int LDRPin = 2;
int HRPin = 3;

float TCelsius;
float Luminosidade = 0;
float Umidade = 0;

long antes = 0;
int segundos = 0;
int n =0;
float Cadj = 1;
int entrada = 0, Setpoint = 27, LumSetpoint = 300;
int led = 0;


void setup()
{
  Serial.begin(115200);

  //Serial.println("-------------------------------------------------------------------------------------------------------------------------------------");

  //ss.begin(57600);
  
  pinMode(13, OUTPUT); 
  pinMode(12, OUTPUT);    // Watering relay and arduino led
  pinMode(11, OUTPUT);    // Led LDR indicator

  // set up the LCD's number of columns and rows: 
  lcd.init(); //  lcd.begin(16, 2);
  lcd.backlight();
  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print("  T  RH  Lum  W");
  // lcd.print("Renata eh foda");
    
}

void loop()
{
  Serial.println();


  int i;
  unsigned long agora = millis();

  //T average calculation
  Tagora = analogRead(TPin);
  delay(10);
  Tagora = analogRead(TPin);
  delay(10);
  
  //read the value from the sensor
  Tagora = k * Tagora; // adjust for specific sensor. T boil must be 100 C
  n = n+1; Tsoma = Tsoma + Tagora;  T = Tsoma / n;
  Luminosidade =0;
  // a cada segundo
  for(int i = 0; i < 10; i++) { // Average 10 readings for accurate reading     
    delay(100);
    Luminosidade += analogRead(LDRPin);
    delay(100);
  }  
  Luminosidade = Luminosidade /10;
  

  // Leitura do sensor de umidade
  Umidade = 0;
  for(int i = 0; i < 10; i++) { // Average 10 readings for accurate reading     
    delay(100);
    Umidade += analogRead(HRPin);
    delay(100);
  }  
  Umidade = Umidade /10;
  Umidade = (Umidade/1024)*100;
  if(agora - antes > 1000) {            
    antes = agora;
    segundos = segundos + 1;
    //Heartbeat
    if ( led == 0 ) {led=1; digitalWrite(13, HIGH); }
      else {led=0; digitalWrite(13, LOW); };
    //lcd.setCursor(11, 0); lcd.print((int) segundos);  
    TCelsius = Cadj * ( 5 * T  * 100 ) / 1024;  // Voltage to Celsius 
    // Constante de ajuste , 5V =  V limite da entrada do Arduino,
    // 1024 niveis (Quantizacao da entrada do Arduino), 100 = 1/0.01 = 10mV/ºC (LM35)
  
    //TCelsius = 100;  
    lcd.setCursor(1, 1); lcd.print((int) TCelsius); // T em Celsius
    lcd.setCursor(5, 1); lcd.print((int) Umidade); // RH em %
    lcd.setCursor(9, 1); lcd.print((int) Luminosidade); // Ev em lx
    // Reset averaging cycle 
   Tagora = 0; Tsoma = 0; T = 0; n = 0; 
    }
    /******************************CONTROLE DE ATUADORES ***************************************/
  //Liga ou desliga resistência com histerese de +0 e -1 do SetPoint
  if (TCelsius < (Setpoint - 1)) { digitalWrite(12, LOW); lcd.setCursor(13, 1); lcd.print("Off "); };
  if (TCelsius > (Setpoint + 0)) { digitalWrite(12, HIGH); lcd.setCursor(13, 1); lcd.print("On "); };
  if (Luminosidade < (LumSetpoint )) { digitalWrite(11, LOW);  };
  if (Luminosidade > (LumSetpoint + 50)) { digitalWrite(11, HIGH);  };

}



