#include <PZEM004Tv30.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

//Konfigurasi WiFi
const char *ssid = "POCO X3 Pro";
const char *password = "bayu1234";
 
//IP Address Server yang terpasang XAMPP
const char *host = "192.168.225.249";
 

//deklarate Sensor PZEM-004T
PZEM004Tv30 pzem(14, 12);  // Menggunakan Hardware Serial 3 (RX-D6 -> TX-D5)

unsigned long start, finished, elapsed;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
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

void loop() {
  float current = pzem.current();
  if(current != NAN){
    Serial.print("Current: ");
    Serial.print(current);
    Serial.println(" A");
  } else {
    Serial.println("Error reading current");
  }

  if(current != NAN && current>0){
    Serial.println("Mulai menghitung....");
    
    float voltage_sensor, current_sensor, power_sensor, energy_sensor;
    voltage_sensor = pzem.voltage();
    current_sensor = pzem.current();
    power_sensor = pzem.power();
    energy_sensor = pzem.energy();
    /*
    status pembacaan :
    1 -> start
    2 -> on progress
    3 -> stop
    */

    int status = 1;
    //mulai mengirim data
    send_data(status, current_sensor, voltage_sensor, power_sensor, energy_sensor);
    while(pzem.current()>0){
      status = 2;
      send_data(status, current_sensor, voltage_sensor, power_sensor, energy_sensor);
      delay(2000);
    }

    //selesai perhitungan
    status = 3;
    send_data(status, current_sensor, voltage_sensor, power_sensor, energy_sensor);

  }

  delay(1000);
}

void send_data(int status, float current, float voltage, float power, float energy){

  String status_sensor;
  if(status == 1){
    status_sensor = "start";
  }else if(status == 2){
    status_sensor = "on_count";
  }else if(status == 3){
    status_sensor = "stop";
  }

  String post_data = (String)"id=''&tegangan="+voltage+"&arus="+current+"&daya="+power+"&energi="+energy+"&status_alat="+status_sensor;

  Serial.println(post_data);
  WiFiClient wificlient;

  HTTPClient http;
  http.begin(wificlient,"http://192.168.59.249/monitoring_daya_listrik/config/restapi.php?function=insert_data_sensor");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  auto httpCode = http.POST(post_data);
  String payload = http.getString();
  Serial.println(payload);
}