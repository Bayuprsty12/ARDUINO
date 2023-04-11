// ------------- Welcome To Skiripsi/TA ALL CODINGAN --------------------
// 1908561005 Bayu Prasetiyo / INFORMATIKA / UDAYANA 

#include <ESP8266WiFi.h>

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
  delay(1000);
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

  Serial.print(nilaiadc);
  Serial.print(tegangan,2);                 // nilai 2 adalah nilai yang ditampilkan setelah titik desimal
 
  Serial.println(nilaiarus,2);
  delay(500);


  Serial.print("connecting to ");
  Serial.println(host);
 
  // Mengirimkan ke alamat host dengan port 80 -----------------------------------
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // Isi Konten yang dikirim adalah alamat ip si esp -----------------------------
  String url = "http://192.168.225.249/web_server/config/restapi.php?function=insert_data_sensor";
  
 
  Serial.print("Requesting URL: ");
  Serial.println(url);

  // Mengirimkan Request ke Server -----------------------------------------------
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 1000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
 
  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
 
  Serial.println();
  Serial.println("closing connection");
}
