// Primeira versão do controle do LCD com keypad
// Autor : Vinicius

#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup()
{
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Mordomo Verde");
  lcd.setCursor(0, 1);
  lcd.print("Planta:");
}

void loop()
{

  int botao;
  botao = analogRead (0);  //Leitura do valor da porta analógica A0

  if (botao > 100 && botao < 200 || botao > 400 && botao < 600 ) {
    lcd.setCursor(8, 1);
    lcd.print ("Avenca    ");
  }
  if (botao > 200 && botao < 400 || botao < 100) {
    lcd.setCursor(8, 1);
    lcd.print ("Renda    ");
  }
  if (botao > 600 && botao < 800) {
    lcd.setCursor(0, 1);
    lcd.print ("Selecionado!!         ");
  }
}
