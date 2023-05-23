/*
LIA
Using the Arduino, create a PID controller.
connect an LED and an LDR that are pointing to each other to the Arduino to control the light from the LED.
*/
#include <LiquidCrystal.h>          // Allows the lcd library to be referenced for new functions
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int LED = 9;                              // Pin for the lcd output
int Sense = A0;                           // Resistor sensor input
int Var = A1;                             // Pot Input 


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);                                 // Establish Baud rate for serial monitor
pinMode(LED,OUTPUT);                                // LED is an output
lcd.begin(15,2);                                    // Declare how mnay colums and rows will be used
}

void loop() {
  // put your main code here, to run repeatedly:
delay(100);
int PV = analogRead(Sense);                         // Read the input from the photo resistor
int Set = analogRead(Var);                          // Set point is equal to the Potentiometer
long CO = map(Set, 0, 1023, 0, 255);                // Set the Output to be equal to the ratio of the Potentiometer
analogWrite(LED,CO);                                // Output the value

// Write the CO to the serial monitor and LCD
Serial.print("CO= ");
Serial.print(CO);
lcd.setCursor(0,1);
lcd.print("CO");
lcd.print(CO);
lcd.print(" ");

// Write the PV to the serial monitor and LCD
Serial.print("\t");
Serial.print("PV= ");
Serial.print(PV);
lcd.setCursor(0,0);
lcd.print("PV=");
lcd.print(PV);
lcd.print(" ");

// Write the SP to the serial monitor and LCD
Serial.print("\t");
Serial.print("SP= ");
Serial.println(Set);
lcd.setCursor(8,0);
lcd.print("SP=");
lcd.print(Set);
lcd.print(" ");


}
