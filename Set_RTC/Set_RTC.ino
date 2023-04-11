#include <DS3231.h> // input library DS3231

DS3231 rtc(SDA, SCL); // inisialisasi Penggunaan i2c
Time bacajam;
int pompa = 13;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // sett komunikasi baud serial monitor pada 115200
  rtc.begin();
  pinMode(pompa, OUTPUT);
  digitalWrite(pompa, LOW);
  //sett pertama download program
  rtc.setDate(10, 12, 2022);  // set tanggal
  rtc.setTime(10, 55, 00);  // set jam
  rtc.setDOW(6);   // set 1 hari "Sabtu"

}

void baca_waktu() {
  bacajam = rtc.getTime();
  Serial.println(rtc.getTimeStr());
  if (bacajam.hour = 6 && bacajam.min <= 1 && bacajam.sec <= 5) {
    digitalWrite(pompa, HIGH);
    Serial.println("NYALA");
  } 
  else if (bacajam.hour = 12 && bacajam.min <= 1 && bacajam.sec <= 5) {
    digitalWrite(pompa, HIGH);
    Serial.println("NYALA");
  } 
  else if (bacajam.hour = 18 && bacajam.min <=1 && bacajam.sec <= 5) {
    digitalWrite(pompa, HIGH);
    Serial.println("NYALA");
  } 
  else {
    digitalWrite(pompa, LOW);
    Serial.println("MATI");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  Serial.print(rtc.getDOWStr()); // prosedur pembacaan hari
  Serial.print(" ");

  Serial.print(rtc.getDOWStr()); //  prosedur pembacaan tanggal
  Serial.print(" -- ");
  baca_waktu();
  delay (1000); //waktu tunda 1 detik per cycle
}
