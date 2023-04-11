const int analogIn = A0;
int mVperAmp = 66; // 185 = ACS712 5A, 100 = ACS712 20A, 66 = ACS712-30A
int RawValue= 0;
int ACSoffset = 2500; 
double Voltage = 0;
double Amps = 0;

void setup(){ 
     Serial.begin(9600);
     pinMode(analogIn, INPUT);
}

void loop(){
     RawValue = analogRead(analogIn);
     Voltage = (RawValue / 1023) * 5000; // Voltage ( mV )
     Amps = ((Voltage - ACSoffset) / mVperAmp);
    // Display the Value to Serial Monitor Window
    Serial.print("Raw Value = " ); // shows pre-scaled value
    Serial.println(RawValue);
    Serial.print("mV = "); // shows the voltage measured
    Serial.println(Voltage,3); // display 3 digits after decimal point
    Serial.print("Amps = "); // shows the voltage measured
    Serial.println(Amps,3); // display 3 digits after decimal point
    delay(2500);
 }