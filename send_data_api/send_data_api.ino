
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

//Konfigurasi WiFi
const char *ssid = "POCO X3 Pro";
const char *password = "bayu1234";
 
//IP Address Server yang terpasang XAMPP
const char *host = "192.168.225.249";
 
void setup() {
  Serial.begin(9200);
  
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  Serial.println("");
 
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //Jika koneksi berhasil, maka akan muncul address di serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
 
int value = 0;
  
void loop(){
  // Proses Pengiriman -----------------------------------------------------------
  delay(5000);
  ++value;
  
  // Membaca Sensor Analog -------------------------------------------------------
  const int pinADC = A0;
  int sensitivitas = 66;            // jenis sensor 185 = ACS712 5A, 100 = ACS712 20A, 66 = ACS712-30A
  int nilaiadc= 0;                 // settingan nilai awal yang dibaca sensor
  int teganganoffset = 2500; 
  double tegangan = 00;
  double nilaiarus = 00;

  nilaiadc = analogRead(pinADC);
  tegangan = (nilaiadc / 1099.0) * 5000;                     // pencarian Nilai Tegangan Listrik Volt (V)..  5 volt konversi miliVolt(5000 mV)
  nilaiarus = ((tegangan - teganganoffset) / sensitivitas); // Pencarian Nilai Arus Listrik Ampere (A)
  
  if(nilaiarus < 0){
    nilaiarus *= -1;
  }

  Serial.println(nilaiadc);                   // nilai adc adalah nilai yang baca sensor  
  Serial.println(tegangan,2);                 // nilai 2 adalah nilai yang ditampilkan setelah titik desimal
 
  Serial.println(nilaiarus,2);
  delay(10000);

  String post_data = (String)"id=''&arus="+nilaiarus+"&tegangan="+tegangan;

  Serial.println(post_data);
  WiFiClient wificlient;

  HTTPClient http;
  http.begin(wificlient,"http://192.168.84.249/web_server/config/restapi.php?function=insert_data_sensor");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  auto httpCode = http.POST(post_data);
  String payload = http.getString();
  Serial.println(payload);

}
