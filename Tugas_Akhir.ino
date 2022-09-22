#include "CTBot.h"
CTBot myBot;

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

int buzzer = D5;
int pompa= D3;
//int nilaiTerendah  = 1024;
//int nilaiTertinggi = 600;
//int persentase;

//int nilaiADC = analogRead(sensorValue);
#define Sensor A0
float nilai_sensor;
float n_akhir;

String ssid = "xxxx";     // Sesuikan dengan nama wifi anda
String pass = "xxxxxxxx"; // sesuaikan password wifi
String token = "5268797451:AAFgxfUpyXZZXz-vwq7vKGcfHivWKjrWy4w"; // token bot telegram yang telah dibuat
 
 
 void setup() {
Serial.begin(115200);
//  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(pompa,OUTPUT);
  pinMode(buzzer, OUTPUT);

  
  Serial.println("Starting TelegramBot...");

  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);

  // melakukan cek pada sinyal
  if (myBot.testConnection())
    Serial.println("\ntestConnection OK");
  else
    Serial.println("\ntestConnection NOK");
}

    void buzzerManual(){
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    delay(900);
    digitalWrite(buzzer, HIGH);
    delay(800);
    digitalWrite(buzzer, LOW);
  }

void loop() {
nilai_sensor = analogRead(A0);
n_akhir = map(analogRead(A0), 1024, 100, 0, 100); 

  if (n_akhir >= 100){
  n_akhir = 100;
  }
  else if (n_akhir<0)
  {
   n_akhir=0;
  }
  
  Serial.print("Nilai Analog: ");                 //menampilkan nilai analog pada serial monitor
  Serial.println(nilai_sensor);
  Serial.print("Nilai Kelembaban: ");             //menampilkan hasil konversi nilai analog ke persen
  Serial.println(n_akhir);

  Serial.print("Kelembaban tanah = ");
  Serial.print(n_akhir);
  Serial.print("%\n\n");
  delay(1000);
  
  lcd.setCursor(0,0);
  lcd.print("SOIL MOISTURE: ");
  lcd.setCursor(15,0);
  
//  lcd.print(persentase);
lcd.print(n_akhir);
 
  lcd.setCursor(19,0);
  lcd.print("%");
  
  TBMessage msg;
   if (myBot.getNewMessage(msg)) {
    if (msg.text.equals("siram") ) {    
       lcd.setCursor(0,1);
       lcd.print("WATER PUMP   : ON ");
       lcd.setCursor(0,2);
       lcd.print("PROSES MENYIRAM ");          
       digitalWrite(pompa, LOW);
       delay(17000);
       lcd.setCursor(0,1);
       lcd.print("WATER PUMP   : ON ");
       lcd.setCursor(0,2);
       lcd.print("PROSES MENYIRAM ");   
       buzzerManual();
       myBot.sendMessage(msg.sender.id, "Pompa telah selesai menyiram.");  //kirim pesan ke bot telegram
      
    }else if (msg.text.equals("off") ) {              
       digitalWrite(pompa,LOW);
       delay(100000);
       lcd.setCursor(0,1);
       lcd.print("WATER PUMP   : OFF");
       lcd.setCursor(0,2);
       lcd.print("AIR SUDAH CUKUP ");
       myBot.sendMessage(msg.sender.id, "Air sudah cukup.");  //kirim pesan ke bot telegram
      
    }else if (msg.text.equals("kelembaban") ) {              
       myBot.sendMessage(msg.sender.id,"Nilai kelembaban tanah saat ini " +  String (n_akhir ) + '%');  //kirim pesan ke bot telegram
      
    }else {                                                    
      // membalas pesan selain kode diatas
      String reply;
      reply = (String)"Hallo! " + msg.sender.username + (String)". Ketik : 'siram' untuk melakukan penyiraman terhadap tanaman, 'off' untuk menghentikan penyiraman pada tanaman, 'kelembaban' untuk mengetahui nilai kelembaban tanah saat ini.";
      myBot.sendMessage(msg.sender.id, reply);         
    }
   }

  if(n_akhir >= 60){
    digitalWrite(pompa,HIGH);
    lcd.setCursor(0,1);
    lcd.print("WATER PUMP   : OFF ");
    lcd.setCursor(0,2);
    lcd.print("AIR SUDAH CUKUP ");
    }else if(n_akhir <= 60){
    digitalWrite(pompa,LOW);
    lcd.setCursor(0,1);
    lcd.print("WATER PUMP   : ON ");
    lcd.setCursor(0,2);
    lcd.print("PROSES MENYIRAM ");
    buzzerManual();
    delay(1000);
    }else if(n_akhir <= 10){
    digitalWrite(pompa,HIGH);
    lcd.setCursor(0,1);
    lcd.print("WATER PUMP   : OFF ");
    lcd.setCursor(0,2);
    lcd.print("AIR SUDAH CUKUP ");  
      }
  
  delay(100); 
}
