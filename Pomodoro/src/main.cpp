/**********************************************
*              RELOGIO POMODORO               *
*                                             *
**********************************************/
 
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include <DS1307.h>
 
#define SS_PIN 10
#define RST_PIN 9

#define POMODORO_SEG 25
#define SHORT_PAUSE 5
#define BIG_BREAK 10

MFRC522 mfrc522(SS_PIN, RST_PIN);  
DS1307 rtc(A3, A2);
rgb_lcd lcd;
 
int CONTROL = 0;
int CONTADOR_SEG = 0;
int POMODORO = 0;
bool FLAG = false;
int contador = 1;

String START_TAG = "95 5F 06 02";
String STOP_TAG = "E5 12 0A 02";
char st[20];
 

void mostrarhora()
{
  //Mostra as informações no Serial Monitor
  Serial.print("Hora : ");
  Serial.print(rtc.getTimeStr());
  Serial.print(" ");
  Serial.print("Data : ");
  Serial.print(rtc.getDateStr(FORMAT_SHORT));
  Serial.print(" ");
  Serial.println(rtc.getDOWStr(FORMAT_SHORT));

  lcd.clear();
  lcd.print(rtc.getDateStr(FORMAT_SHORT));
  lcd.print(" ");
  lcd.print(rtc.getDOWStr(FORMAT_SHORT));
  lcd.print(" ");
  lcd.setCursor(5, 1);
  lcd.print(rtc.getTimeStr(FORMAT_SHORT));
  //Aguarda 1 segundo e repete o processo
  delay (1000);
}

void Alarm_clock() {
      tone(8,5000);
      delay(500);
      noTone(8);
      delay(500);
}

void stop(){
    POMODORO = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("      STOP");
    lcd.setCursor(0,1);
    lcd.print("    Ate mais");
    lcd.setRGB(55,55,55);
    CONTADOR_SEG = 0;
    contador = 1;
    delay(3000);
}

int RFID_tag() 
{
      // Look for new cards
      if ( ! mfrc522.PICC_IsNewCardPresent()) 
      {
        return;
      }
      // Select one of the cards
      if ( ! mfrc522.PICC_ReadCardSerial()) 
      {
        return;
      }
      //Mostra UID na serial
      Serial.print("UID da tag :");
      String conteudo= "";
      for (byte i = 0; i < mfrc522.uid.size; i++) 
      {
         Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
         Serial.print(mfrc522.uid.uidByte[i], HEX);
         conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
         conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
      }
      Serial.println();
      Serial.print("Mensagem : ");
      conteudo.toUpperCase();
      if ((conteudo.substring(1) == START_TAG) && (POMODORO == 0)) //POMODORO
      {
        Serial.println("===== INICIANDO O POMODORO =====");   
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(" INICIO POMODORO");
        lcd.setRGB(0,0,55);
        POMODORO = 1;
        FLAG = true;
        return 1;
      }
      if ((conteudo.substring(1) == START_TAG) && (POMODORO == 2)) //PAUSE
      {
        Serial.println("===== INICIANDO A PAUSA =====");   
        lcd.setRGB(0,55,0);
        POMODORO = 1;
        FLAG = false;
        return 2;
      }

      if (conteudo.substring(1) == STOP_TAG) //PAUSE
      {
        Serial.println("=========== STOP ===========");   
        lcd.setRGB(55,55,55);
        POMODORO = 0;
        FLAG = false;
        return 3;
      }
    
      else
          return 0;
} 
 
void setup() 
{
  Serial.begin(9600);   // Inicia a serial
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  Serial.println("RFID OK");
  Serial.println();
  //Define o número de colunas e linhas do LCD:  
  lcd.begin(16, 2);  
  lcd.setRGB(55,55,55);
  //Aciona o relogio
  rtc.halt(false);
   
  // As linhas abaixo setam a data e hora do modulo
  // e podem ser comentada apos a primeira utilizacao
  // rtc.setDOW(MONDAY);      //Define o dia da semana
  // rtc.setTime(01, 04, 0);     //Define o horario
  // rtc.setDate(23, 11, 2020);   //Define o dia, mes e ano
   
  //Definicoes do pino SQW/Out
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);
  mostrarhora();

  pinMode(8,OUTPUT);
}
 
void loop() 

{
  if(!POMODORO){
    mostrarhora();
  }
  else {
    if(CONTADOR_SEG > 3) mostrarhora();
    else  delay(1000);
  }

  Serial.println(CONTADOR_SEG);
  Serial.println(POMODORO);
  Serial.print("FLAG ");
  Serial.println(FLAG);
  Serial.print("Contador ");
  Serial.println(contador);
  CONTROL = RFID_tag();
  if (CONTROL == 3) stop();
  
  if(POMODORO) {
    CONTADOR_SEG++;
    if((CONTADOR_SEG == POMODORO_SEG) && FLAG && (contador < 4)){
        lcd.clear();
        lcd.setCursor(0,0);
        Serial.println("ATIVE A PAUSA");
        lcd.setRGB(55,0,55);
        POMODORO = 2;
        do{
          Alarm_clock();
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("  ATIVE A PAUSA");
          CONTROL = RFID_tag();
          if (CONTROL == 3) {
            stop();
            return;
          }
        }while(!CONTROL);

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(" Pequena Pausa!");
        lcd.setRGB(0,55,0);
        POMODORO = 1;
        CONTADOR_SEG = 0;
    }
    if((CONTADOR_SEG == SHORT_PAUSE) && (!FLAG) && (contador < 4)){
        lcd.clear();
        lcd.setCursor(0,0);
        Serial.println("ATIVE O POMODORO");
        lcd.setRGB(55,0,55);
        POMODORO = 0;
        do{
          Alarm_clock();
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("ATIVE O POMODORO");
          CONTROL = RFID_tag();
          if (CONTROL == 3) {
            stop();
            return;
          }
        }while(!CONTROL);

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  Bons Estudos!");
        lcd.setRGB(0,0,55);
        POMODORO = 1;
        CONTADOR_SEG = 0;
        contador++;
    }
    if((CONTADOR_SEG == POMODORO_SEG) && FLAG && (contador == 4)){
        lcd.clear();
        lcd.setCursor(0,0);
        Serial.println("ATIVE A PAUSA");
        lcd.setRGB(55,0,55);
        POMODORO = 2;
        do{
          Alarm_clock();
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("  ATIVE A PAUSA");
          CONTROL = RFID_tag();
          if (CONTROL == 3) {
            stop();
            return;
          }
        }while(!CONTROL);

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  Grande Pausa");
        lcd.setCursor(0,1);
        lcd.print("  Bom descanco!");
        lcd.setRGB(10,10,0);
        POMODORO = 1;
        CONTADOR_SEG = 0;
    }
    if((CONTADOR_SEG == BIG_BREAK) && (!FLAG) && (contador == 4)){
        lcd.clear();
        lcd.setCursor(0,0);
        Serial.println("FIM POMODORO");
        lcd.setRGB(55,0,55);
        POMODORO = 0;
        do{
          Alarm_clock();
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("  FIM POMODORO");
          CONTROL = RFID_tag();
          if (CONTROL == 3) {
            stop();
            return;
          }
        }while(!CONTROL);

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  Bom trabalho!");
        lcd.setCursor(0,1);
        lcd.print("  Até mais");
        lcd.setRGB(55,55,55);
        POMODORO = 0;
        CONTADOR_SEG = 0;
        contador = 1;

    }
    
  }

} 
 
