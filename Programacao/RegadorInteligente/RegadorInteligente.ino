// Funciona git hub por favorzinho
#include <LiquidCrystal_I2C.h>

#include <Wire.h>

#include <SoftwareSerial.h>

#include <Servo.h>

/* This sample code demonstrates the normal use of a TinyGPS object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/

//1 -> ESTRUTURA VERDE CLARO
//2 -> ESTRUTURA VERDE ESCURO

//SoftwareSerial ss(4, 3);

// initialize the LCD library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x27,16,2);

int Setpoint = 27, LumSetpoint = 300, HumiditySetpointAvenca = 70, HumiditySetpointRenda = 50, LumSetpointAvenca = 210, LumSetpointRenda = 220, passo = 20;
int HumiditySetpoint1, HumiditySetpoint2, LumSetpoint1, LumSetpoint2;

// temperatura, umidade e luminosidade (1)
//c
float Tagora1 = 0, Tsoma1 = 0 , T1 = 0, k1 = 0.92;
int TPin1 = 1; // temp sensor pin
int LDRPin1 = 2;
int HRPin1 = 3;

float TCelsius1;
float Luminosidade1 = 0;
float Umidade1 = 0;

long antes1 = 0;
int segundos1 = 0;
int n1 =0;
float Cadj1 = 1;
int entrada1 = 0;
int led1 = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// temperatura, umidade e luminosidade (2)

float Tagora2 = 0, Tsoma2 = 0 , T2 = 0, k2 = 0.92;
int TPin2 = 8; // temp sensor pin
int LDRPin2 = 9;
int HRPin2 = 10;

float TCelsius2;
float Luminosidade2 = 0;
float Umidade2 = 0;

long antes2 = 0;
int segundos2 = 0;
int n2 =0;
float Cadj2 = 1;
int entrada2 = 0;
int led2 = 0;


//Variaveis do controle das persianas

Servo myservo1;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
int pos1 = 0;    // variable to store the servo pos1ition 
int dest1 = 0;   // Servo dest1ination depending on photocell reading
int spd1 = 25;   // how fast should the servo move? 50 is quier

int servoPin1=0; //havent used this yet
int photocellPin1 = 2; // the cell and 10K pulldown are connected to a2
int photocellReading1; // the analog reading from the analog resistor divider

int state1 = 1; //Keep track of state1 so we don't send signal to the servo without readon, better on battery life
int prevstate1 = 1;

int debug1 = 1; //Set this to 1 for serial debug output

/////////////////////////////////////////////////////////////////////////////////////

Servo myservo2;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
int pos2 = 0;    // variable to store the servo pos1ition 
int dest2 =0;   // Servo dest1ination depending on photocell reading
int spd2 = 25;   // how fast should the servo move? 50 is quier

int servoPin2=0; //havent used this yet
int photocellPin2 = 9; // the cell and 10K pulldown are connected to a9
int photocellReading2; // the analog reading from the analog resistor divider

int state2 = 1; //Keep track of state1 so we don't send signal to the servo without readon, better on battery life
int prevstate2 = 1;

int debug2 = 0; //Set this to 1 for serial debug output

int plantaVerdeClaro; //VERDE CLARO
int plantaVerdeEscuro; //VERDE ESCURO
int sair = 0;

void setup()
{
  Serial.begin(115200);

  //Serial.println("-------------------------------------------------------------------------------------------------------------------------------------");

  //ss.begin(57600);
  
  pinMode(13, OUTPUT); 
  pinMode(12, OUTPUT);    // Watering relay and arduino led1 (1)
  pinMode(9, OUTPUT);    // Watering relay and arduino led2 (2)
  pinMode(11, OUTPUT);    // led1 LDR indicator (1)

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
//debug
/*    int plantaVerdeClaro =  HIGH; //VERDE CLARO 
    int plantaVerdeEscuro = LOW; //VERDE ESCURO
*/  
  sair = 0;
    //if(jaleu == false)
  //  {
      plantaVerdeClaro =  HIGH; //VERDE CLARO
      plantaVerdeEscuro = LOW; //VERDE ESCURO
      //jaleu = true;
    //}
//HIGH -> AVENCA
//LOW  -> RENDA
    
    if (plantaVerdeClaro == HIGH){
      HumiditySetpoint1 = HumiditySetpointAvenca;
      LumSetpoint1 = LumSetpointAvenca;
      Serial.println("Avenca");
    }
    else{
      HumiditySetpoint1 = HumiditySetpointRenda;
      LumSetpoint1 = LumSetpointRenda;
      Serial.println("Renda");
    }

    if (plantaVerdeEscuro == HIGH){
      HumiditySetpoint2 = HumiditySetpointAvenca;
      LumSetpoint2 = LumSetpointAvenca;
      Serial.println("AVENCA2");
    }
    else{
      HumiditySetpoint2 = HumiditySetpointRenda;
      LumSetpoint2 = LumSetpointRenda;
      Serial.println("RENDA2");
    } 


  int i;
//controle Temperatura, umidade e luminosidade (1)

  unsigned long agora1 = millis();

  //T average calculation
  Tagora1 = analogRead(TPin1);
  delay(10);
  Tagora1 = analogRead(TPin1);
  delay(10);
  
  //read the value from the sensor
  Tagora1 = k1 * Tagora1; // adjust for specific sensor. T boil must be 100 C
  n1 = n1+1; Tsoma1 = Tsoma1 + Tagora1;  T1 = Tsoma1 / n1;
  Luminosidade1 =0;
  // a cada segundo
  for(int i = 0; i < 10; i++) { // Average 10 readings for accurate reading     
    delay(100);
    Luminosidade1 += analogRead(LDRPin1);
    delay(100);
  }  
  Luminosidade1 = Luminosidade1 /10;
  

  // Leitura do sensor de umidade
  Umidade1 = 0;
  for(int i = 0; i < 10; i++) { // Average 10 readings for accurate reading     
    delay(100);
    Umidade1 += analogRead(HRPin1);
    delay(100);
  }  
  Umidade1 = Umidade1 /10;
  Umidade1 = (Umidade1/1024)*100;
  if(agora1 - antes1 > 1000) {            
    antes1 = agora1;
    segundos1 = segundos1 + 1;
    //Heartbeat
  /*  if ( led1== 0 ) {led1=1; digitalWrite(13, HIGH); }
      else {led1=0; digitalWrite(13, LOW); };*/
    //lcd.setCursor(11, 0); lcd.print((int) segundos);  
    TCelsius1 = Cadj1 * ( 5 * T1  * 100 ) / 1024;  // Voltage to Celsius 
    // Constante de ajuste , 5V =  V limite da entrada do Arduino,
    // 1024 niveis (Quantizacao da entrada do Arduino), 100 = 1/0.01 = 10mV/ºC (LM35)
  
    //TCelsius = 100;  
    lcd.setCursor(1, 1); lcd.print((int) TCelsius1); // T em Celsius
    lcd.setCursor(5, 1); lcd.print((int) Umidade1); // RH em %
    lcd.setCursor(9, 1); lcd.print((int) Luminosidade1); // Ev em lx
    // Reset averaging cycle 
   Tagora1 = 0; Tsoma1 = 0; T1 = 0; n1 = 0; 
    }
    /******************************CONTROLE DE ATUADORES ***************************************/
  //Liga ou desliga resistência com histerese de +0 e -1 do Setpoint
//  if (TCelsius1 < (Setpoint - 1) || Umidade1 < HumiditySetpoint) { digitalWrite(12, LOW); lcd.setCursor(13, 1); lcd.print("Off "); };
   if ( Umidade1 < HumiditySetpoint1) { digitalWrite(12, HIGH); lcd.setCursor(13, 1); lcd.print("Off "); };

  //if (TCelsius1 > (Setpoint + 0) || Umidade1 > HumiditySetpoint) { digitalWrite(12, HIGH); lcd.setCursor(13, 1); lcd.print("On "); };
    if ( Umidade1 > HumiditySetpoint1) { digitalWrite(12, LOW); lcd.setCursor(13, 1); lcd.print("On "); };
 // digitalWrite(12, HIGH);
  if (Luminosidade1 < (LumSetpoint )) { digitalWrite(11, LOW);  };
  if (Luminosidade1 > (LumSetpoint + 50)) { digitalWrite(11, HIGH);  };

Serial.println("u1");
Serial.println(Umidade1);
Serial.println("\\\\\\\\\\\\\\\\\\\\\\\\\\\\");


//controle Temperatura, umidade e luminosidade (2)

  unsigned long agora2 = millis();

  //T average calculation
  Tagora2 = analogRead(TPin2);
  delay(10);
  Tagora2 = analogRead(TPin2);
  delay(10);
  
  //read the value from the sensor
  Tagora2 = k2 * Tagora2; // adjust for specific sensor. T boil must be 100 C
  n2 = n2+1; Tsoma2 = Tsoma2 + Tagora2;  T2 = Tsoma2 / n2;
  Luminosidade2 =0;
  // a cada segundo
  for(int i = 0; i < 10; i++) { // Average 10 readings for accurate reading     
    delay(100);
    Luminosidade2 += analogRead(LDRPin2);
    delay(100);
  }  
  Luminosidade2 = Luminosidade2 /10;
  

  // Leitura do sensor de umidade
  Umidade2 = 0;
  for(int i = 0; i < 10; i++) { // Average 10 readings for accurate reading     
    delay(100);
    Umidade2 += analogRead(HRPin2);
    delay(100);
  }  
  Umidade2 = Umidade2 /10;
  Umidade2 = (Umidade2/1024)*100;
  if(agora2 - antes2 > 1000) {            
    antes2 = agora2;
    segundos2 = segundos2 + 1;
    //Heartbeat
    if ( led2== 0 ) {led2=1; digitalWrite(13, HIGH); }
      else {led2=0; digitalWrite(13, LOW); };
    //lcd.setCursor(11, 0); lcd.print((int) segundos);  
    TCelsius2 = Cadj2 * ( 5 * T2  * 100 ) / 1024;  // Voltage to Celsius 
    // Constante de ajuste , 5V =  V limite da entrada do Arduino,
    // 1024 niveis (Quantizacao da entrada do Arduino), 100 = 1/0.01 = 10mV/ºC (LM35)
  
    //TCelsius = 100;  
    lcd.setCursor(1, 1); lcd.print((int) TCelsius2); // T em Celsius
   // lcd.setCursor(5, 1); lcd.print((int) Umidade2); // RH em %
    lcd.setCursor(9, 1); lcd.print((int) Luminosidade2); // Ev em lx
    // Reset averaging cycle 
   Tagora2 = 0; Tsoma2 = 0; T2 = 0; n2 = 0; 
    }
    /******************************CONTROLE DE ATUADORES ***************************************/
  //Liga ou desliga resistência com histerese de +0 e -1 do Setpoint
  if (Umidade2 < HumiditySetpoint2) {digitalWrite(9, HIGH); lcd.setCursor(13, 1); lcd.print("Off "); };
  if (Umidade2 > HumiditySetpoint2) {digitalWrite(9, LOW); lcd.setCursor(13, 1); lcd.print("On "); };
  /*if (Luminosidade2 < (LumSetpoint )) { digitalWrite(11, LOW);  };
  if (Luminosidade2 > (LumSetpoint + 50)) { digitalWrite(11, HIGH);  };*/
Serial.println("u2");
Serial.println(Umidade2);
Serial.println("\\\\\\\\\\\\\\\\\\\\\\\\\\\\");
  
  //Controle da persiana 1
  do
  {
    photocellReading1 = analogRead(photocellPin1); //Query photo cell
      debug1 and Serial.print("Light Reading :");
      debug1 and Serial.print(photocellReading1); // the raw analog reading
      debug1 and Serial.print(" | pos1ition: ");
      debug1 and Serial.print(pos1);    
      debug1 and Serial.print(" | state1: ");

    //Define the modes based on how bright it is, and set corresponding servo pos1ition
    if (photocellReading1 <= LumSetpoint1 + 20 && photocellReading1 >= LumSetpoint1 - 20) {
        debug1 and Serial.println("ideal");
        debug1 and Serial.println("dest1: ");
        debug1 and Serial.println(dest1);
      //dest2=180;      
      state1=1;
    } 
    else if (photocellReading1 < LumSetpoint1 -20) {
        debug1 and Serial.println("claro");
                debug1 and Serial.println("dest1: ");

        debug1 and Serial.println(dest1);
    //    if(dest1 <= 110){
      dest1 = dest1 - passo;
      if (dest1 < 0){
        dest1 = 180; 
      }     
      state1=2;   //  }      
    } 
    else if (photocellReading1 > LumSetpoint1 + 20) {
        debug1 and Serial.println("escuro");
                debug1 and Serial.println("dest1: ");

        debug1 and Serial.println(dest1);
  //      if(dest1>=70) {
      dest1 = dest1+passo;
      if (dest1 > 180){
        dest1 = 0;
      }
      state1=3; //}
      
    } 
   /* else if (photocellReading1 < 1023) {
        debug1 and Serial.println("Night");
      dest1=0;
      state1=4;
    } */
    else {
        debug1 and Serial.println("No reading");      
    }
    


    if (state1 == 2 || state1 == 3){ //IF the photocell reading is different from last sample then execute servo controls
          debug1 and Serial.println("state1 Change");      
        myservo1.attach(8);                     //Connect to servo   porta  PWM 8
    // if(dest1 <= 180 && dest1 >= 0){
        if (pos1 > dest1){  // If the current pos1ition is great than the dest1ination then we must subtract
              //for(pos2 = pos2; pos2>=dest2; pos2-=1)     // Change current pos1ition to desired pos1ition, one degree at a time.
              while (pos1 > dest1)
              {                                
                  debug1 and Serial.print("Was :");
                  debug1 and Serial.print(pos1);
                myservo1.write(pos1);              // tell servo to go to pos1ition in variable 'pos1' 
                delay(spd1);                 // waits 15ms for the servo to reach the pos1ition 
                pos1--;
                  debug1 and Serial.print(" | Is :");
                  debug1 and Serial.println(pos1);
              } 
            myservo1.detach();                //Detach from Servo
        } 
        else {  // If the current pos1ition is great than the dest1ination then we must add
            myservo1.attach(8);
              //for(pos2 = pos2; pos2 <= dest2; pos2+=1)     // goes from 180 degrees to 0 degrees 
              while (pos1 < dest1)
              {                        
                  debug1 and Serial.print("Was :");
                  debug1 and Serial.print(pos1);        
                myservo1.write(pos1);              // tell servo to go to pos1ition in variable 'pos1' 
                delay(spd1);                     // waits 15ms for the servo to reach the pos1ition 
                pos1++;
                  debug1 and Serial.print(" | Is :");
                  debug1 and Serial.println(pos1);
              } 
        }
        myservo1.write(pos1); // Doing a write out side of the loop because I had a feeling the last pos1ition value was being skipped. I think I'm wrong though
        delay(spd1); 
        myservo1.detach();
    //}
    }
    prevstate1 = state1; //Remember state1 so we can compare it again next round
    delay(10); //Optional delay, this probalby needs to be removed when IR receiver code get's added.

//} while(state1 != 1);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  //Controle da persiana 2
//    do
  //  {
    photocellReading2 = analogRead(photocellPin2); //Query photo cell
      debug2 and Serial.print("Light Reading :");
      debug2 and Serial.print(photocellReading2); // the raw analog reading
      debug2 and Serial.print(" | pos1ition: ");
      debug2 and Serial.print(pos2);    
      debug2 and Serial.print(" | state2: ");

    //Define the modes based on how bright it is, and set corresponding servo pos1ition
    if (photocellReading2 <= LumSetpoint2 + 20 && photocellReading2 >= LumSetpoint2 - 20) {
        debug2 and Serial.println("ideal");
        debug2 and Serial.println("dest2: ");
        debug2 and Serial.println(dest2);
      //dest2=180;      
      state2=1;
    } 
    else if (photocellReading2 < LumSetpoint2 -20) {
        debug2 and Serial.println("claro");
                debug2 and Serial.println("dest2: ");

        debug2 and Serial.println(dest2);
       // if(dest2 <= 110) {
      dest2 = dest2 + passo;
      if (dest2>180) {
            dest2 = 0;
        }     
      state2=2;     //      }
    } 
    else if (photocellReading2 > LumSetpoint2 + 20) {
        debug2 and Serial.println("escuro");
                debug2 and Serial.println("dest2: ");

        debug2 and Serial.println(dest2);
//      if(dest2 >= 70) {
      dest2 = dest2-passo;
      if (dest2 < 0){
        dest2 = 180;
        }
      state2=3;  //}
      
    } 
   /* else if (photocellReading1 < 1023) {
        debug1 and Serial.println("Night");
      dest1=0;
      state1=4;
    } */
    else {
        debug2 and Serial.println("No reading");      
    }
    


    if (state2 == 2 || state2 == 3){ //IF the photocell reading is different from last sample then execute servo controls
          debug2 and Serial.println("state2 Change");      
        myservo2.attach(2);                     //Connect to servo   porta  PWM 4
if(dest2 <= 180 && dest2 >= 0){
        if (pos2 > dest2){  // If the current pos1ition is great than the dest1ination then we must subtract
              //for(pos2 = pos2; pos2>=dest2; pos2-=1)     // Change current pos1ition to desired pos1ition, one degree at a time.
              while (pos2 > dest2)
              {                                
                  debug2 and Serial.print("Was :");
                  debug2 and Serial.print(pos2);
                myservo2.write(pos2);              // tell servo to go to pos1ition in variable 'pos1' 
                delay(spd2);                 // waits 15ms for the servo to reach the pos1ition 
                pos2--;
                  debug2 and Serial.print(" | Is :");
                  debug2 and Serial.println(pos2);
              } 
            myservo2.detach();                //Detach from Servo
        } 
        else {  // If the current pos1ition is great than the dest1ination then we must add
            myservo2.attach(2);
              //for(pos2 = pos2; pos2 <= dest2; pos2+=1)     // goes from 180 degrees to 0 degrees 
              while (pos2 < dest2)
              {                        
                  debug2 and Serial.print("Was :");
                  debug2 and Serial.print(pos2);        
                myservo2.write(pos2);              // tell servo to go to pos1ition in variable 'pos1' 
                delay(spd2);                     // waits 15ms for the servo to reach the pos1ition 
                pos2++;
                  debug2 and Serial.print(" | Is :");
                  debug2 and Serial.println(pos2);
              } 
        }
        myservo2.write(pos2); // Doing a write out side of the loop because I had a feeling the last pos1ition value was being skipped. I think I'm wrong though
        delay(spd2); 
        myservo2.detach();
    }
   
    prevstate2 = state2; //Remember state1 so we can compare it again next round
    delay(10); //Optional delay, this probalby needs to be removed when IR receiver code get's added.
    }
    sair++;
} while(sair < 5);

Serial.println(digitalRead(9));


    
}




