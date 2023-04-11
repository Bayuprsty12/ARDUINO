#include <PZEM004Tv30.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <LiquidCrystal_I2C.h>
#define led D8

//Konfigurasi WiFi
const char *ssid = "POCO X3 Pro";
const char *password = "bayu1234";
 
//IP Address Server yang terpasang XAMPP
const char *host = "192.168.147.208";
 
//deklarate Sensor PZEM-004T
PZEM004Tv30 pzem(14, 12);  // Menggunakan Hardware Serial 3 (RX-D6 -> TX-D5)

unsigned long start, finished, elapsed;

//deklarate type lcd i2c
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led, OUTPUT); // deklarasi pin LED 5mm

  lcd.begin();
  lcd.setCursor(1,0);
  lcd.print("Welcome User");
  
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
    lcd.clear();
    lcd.setCursor(0,0);

    lcd.print("Current: ");
    lcd.print(current);
    lcd.print("amp");

    //fungsi LED 5mm
    digitalWrite (led, HIGH); // Led Nyala
    delay(1000);

    Serial.print("Current: ");
    Serial.print(current);
    Serial.println(" A");
  } else {
    Serial.println("Error reading current");
  }

  //start hitung if ada arus >0
  if(current != NAN && current>0){
    //fungsi LCD 16x2
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Mulai Menghitung"); // tampil di lcd
    
    Serial.println("Mulai Menghitung.");
    
    float voltage_value, current_value, power_value, energy_value;
    voltage_value = pzem.voltage();
    current_value = pzem.current();
    power_value = voltage_value * current_value;
    energy_value = pzem.energy();
    
    start=millis();
    while(pzem.current()>0){
      //Serial.print(".");
      //fungsi LED 5mm
      digitalWrite (led, HIGH); // Led Nyala
      delay(1000);

      digitalWrite (led, LOW); //Led Mati
      delay(1000); 
    }
    Serial.println(" ");
    finished=millis();
    delay(500); // for debounce
    
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

    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Data Terkirim");

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
  http.begin(wificlient,"http://192.168.147.249/monitoring_daya_listrik/config/restapi.php?function=insert_data_sensor");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  auto httpCode = http.POST(post_data);
  String payload = http.getString();
  Serial.println(payload);
}