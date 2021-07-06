#include <SPI.h>                          //SPI kütüphanemizi tanımlıyoruz.
#include <MFRC522.h>                      //MFRC522 kütüphanemizi tanımlıyoruz.
#include <Servo.h>                        //Servo kütüphanemizi tanımlıyoruz.

int RST_PIN = 9;                          //RC522 modülü reset pinini tanımlıyoruz.
int SS_PIN = 10;                          //RC522 modülü chip select pinini tanımlıyoruz.
int servoPin = 8;                         //Servo motor pinini tanımlıyoruz.
int girenkisi = 0;                        //kisi sayisini tutuyoruz.

int const RedLed=6;
int const GreenLed=5;

String Name;//user name

 
Servo motor;                              //Servo motor için değişken oluşturuyoruz.
MFRC522 rfid(SS_PIN, RST_PIN);            //RC522 modülü ayarlarını yapıyoruz.
byte ID[5] = {249,48,246,162,0};          //Yetkili kart ID'sini tanımlıyoruz. 


void setup() { 
  motor.attach(servoPin);                 //Servo motor pinini motor değişkeni ile ilişkilendiriyoruz.
  Serial.begin(9600);                     //Seri haberleşmeyi başlatıyoruz.
  SPI.begin();                            //SPI iletişimini başlatıyoruz.
  rfid.PCD_Init();                        //RC522 modülünü başlatıyoruz.

                
  Serial.println("LABEL,Date,Time"+Name);// 
   Serial.println(girenkisi); //send the Number to excel

  pinMode(RedLed,OUTPUT);
  pinMode(GreenLed,OUTPUT);

  
}
 
void loop() {
  if (girenkisi <3){
  if ( ! rfid.PICC_IsNewCardPresent())    //Yeni kartın okunmasını bekliyoruz.
  
    return;

  if ( ! rfid.PICC_ReadCardSerial())      //Kart okunmadığı zaman bekliyoruz.
  
    return;

  if (rfid.uid.uidByte[0] == ID[0] &&     //Okunan kart ID'si ile ID değişkenini karşılaştırıyoruz.
    rfid.uid.uidByte[1] == ID[1] && 
    rfid.uid.uidByte[2] == ID[2] && 
    rfid.uid.uidByte[3] == ID[3] && 
    rfid.uid.uidByte[4] == ID[4]) {

       Name="Kullanıcı";//user name
          digitalWrite(GreenLed,LOW);
          digitalWrite(RedLed,HIGH);
         delay(1500);
          
        girenkisi += 1;                  //kisi sayisini her kapı açıldığında 1 arttır.
        Serial.print("İcerideki kisi sayisi:");   // içerideki kişi sayısını bastırıyoruz.
        Serial.println(girenkisi); 
        Serial.println("Kapi acildi");
        ekranaYazdir();
        motor.write(90);                 //Servo motoru 90 dereceye getiriyoruz.
        delay(1500);
        motor.write(0);                   //Servo motoru 0 dereceye getiriyoruz.

        
 
      
        delay(1000);
    }
    else{                                 //Yetkisiz girişte içerideki komutlar çalıştırılır.

      Name="Geçersiz kullanıcı";//user name
     
       digitalWrite(GreenLed,HIGH);
       digitalWrite(RedLed,LOW);
         delay(1000);
      
      Serial.println("Yetkisiz Kart");
      ekranaYazdir();
    }
  rfid.PICC_HaltA();
  }
      else{
        if ( ! rfid.PICC_IsNewCardPresent())    //Yeni kartın okunmasını bekliyoruz.
          return;

        if ( ! rfid.PICC_ReadCardSerial())      //Kart okunmadığı zaman bekliyoruz.
          return;

        if (rfid.uid.uidByte[0] == ID[0] &&     //Okunan kart ID'si ile ID değişkenini karşılaştırıyoruz.
          rfid.uid.uidByte[1] == ID[1] && 
          rfid.uid.uidByte[2] == ID[2] && 
          rfid.uid.uidByte[3] == ID[3] && 
          rfid.uid.uidByte[4] == ID[4]) {
     
               digitalWrite(GreenLed,LOW);
          digitalWrite(RedLed,LOW);          
      Serial.println("İçerideki kişi sayısı:");   // içerideki kişi sayısını bastırıyoruz.
           Serial.println(girenkisi); 
         Serial.println("Üzgünüm İçerisi dolu");
            delay(1000);
           
         
        }
          else{  
             digitalWrite(GreenLed,LOW);
          digitalWrite(RedLed,LOW);   
           Serial.print("İçerideki kişi sayısı:");   // içerideki kişi sayısını bastırıyoruz.
            Serial.println(girenkisi); 
            Serial.println("Üzgünüm İçerisi dolu");
            rfid.PICC_HaltA();
            delay(1000);
         
            Serial.println("SAVEWORKBOOKAS,Names/WorkNames");
            }
    

    }
  }
void ekranaYazdir(){
  Serial.print("ID Numarasi: ");
  for(int sayac = 0; sayac < 5; sayac++){
    Serial.print(rfid.uid.uidByte[sayac]);
   Serial.print(" ");
  }
  Serial.println("");

}
