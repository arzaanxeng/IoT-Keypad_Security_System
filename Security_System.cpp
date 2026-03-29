# IoT-Keypad_Security_System

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

// LCD Setup (Address 0x27 is common)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Keypad Setup
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {7, 6, 5, 4}; 
byte colPins[COLS] = {3, 2, 12, 11}; // Avoided 0 and 1 to keep Serial free

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Variables
String correctPass = "1234"; // Changed to numbers for keypad ease
String inputPass = "";
int greenLED = 8;
int redLED = 9;
int buzzer = 10;

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  resetSystem();
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    if (key == '#') { // Use '#' as Enter
      checkPassword();
    } else if (key == '*') { // Use '*' to Clear
      inputPass = "";
      updateLCD();
    } else {
      inputPass += key;
      updateLCD();
    }
  }
}

void updateLCD() {
  lcd.setCursor(0, 1);
  String masked = "";
  for(int i=0; i < inputPass.length(); i++) masked += "*"; 
  lcd.print(masked + "                "); // Spaces clear old chars
}

void checkPassword() {
  lcd.clear();
  if (inputPass == correctPass) {
    lcd.print("ACCESS GRANTED");
    digitalWrite(greenLED, HIGH);
    delay(2000);
    digitalWrite(greenLED, LOW);
  } else {
    lcd.print("ACCESS DENIED!");
    digitalWrite(redLED, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(2000);
    digitalWrite(redLED, LOW);
    digitalWrite(buzzer, LOW);
  }
  resetSystem();
}

void resetSystem() {
  inputPass = "";
  lcd.clear();
  lcd.print("ENTER PASSWORD:");
  lcd.setCursor(0, 1);
}
