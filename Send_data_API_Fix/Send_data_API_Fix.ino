#include <PZEM004Tv30.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

//Konfigurasi WiFi
const char *ssid = "POCO X3 Pro";
const char *password = "bayu1234";
 
//IP Address Server yang terpasang XAMPP
const char *host = "192.168.13.42";
 

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

  Serial.println("Mulai menghitung waktu");

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

  //start hitung if ada arus >0
  if(current != NAN && current>0){
    Serial.println("Mulai menghitung....");
    
    float voltage_value, current_value, power_value, energy_value;
    voltage_value = pzem.voltage();
    current_value = pzem.current();
    power_value = pzem.power();
    energy_value = pzem.energy();
    
    start=millis();
    while(pzem.current()>0){
      //Serial.print(".");
    }
    Serial.println(" ");
    finished=millis();
    delay(200); // for debounce
    
    float used_time = count_time();
    
        
    Serial.print(used_time);
    Serial.println(" second");
    Serial.print("V : ");
    Serial.println(voltage_value);
    Serial.print("A : ");
    Serial.println(current_value);
    Serial.print("W : ");
    Serial.println(power_value);
    Serial.print("Kwh : ");
    Serial.println(energy_value);

    send_data(used_time, current_value, voltage_value, power_value, energy_value);
  }

  delay(1000);
}


float count_time(){
  float second;
  elapsed = finished-start;
  second = elapsed/1000;  
  return second;  
}

//send data with NodeMcu
void send_data(float time, float current, float voltage, float power, float energy){
  String post_data = (String)"id=''&tegangan="+voltage+"&arus="+current+"&daya="+power+"&energi="+energy+"&waktu="+time;

  Serial.println(post_data);
  WiFiClient wificlient;

  HTTPClient http;
  http.begin(wificlient,"http://192.168.13.249/monitoring_daya_listrik/config/restapi.php?function=insert_data_sensor");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  auto httpCode = http.POST(post_data);
  String payload = http.getString();
  Serial.println(payload);
}