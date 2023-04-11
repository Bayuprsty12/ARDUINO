#include <PZEM004Tv30.h>
#include <RTClib.h>
#include <Wire.h>

//deklarate Sensor PZEM-004T
PZEM004Tv30 pzem(14, 12);  // Menggunakan Hardware Serial 3 (RX-D6 -> TX-D5)

//deklarate Date/Time
RTC_DS3231 rtc;
char t[32];


void setup() {
  Serial.begin(9600);

  //SENSOR RTC DS3132
  Wire.begin(5, 4);   //Setting wire (5 untuk SDA dan 4 untuk SCL)

  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));  //Setting Time
  
  //dibawah ini untuk set manual jam
  //rtc.adjust(DateTime(2023, 1, 21, 3, 0, 0));
}

void loop() {
  //Pembacaan SENSOR PZEM-004T Nilai Tegangan(V) Arus(A) Daya(W) Energi(KWh) 
  float voltage = pzem.voltage();
     if(voltage != NAN){
         Serial.print("Voltage: "); 
         Serial.print(voltage);
         Serial.println(" V");
     } else {
         Serial.println("Error reading voltage");
     }

     float current = pzem.current();
     if(current != NAN){
         Serial.print("Current: ");
         Serial.print(current);
         Serial.println(" A");
     } else {
         Serial.println("Error reading current");
     }

     float power = pzem.power();
     if(current != NAN){
         Serial.print("Power: ");
         Serial.print(power);
         Serial.println(" W");
     } else {
         Serial.println("Error reading power");
     }

     float energy = pzem.energy();
     if(current != NAN){
         Serial.print("Energy: ");
         Serial.print(energy,3);
         Serial.println(" kWh");
     } else {
         Serial.println("Error reading energy");
     }
     


      //Pembacaan SENSOR RTC DS3132 
      DateTime now = rtc.now();       //Menampilkan RTC pada variable now
    
      Serial.print("Tanggal : ");
      Serial.print(now.day());        //Menampilkan Tanggal
      Serial.print("/");
      Serial.print(now.month());      //Menampilkan Bulan
      Serial.print("/");
      Serial.print(now.year());       //Menampilkan Tahun
      Serial.print(" ");
      
      Serial.print("Jam : ");
      Serial.print(now.hour());       //Menampilkan Jam
      Serial.print(":");
      Serial.print(now.minute());     //Menampilkan Menit
      Serial.print(":");
      Serial.print(now.second());     //Menampilkan Detik
      Serial.println();

    Serial.println();
    delay(2000);
}
