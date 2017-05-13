// FUnnciona git hub por favorzinho
#include <LiquidCrystal_I2C.h>

#include <Wire.h>

#include <SoftwareSerial.h>

#include <Servo.h>

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
int entrada = 0, Setpoint = 27, LumSetpoint = 300, HumiditySetpoint = 70;
int led = 0;

//Variaveis do controle da persiana

Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
int pos = 0;    // variable to store the servo position 
int dest = 0;   // Servo destination depending on photocell reading
int spd = 25;   // how fast should the servo move? 50 is quier

int servoPin=0; //havent used this yet
int photocellPin = 2; // the cell and 10K pulldown are connected to a0
int photocellReading; // the analog reading from the analog resistor divider

int state = 0; //Keep track of state so we don't send signal to the servo without readon, better on battery life
int prevstate = 0;

int debug = 0; //Set this to 1 for serial debug output

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
  if (TCelsius < (Setpoint - 1) || Umidade > HumiditySetpoint) { digitalWrite(12, LOW); lcd.setCursor(13, 1); lcd.print("Off "); };
  if (TCelsius > (Setpoint + 0) || Umidade < HumiditySetpoint) { digitalWrite(12, HIGH); lcd.setCursor(13, 1); lcd.print("On "); };
  if (Luminosidade < (LumSetpoint )) { digitalWrite(11, LOW);  };
  if (Luminosidade > (LumSetpoint + 50)) { digitalWrite(11, HIGH);  };


  //Controle da persiana
    photocellReading = analogRead(photocellPin); //Query photo cell
      debug and Serial.print("Light Reading :");
      debug and Serial.print(photocellReading); // the raw analog reading
      debug and Serial.print(" | Position: ");
      debug and Serial.print(pos);    
      debug and Serial.print(" | State: ");

    //Define the modes based on how bright it is, and set corresponding servo position
    if (photocellReading < 125) {
        debug and Serial.println("Very Bright Day");
      dest=180;      
      state=1;
    } 
    else if (photocellReading < 250) {
        debug and Serial.println("Day");
      dest=135;      
      state=2;           
    } 
    else if (photocellReading < 375) {
        debug and Serial.println("Dusk");
      dest=85;
      state=3;
      
    } 
    else if (photocellReading < 1023) {
        debug and Serial.println("Night");
      dest=20;
      state=4;
    } 
    else {
        debug and Serial.println("No reading");      
    }
    


    if (state != prevstate){ //IF the photocell reading is different from last sample then execute servo controls
          debug and Serial.println("State Change");      
        myservo.attach(8);                     //Connect to servo
        if (pos > dest){  // If the current position is great than the destination then we must subtract
              //for(pos = pos; pos>=dest; pos-=1)     // Change current position to desired position, one degree at a time.
              while (pos > dest)
              {                                
                  debug and Serial.print("Was :");
                  debug and Serial.print(pos);
                myservo.write(pos);              // tell servo to go to position in variable 'pos' 
                delay(spd);                 // waits 15ms for the servo to reach the position 
                pos--;
                  debug and Serial.print(" | Is :");
                  debug and Serial.println(pos);
              } 
            myservo.detach();                //Detach from Servo
        } 
        else {  // If the current position is great than the destination then we must add
            myservo.attach(8);
              //for(pos = pos; pos <= dest; pos+=1)     // goes from 180 degrees to 0 degrees 
              while (pos < dest)
              {                        
                  debug and Serial.print("Was :");
                  debug and Serial.print(pos);        
                myservo.write(pos);              // tell servo to go to position in variable 'pos' 
                delay(spd);                     // waits 15ms for the servo to reach the position 
                pos++;
                  debug and Serial.print(" | Is :");
                  debug and Serial.println(pos);
              } 
        }
        myservo.write(pos); // Doing a write out side of the loop because I had a feeling the last position value was being skipped. I think I'm wrong though
        delay(spd); 
        myservo.detach();
    }
    prevstate = state; //Remember state so we can compare it again next round
    delay(10); //Optional delay, this probalby needs to be removed when IR receiver code get's added.
}



