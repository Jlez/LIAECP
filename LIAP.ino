/*
LIA On/Off
Using the Arduino, create a PID controller.
connect an LED and an LDR that are pointing to each other to the Arduino to control the light from the LED.
*/

#include <LiquidCrystal.h>          // Allows the lcd library to be referenced for new functions
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int LED = 9;            // Pin for the lcd output
int Sense = A0;         // Resistor sensor input
// int Var = A1;           // Pot Input
int Hyst;
int CO;
int PBUp = 13;          // push button to increase SP
int PBDown = 10;        // push button to decrease SP
int PBLeft = 8;         // push button to increase hysterisis
int PBRight = 7;        // push button to decrease hysterisis
int Screen = 6;         // push button to change screen
int Error;
float PGain;

// Variables for Screen change
int HoldScreen;
int STATEScreen;
int Count = 0;

// Variables for the Set Point function
int STATEUp;
int STATEDown;
int HoldUp;
int HoldDown;
int CountSP = 700;

// Variables for the PGain function
int STATELeft;
int STATERight;
int HoldLeft;
int HoldRight;
float CountP = 0.10;
float PNum;

int SetValue() {
// Function to modify the Set Point value

  HoldUp= STATEUp;
  HoldDown= STATEDown;

STATEUp = digitalRead(PBUp);                             // Reads the value of PBup and stores it under the value "STATE
STATEDown = digitalRead(PBDown);                         // Reads the value of PBup and stores it under the value "STATE"
if (STATEUp == 1 && HoldUp == 0) {                                      // Check if the PB is pressed
    CountSP = CountSP + 10;                              // Add 10 to the count

    if (CountSP >= 1023) {                               // If the counter is above or equal to 1023 then keep it at 1023
      CountSP = 1023;
    }
    else if (STATEUp == 0) {
      HoldUp = 0;
    }
}

if (STATEDown == 1 && HoldDown == 0) {                                    // Check if the PB is pressed 
    CountSP = CountSP - 10;                              // Remove 10 from the count

    if (CountSP <= 0) {                                  // If the counter is less or equal to 0 then keep it at 0
      CountSP = 0;
    }
    else if (STATEDown == 0){
      HoldDown = 0;
    }
}
return CountSP;
}

float PValue() {
// Function to modify the Hysterisis value

  HoldLeft= STATELeft;
  HoldRight= STATERight;

STATELeft = digitalRead(PBLeft);                          // Reads the value of PBup and stores it under the value "STATE 
STATERight = digitalRead(PBRight);                        // Reads the value of PBup and stores it under the value "STATE"
if (STATELeft == 1 && HoldLeft == 0) {                                     // Check if the PB is pressed and held
    CountP = CountP + 0.1;                              // Add 5 to the count

    if (CountP >=100 ) {                                // If the counter is above or equal to 250 then keep it at 250
      CountP = 100;
    }
    else if (STATELeft ==0) {
      HoldLeft = 0;
    }
}

if (STATERight == 1 && HoldRight == 0) {                                    // Check if the PB is pressed and held
    CountP = CountP - 0.1;                              // Remove 5 from the count

    if (CountP <= 0) {                                  // If the counter is less or equal to 5 then reset it
      CountP = 0;
    }
    else if (STATERight == 0) {
      HoldRight = 0;
    }
}
return CountP;
}

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(LED,OUTPUT);                                // LED is an output
pinMode(PBUp,INPUT_PULLUP);                         // PBUp is an input pullup
pinMode(PBDown,INPUT_PULLUP);                       // PBDown is an input pullup
pinMode(PBLeft,INPUT_PULLUP);                       // PBLeft is an input pullup
pinMode(PBRight,INPUT_PULLUP);                      // PBRight is an input pullup
pinMode(Screen,INPUT_PULLUP);                       // Screen is an input pullup
lcd.begin(15,2);                                    // Setup the rows an colums needed
}

void loop() {
  // put your main code here, to run repeatedly:
//delay(100);                                         // Used to make the lcd more legible
HoldScreen= STATEScreen;
STATEScreen = digitalRead(Screen);                  // Reads the value of Screen and stores it under the value "STATE"
  if (STATEScreen == 1 && HoldScreen == 0) {        // Check if the PB is pressed and held
    Count = Count + 1;                              // Add 1 to the count
    lcd.clear();

    
    if (Count >= 2) {                               // If the counter is above or equal to 2 then reset it
      Count = 0;
    }
  else if (STATEScreen == LOW) {                    // If PB is not being pressed, set it as not pressed
    HoldScreen = 0;

    }
  }
int SCount = SetValue();                            // Store the function as an integer
float PGain = PValue();
int PV = analogRead(Sense);                         // Read the input from the photo resistor 
int Set = SCount;                                   // Store SCount as Set
Error = Set - PV;
CO = PGain*Error;
if (CO > 255) {                             // If the value read on the sensor is below the set point, turn on the LED
  CO = 255;
}
if (CO<0) {                             // If the value read on the sensor is above the set point, turn the LED off
  CO = 0;
}
//long CO = map(Set, 0, 1023, 0, 255);
analogWrite(LED,CO);                                // Output CO

switch (Count) {

  case 0:
  // If screen 1 is displaying then show PV, SP and Error on the lcd
  lcd.setCursor(0,0);
  lcd.print("PV=");
  lcd.print(PV);
  lcd.print(" ");


  lcd.setCursor(0,1);
  lcd.print("SP=");
  lcd.print(Set);
  lcd.print(" ");


  lcd.setCursor(7,1);
  lcd.print("Er=");
  lcd.print(Error);
  lcd.print(" ");
  break;

  case 1:
  // If screen 2 is displaying then show CO and PGain on the lcd

  lcd.setCursor(0,0);
  lcd.print("CO=");
  lcd.print(CO);
  lcd.print(" ");


  lcd.setCursor(0,1);
  lcd.print("PGain=");
  lcd.print(PGain);
  lcd.print(" ");
  break;

}
Serial.print("PV= ");
Serial.print(PV);
Serial.print("\t");
Serial.print("SP= ");
Serial.println(Set);
// Serial.print("\t");
// Serial.print("Error= ");
// Serial.print(Error); 
// Serial.print("\t"); 
// Serial.print("CO= ");
// Serial.print(CO);  
// Serial.print("\t");
// Serial.print("PGain= ");
// Serial.println(PGain);  
}