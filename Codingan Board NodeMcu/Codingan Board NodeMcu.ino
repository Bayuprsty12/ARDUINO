//IN BELOW CODINGAN for Sensor ACS712 ----//
//deklarasi Nilai sensor ACS712 yang dipakai 
const int pinADC = A0;
int sensitivitas = 66;            // jenis sensor 185 = ACS712 5A, 100 = ACS712 20A, 66 = ACS712-30A
int nilaiadc = 0;                 // settingan nilai awal yang dibaca sensor
int teganganoffset = 2500; 
double tegangan = 00;
double nilaiarus = 00;

double voltage = 00;
float volt = 220;
  

void setup(){
  Serial.begin(9600); //baud komunikasi serial monitor 9600bps
  Serial.print("-----> MEMBACA ARUS <----- \n");

  
  // delay(2000);


}
  
void loop(){
  nilaiadc = analogRead(pinADC);
  tegangan = (nilaiadc / 1094.0) * 5000;                     // pencarian Nilai Tegangan Listrik Volt (V)..  5 volt konversi miliVolt(5000 mV)
  nilaiarus = ((tegangan - teganganoffset) / sensitivitas); // Pencarian Nilai Arus Listrik Ampere (A)
  voltage = nilaiadc * nilaiarus;

  Serial.print("Nilai ADC yang terbaca = " );
  Serial.print(nilaiadc);
  
  Serial.print("    || tegangan (mV) = ");
  Serial.print(tegangan,2);                 // nilai 2 adalah nilai yang ditampilkan setelah titik desimal

  Serial.print("    || tegangan (V) = ");
  Serial.print(voltage,2); 

  Serial.print("    || Arus = ");
  Serial.println(nilaiarus,2);
 
  delay(500);
}
