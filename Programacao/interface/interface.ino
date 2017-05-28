// Primeira versão do controle do LCD com keypad
// Autor : Vinicius

#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup()
{
  lcd.begin(16, 2);
  //lcd.setCursor(0, 0);
  //lcd.print("Mordomo Verde");
  //lcd.setCursor(0, 1);
  //lcd.print("Planta:");
  pinMode(01, OUTPUT);
  pinMode(02, OUTPUT);
}

void loop()
{   
    
    
    lcd.setCursor(0, 0);
    lcd.print("Mordomo Verde");
    lcd.setCursor(0, 1);
    lcd.print("Planta 1:");
  
    int botao1;
    //botao1 = analogRead (0);  //Leitura do valor da porta analógica A0
    int naoselecionado = 0;
    
    while (naoselecionado == 0) {
      botao1 = analogRead (0);  //Leitura do valor da porta analógica A0
      if (botao1 > 100 && botao1 < 200 || botao1 > 400 && botao1 < 600 ) {
        lcd.setCursor(8, 1);
        lcd.print ("Avenca    ");
        digitalWrite(01, HIGH); 
      }
      if (botao1 > 200 && botao1 < 400 || botao1 < 100) {
        lcd.setCursor(8, 1);
        lcd.print ("Renda    ");
        digitalWrite(01, LOW); 
      }
      if (botao1 > 600 && botao1 < 800) {
        lcd.setCursor(0, 1);
        lcd.print ("Selecionado 1!!         ");
        naoselecionado = 1;
      }
//     delay(1000);
    }
      delay(1000);

    int botao2;
//    botao2 = analogRead(0);
    naoselecionado = 0;
     
    lcd.setCursor(0, 0);
    lcd.print("Mordomo Verde");
    lcd.setCursor(0, 1);
    lcd.print("Planta 2:");

    while (naoselecionado == 0) {
      botao2 = analogRead(0);
      if (botao2 > 100 && botao2 < 200 || botao2 > 400 && botao2 < 600 ) {
        lcd.setCursor(9, 1);
        lcd.print ("Avenca    ");
        digitalWrite(02, HIGH); 
      }
      if (botao2 > 200 && botao2 < 400 || botao2 < 100) {
        lcd.setCursor(9, 1);
        lcd.print ("Renda    ");
        digitalWrite(02, LOW); 
      }
      if (botao2 > 600 && botao2 < 800) {
        lcd.setCursor(9, 1);
        lcd.print ("Selecionado 2!!         ");
        naoselecionado = 1;
      } 
      
    }

    
    lcd.clear();
     while(true) {
        lcd.setCursor(0, 0);
        lcd.print("Plantas ok");
      }
}
