#include <PZEM004Tv30.h>

PZEM004Tv30 pzem(14, 12);  // Menggunakan Hardware Serial 3 (RX-D6 -> TX-D5)

void setup() {
   Serial.begin(9600);
}

void loop() {
     float voltage = pzem.voltage();
     if(voltage != NAN){
         Serial.print("Voltage: "); 
         Serial.print(voltage);
         Serial.println("V");
     } else {
         Serial.println("Error reading voltage");
     }

     float current = pzem.current();
     if(current != NAN){
         Serial.print("Current: ");
         Serial.print(current);
         Serial.println("A");
     } else {
         Serial.println("Error reading current");
     }

     float power = current * voltage;
     Serial.print("Power: ");
     Serial.print(power);
     Serial.println("W");
     

     float energy = pzem.energy();
     if(current != NAN){
         Serial.print("Energy: ");
         Serial.print(energy,3);
         Serial.println("kWh");
     } else {
         Serial.println("Error reading energy");
     }

     Serial.println();
     delay(2000);
 }